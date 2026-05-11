%class MyFactorPosePoint2, generated wrapper class
%
%-------Constructors-------
%MyFactorPosePoint2(size_t key1, size_t key2, double measured, Base noiseModel)
%
%-------Methods-------
%print(string s, KeyFormatter keyFormatter) : returns void
%
classdef MyFactorPosePoint2 < handle
  properties
    ptr_MyFactorPosePoint2 = 0
  end
  methods
    function obj = MyFactorPosePoint2(varargin)
      if nargin == 2 && isa(varargin{1}, 'uint64') && varargin{1} == uint64(5139824614673773682)
        my_ptr = varargin{2};
        class_wrapper(87, my_ptr);
      elseif nargin == 4 && isa(varargin{1},'numeric') && isa(varargin{2},'numeric') && isa(varargin{3},'double') && isa(varargin{4},'gtsam.noiseModel.Base')
        my_ptr = class_wrapper(88, varargin{1}, varargin{2}, varargin{3}, varargin{4});
      else
        error('Arguments do not match any overload of MyFactorPosePoint2 constructor');
      end
      obj.ptr_MyFactorPosePoint2 = my_ptr;
    end

    function delete(obj)
      class_wrapper(89, obj.ptr_MyFactorPosePoint2);
    end

    function display(obj), obj.print(''); end
    %DISPLAY Calls print on the object
    function disp(obj), obj.display; end
    %DISP Calls print on the object
    function varargout = print(this, varargin)
      % PRINT usage: print(string s, KeyFormatter keyFormatter) : returns void
      % Documentation can be found in the wrapped project.
      if length(varargin) == 2 && isa(varargin{1},'char') && isa(varargin{2},'gtsam.KeyFormatter')
        class_wrapper(90, this, varargin{:});
        return
      end
      % PRINT usage: print(string s) : returns void
      % Documentation can be found in the wrapped project.
      if length(varargin) == 1 && isa(varargin{1},'char')
        class_wrapper(91, this, varargin{:});
        return
      end
      % PRINT usage: print() : returns void
      % Documentation can be found in the wrapped project.
      if length(varargin) == 0
        class_wrapper(92, this, varargin{:});
        return
      end
      error('Arguments do not match any overload of function MyFactorPosePoint2.print');
    end

  end

  methods(Static = true)
  end
end
