function run_benchmark(toolboxDir, api, nIters, sizes)
% RUN_BENCHMARK  Time + measure memory of a generated MEX wrapper.
%   run_benchmark(toolboxDir, api, nIters, sizes) adds the generated toolbox to
%   the path and benchmarks the wrap/unwrap marshalling surface. The generated
%   .m classdef is byte-identical between the 'c' and 'cpp' MEX targets, so the
%   MATLAB-side dispatch cost is identical across backends and any measured
%   difference is attributable to the MEX runtime (matlab.h vs matlab_cpp.h).
%
%   api    'c' or 'cpp' (default 'cpp'); labels the emitted rows only.
%   nIters total reps per size-independent op (default 2000); matrix/vector ops
%          scale reps down with size to bound runtime.
%   sizes  vector of dimensions for vector length / NxN matrices
%          (default [8 64 256 1024]).
%
%   Emits machine-parseable lines for run_benchmark.py to scrape:
%       BENCH,<op>,<api>,<reps>,<size>,<median_us>
%       BENCH,vmrss_<phase>,<api>,0,0,<kB>     (Linux /proc; NA otherwise)
%       BENCH,vmhwm_<phase>,<api>,0,0,<kB>
%   and the sentinel "ALL BENCHMARKS COMPLETE". Errors out (non-zero exit under
%   -batch) on failure.
%
%   Timing uses a block-median scheme (median over nBlocks tight loops) for
%   robustness to GC/OS jitter; the reported figure is microseconds per call.
%
%   Helpers are subfunctions (never nested) so this workspace is not static -
%   the wrapper writes its RTTI registry into it, which a static workspace
%   would forbid (same constraint as run_test.m).
  if nargin < 2 || isempty(api),    api = 'cpp';            end
  if nargin < 3 || isempty(nIters), nIters = 2000;          end
  if nargin < 4 || isempty(sizes),  sizes = [8 64 256 1024]; end
  addpath(toolboxDir);

  nBlocks = 15;
  pid = feature('getpid');
  c = demo.Counter(5);

  % --- warmup: load the module, JIT the dispatch, prime the allocator ---
  noop_call(c); c.echoInt(3); c.echoBool(true);
  for s = sizes(:)'
    c.echoVector(ones(s, 1));
    c.echoMatrix(ones(s, s));
    c.traceMatrix(ones(s, s));
    c.ramp(s, s);
  end
  ctor_dtor(); try_throw(c);
  emit_rss('baseline', api, pid);

  % --- size-independent ops (size column = 0) ---
  emit('noop',              api, nIters, 0, bench(@() noop_call(c), nIters, nBlocks));
  emit('scalar_int',        api, nIters, 0, bench(@() c.echoInt(7), nIters, nBlocks));
  emit('scalar_bool',       api, nIters, 0, bench(@() c.echoBool(true), nIters, nBlocks));
  emit('construct_destroy', api, nIters, 0, bench(@ctor_dtor, max(50, round(nIters/4)), nBlocks));
  emit('error_path',        api, nIters, 0, bench(@() try_throw(c), max(50, round(nIters/4)), nBlocks));

  % --- size-swept marshalling ops ---
  for s = sizes(:)'
    reps = max(20, round(nIters * 8 / s));   % keep work per size bounded
    v = ones(s, 1);
    M = ones(s, s);
    emit('vector_echo',   api, reps, s, bench(@() c.echoVector(v), reps, nBlocks));
    emit('matrix_echo',   api, reps, s, bench(@() c.echoMatrix(M), reps, nBlocks));
    emit('matrix_unwrap', api, reps, s, bench(@() c.traceMatrix(M), reps, nBlocks));
    emit('matrix_wrap',   api, reps, s, bench(@() c.ramp(s, s), reps, nBlocks));
    if s == sizes(end)
      emit_rss('matrix', api, pid);
    end
  end

  % --- lifecycle memory snapshot ---
  pool = cell(1, 200);
  for k = 1:numel(pool), pool{k} = demo.Counter(k); end
  for k = 1:numel(pool), delete(pool{k}); end
  emit_rss('lifecycle', api, pid);

  delete(c);
  clear mex;                  % triggers ~MexFunction / _deleteAllObjects
  emit_rss('end', api, pid);

  fprintf(1, 'ALL BENCHMARKS COMPLETE\n');
end

% --- timing -------------------------------------------------------------------
function us = bench(fn, reps, nBlocks)
% Median over nBlocks tight loops of `reps` calls; microseconds per call.
  times = zeros(nBlocks, 1);
  for b = 1:nBlocks
    t = tic;
    for k = 1:reps
      fn();
    end
    times(b) = toc(t);
  end
  us = median(times) / reps * 1e6;
end

% --- ops that cannot be expressed as a value-returning anonymous fn -----------
function noop_call(c)
  c.noop();                   % void return: must be a statement, not an expr
end

function ctor_dtor()
  o = demo.Counter(5);
  delete(o);
end

function try_throw(c)
  try
    c.mayThrow(true);
  catch
  end
end

% --- emit ---------------------------------------------------------------------
function emit(op, api, reps, sz, us)
  fprintf(1, 'BENCH,%s,%s,%d,%d,%.4f\n', op, api, reps, sz, us);
end

function emit_rss(phase, api, pid)
  [rss, hwm] = read_status(pid);
  emit_kb(['vmrss_' phase], api, rss);
  emit_kb(['vmhwm_' phase], api, hwm);
end

function emit_kb(metric, api, kb)
  if isnan(kb)
    fprintf(1, 'BENCH,%s,%s,0,0,NA\n', metric, api);
  else
    fprintf(1, 'BENCH,%s,%s,0,0,%d\n', metric, api, kb);
  end
end

function [rss, hwm] = read_status(pid)
% Read VmRSS / VmHWM (kB) from /proc/<pid>/status; NaN off Linux.
  rss = NaN; hwm = NaN;
  fn = sprintf('/proc/%d/status', pid);
  if exist(fn, 'file') ~= 2
    return;
  end
  txt = fileread(fn);
  rss = parse_kb(txt, 'VmRSS');
  hwm = parse_kb(txt, 'VmHWM');
end

function kb = parse_kb(txt, key)
  kb = NaN;
  tok = regexp(txt, [key '\:\s*(\d+)\s*kB'], 'tokens', 'once');
  if ~isempty(tok)
    kb = str2double(tok{1});
  end
end
