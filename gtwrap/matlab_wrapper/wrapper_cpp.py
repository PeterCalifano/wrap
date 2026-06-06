"""MATLAB wrapper generator targeting the modern C++ MEX API.

`MatlabWrapperCpp` reuses the entire AST walk and `.m` generation of
`MatlabWrapper`; it only overrides the handful of emit points that differ
between the legacy C MEX API (mex.h) and the C++ MEX API (mex.hpp). The
generated `.m` files are byte-identical to the C target - only the
`*_wrapper.cpp` and the runtime header (`matlab_cpp.h`) change.
"""

# pylint: disable=consider-using-f-string

import textwrap

from gtwrap.matlab_wrapper.templates_cpp import WrapperTemplateCpp
from gtwrap.matlab_wrapper.wrapper import MatlabWrapper


class MatlabWrapperCpp(MatlabWrapper):
    """Wrap C++ into a MATLAB toolbox using the matlab::mex C++ API."""

    # Diverging templates (see WrapperTemplateCpp).
    _delete_all_objects_template = WrapperTemplateCpp.delete_all_objects
    _rtti_register_template = WrapperTemplateCpp.rtti_register
    _upcast_from_void_template = \
        WrapperTemplateCpp.collector_function_upcast_from_void

    def _runtime_include(self):
        return "#include <{}/matlab_cpp.h>\n#include <map>".format(
            self._include_namespace())

    def _ctx_arg(self):
        return 'ctx, '

    def _include_namespace(self):
        """Match the include namespace used by the base `.tpl` header."""
        header = self.wrapper_file_headers
        # Base header looks like `#include <wrap/matlab.h>`; reuse that prefix.
        start = header.find('<')
        end = header.find('/matlab.h>')
        if start != -1 and end != -1:
            return header[start + 1:end]
        return "gtwrap"

    def _collector_signature(self, name):
        return ("void {}(Context& ctx, int nargout, OutputList& out, "
                "int nargin, WrapIn in)\n").format(name)

    def _collector_insert_make_base_body(self, collector_func, class_name,
                                         class_name_separated):
        body = textwrap.indent(textwrap.dedent('''\
            typedef std::shared_ptr<{class_name_sep}> Shared;\n
            Shared *self = get_handle<Shared>(in[0]);
            collector_{class_name}.insert(self);
        ''').format(class_name_sep=class_name_separated, class_name=class_name),
            prefix='  ')

        if collector_func[1].parent_class:
            body += textwrap.indent(textwrap.dedent('''
                typedef std::shared_ptr<{}> SharedBase;
                out[0] = make_handle<SharedBase>(new SharedBase(*self));
            ''').format(collector_func[1].parent_class), prefix='  ')

        return body

    def _collector_constructor_body(self, collector_func, extra, class_name,
                                    class_name_separated):
        base = ''
        params, body_args = self._wrapper_unwrap_arguments(
            extra.args, instantiated_class=collector_func[1])

        if collector_func[1].parent_class:
            base += textwrap.indent(textwrap.dedent('''
                typedef std::shared_ptr<{}> SharedBase;
                out[1] = make_handle<SharedBase>(new SharedBase(*self));
            ''').format(collector_func[1].parent_class), prefix='  ')

        return textwrap.dedent('''\
              typedef std::shared_ptr<{class_name_sep}> Shared;\n
            {body_args}  Shared *self = new Shared(new {class_name_sep}({params}));
              collector_{class_name}.insert(self);
              out[0] = make_handle<Shared>(self);
            {base}''').format(class_name_sep=class_name_separated,
                              body_args=body_args,
                              params=params,
                              class_name=class_name,
                              base=base)

    def _collector_deconstructor_body(self, class_name, class_name_separated):
        return textwrap.indent(textwrap.dedent('''\
            typedef std::shared_ptr<{class_name_sep}> Shared;
            checkArguments("delete_{class_name}",nargout,nargin,1);
            Shared *self = get_handle<Shared>(in[0]);
            Collector_{class_name}::iterator item;
            item = collector_{class_name}.find(self);
            if(item == collector_{class_name}.end()) {{
              return;
            }}
            collector_{class_name}.erase(item);
            delete self;
        ''').format(class_name_sep=class_name_separated, class_name=class_name),
            prefix='  ')

    def mex_function(self):
        cases = self._mex_function_cases(
            '{name}(ctx, (int)outputs.size(), out, (int)in.size(), in);',
            prefix='        ')

        return WrapperTemplateCpp.mex_function.format(
            module_name=self.module_name, cases=cases)
