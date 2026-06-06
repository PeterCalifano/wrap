function run_cleanup(toolboxDir, mode)
% RUN_CLEANUP  Stress the wrapper's teardown paths for segfault safety.
%   run_cleanup(toolboxDir, mode) drives one of:
%     'reload' : use the wrapper, `clear all` (forces module teardown and, for
%                the C++ target, ~MexFunction / _deleteAllObjects), then create
%                and use brand-new objects. Verifies no crash and that fresh
%                objects work after a teardown.
%     'exit'   : use the wrapper, leave a live object in the collector, and let
%                the -batch session end so MATLAB unloads the module on exit.
%                Verifies the on-exit cleanup does not segfault.
%   Errors out (non-zero exit) on a failed assertion; a segfault shows up as a
%   non-zero MATLAB exit independently.
  addpath(toolboxDir);
  if nargin < 2
    mode = 'reload';
  end

  switch mode
    case 'reload'
      % Use a mix of plain and virtual (RTTI) objects, then delete some and
      % leave others live across the teardown.
      a = demo.Counter(3);
      a.increment();
      assert(a.count() == 4, 'pre-clear object works');
      b = demo.makeCircle(2.0);
      assert(abs(b.area() - pi * 4) < 1e-9, 'pre-clear virtual works');
      leftLive = demo.Counter(123);  %#ok<NASGU> intentionally not deleted

      % Tear everything down: clears the workspace and unloads the MEX module,
      % running the destructor path over any still-live collector entries.
      % `clear all` also clears this function's locals (including toolboxDir),
      % but it does not touch the MATLAB path, so the package stays resolvable.
      clear all  %#ok<CLALL>

      % Create fresh objects; the RTTI registry and collectors must be rebuilt
      % cleanly after the teardown.
      c = demo.Counter(10);
      assert(c.count() == 10, 'fresh scalar object after clear');
      d = demo.makeCircle(1.0);
      assert(abs(d.area() - pi) < 1e-9, 'fresh virtual object after clear');
      e = demo.Counter(1);
      e.setName('reborn');
      assert(strcmp(e.name(), 'reborn'), 'fresh object methods after clear');

      delete(c);
      delete(d);
      delete(e);
      fprintf(1, 'CLEANUP reload OK\n');

    case 'exit'
      x = demo.Counter(7);
      x.increment();
      assert(x.count() == 8, 'object works before exit');
      y = demo.Circle(3.0);
      assert(abs(y.radius() - 3.0) < 1e-12, 'derived object works before exit');
      z = demo.makeCircle(2.0);
      assert(abs(z.area() - pi * 4) < 1e-9, 'virtual object works before exit');
      % Deliberately leave x, y and z live: the module must clean them up when
      % the -batch session exits, without a segfault.
      fprintf(1, 'CLEANUP exit OK\n');

    otherwise
      error('run_cleanup:mode', 'unknown mode %s', mode);
  end
end
