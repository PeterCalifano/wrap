function run_test(toolboxDir, api)
% RUN_TEST  Exercise a generated MEX wrapper against real libgtsam.
%   run_test(toolboxDir) or run_test(toolboxDir, api) adds the generated
%   toolbox to the path and runs a battery of checks covering scalars,
%   strings, Eigen vectors/matrices (incl. column-major ordering), enums,
%   virtual dispatch, raw-pointer args, the error path and object lifetime.
%   Errors out (non-zero exit under -batch) if any check fails.
%
%   api is 'c' or 'cpp' (default 'cpp'). The core surface must pass on both;
%   a few checks assert C++-target-only refinements (bool returned as logical,
%   a 'wrap:error' identifier, and type/shape-named mismatch messages) and are
%   skipped for the C target, which is otherwise identical.
%
%   `check` is a subfunction (not nested) so this workspace is not static -
%   the wrapper writes its RTTI registry to the global workspace, which the
%   caller must allow.
  if nargin < 2
    api = 'cpp';
  end
  isCpp = strcmp(api, 'cpp');
  addpath(toolboxDir);
  failures = 0;

  % --- scalars ---
  c = demo.Counter(5);
  failures = failures + ~check(c.count() == 5, 'ctor int + count');
  c.increment();
  failures = failures + ~check(c.count() == 6, 'increment');
  failures = failures + ~check(abs(c.half() - 3.0) < 1e-12, 'double return');
  pos = c.isPositive();
  failures = failures + ~check(pos == 1, 'bool return truthy');
  if isCpp
    failures = failures + ~check(islogical(pos), ...
                                 'bool return is logical (cpp)');
  end

  d = demo.Counter();
  failures = failures + ~check(d.count() == 0, 'default ctor');

  % --- string round trip ---
  failures = failures + ~check(strcmp(c.name(), 'counter'), 'string return');
  c.setName('hello');
  failures = failures + ~check(strcmp(c.name(), 'hello'), 'string arg + return');

  % --- Eigen vector ---
  v = c.asVector();           % count==6 -> [6;7;8]
  failures = failures + ~check(isequal(size(v), [3 1]), 'vector shape');
  failures = failures + ~check(isequal(v, [6;7;8]), 'vector values');
  failures = failures + ~check(c.sumVector([1;2;3]) == 6, 'vector arg sum');

  % --- Eigen matrix + column-major ordering edge case ---
  M = c.ramp(2, 3);           % m(i,j)=10*i+j -> [0 1 2; 10 11 12]
  failures = failures + ~check(isequal(size(M), [2 3]), 'matrix shape');
  failures = failures + ~check(isequal(M, [0 1 2; 10 11 12]), ...
                               'matrix values + ordering');
  failures = failures + ~check(c.traceMatrix([1 0; 0 5]) == 6, ...
                               'matrix arg trace');
  failures = failures + ~check(c.traceMatrixView([1 0; 0 5]) == 6, ...
                               'matrix view arg trace');
  failures = failures + ~check(c.elem([1 2; 3 4], 0, 1) == 2, ...
                               'matrix (0,1) maps to MATLAB (1,2)');

  % --- enum arg + return ---
  failures = failures + ~check(c.colorValue(demo.Color.Blue) == 2, 'enum arg');
  failures = failures + ~check(c.favorite() == demo.Color.Green, 'enum return');

  % --- raw-pointer (non-owning) object argument (unwrap_ptr) ---
  other = demo.Counter(10);
  failures = failures + ~check(c.addCount(other) == c.count() + 10, ...
                               'raw-pointer object argument');

  % --- virtual base: a function declared to return Shape* yields a Shape
  %     proxy whose virtual area() dispatches to the underlying Circle. ---
  s = demo.makeCircle(2.0);
  failures = failures + ~check(isa(s, 'demo.Shape'), ...
                               'base-returning factory yields declared type');
  failures = failures + ~check(abs(s.area() - pi * 4) < 1e-9, ...
                               'virtual dispatch through base proxy');

  % --- derived class directly: ctor + derived-only method; constructing a
  %     Circle also registers it under the MyBase collector. ---
  c2 = demo.Circle(3.0);
  failures = failures + ~check(isa(c2, 'demo.Circle'), 'derived ctor type');
  failures = failures + ~check(abs(c2.area() - pi * 9) < 1e-9, ...
                               'derived virtual method');
  failures = failures + ~check(abs(c2.radius() - 3.0) < 1e-12, ...
                               'derived-only method');

  % --- typed mismatch reaching C++ (the .m overload guard checks class but
  %     not size for scalar args): pass a 1x2 where a scalar int is expected.
  %     The C++ unwrap error should name what was expected and what arrived. ---
  badScalar = false;
  try
    c.elem([1 0; 0 1], [1 2], 0);
  catch e
    badScalar = true;
    if isCpp
      failures = failures + ~check(contains(e.message, 'expected a scalar'), ...
                                   'mismatch error names the expectation');
      failures = failures + ~check(contains(e.message, '1x2 double'), ...
                                   'mismatch error names the actual shape/type');
    end
  end
  failures = failures + ~check(badScalar, 'non-scalar int argument rejected');

  % --- error path ---
  threw = false;
  try
    c.mayThrow(true);
  catch e
    threw = true;
    failures = failures + ~check(contains(e.message, 'intentional error'), ...
                                 'error message propagated');
    if isCpp
      failures = failures + ~check(strcmp(e.identifier, 'wrap:error'), ...
                                   'error identifier propagated (MATLABException)');
    end
  end
  failures = failures + ~check(threw, 'exception thrown to MATLAB');
  c.mayThrow(false);          % no-throw branch must not error

  % --- lifetime ---
  delete(c);
  delete(d);
  delete(s);
  delete(c2);
  delete(other);
  clear mex;                  % triggers ~MexFunction / _deleteAllObjects

  if failures > 0
    error('run_test:failures', '%d runtime check(s) failed', failures);
  end
  fprintf(1, 'ALL RUNTIME CHECKS PASSED\n');
end

function ok = check(cond, msg)
  ok = logical(cond);
  if ok
    fprintf(1, 'ok  : %s\n', msg);
  else
    fprintf(2, 'FAIL: %s\n', msg);
  end
end
