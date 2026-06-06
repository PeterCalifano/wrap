"""Code generation templates for the modern C++ MEX API Matlab wrapper.

Only the fragments that genuinely diverge from the legacy C MEX API live here;
everything else is reused from :class:`WrapperTemplate` in ``templates.py``.
"""

import textwrap


class WrapperTemplateCpp:
    """C++ MEX API counterparts of the diverging :class:`WrapperTemplate` entries."""

    # Warning emitted from _deleteAllObjects via the engine instead of mexPrintf.
    delete_all_objects = textwrap.dedent('''
            void _deleteAllObjects(matlab::engine::MATLABEngine* engine)
            {{
              bool anyDeleted = false;
            {delete_objs}
              if(anyDeleted) {{
                matlab::data::ArrayFactory f_;
                engine->feval(u"fprintf", 0, std::vector<matlab::data::Array>({{
                  f_.createScalar<double>(1.0), f_.createCharArray("%s"),
                  f_.createCharArray(
                    "WARNING:  Wrap modules with variables in the workspace have been reloaded due to\\n"
                    "calling destructors, call \'clear all\' again if you plan to now recompile a wrap\\n"
                    "module, so that your recompiled module is used instead of the old one.\\n")}}));
              }}
            }}
        ''')

    # RTTI registry kept in the global workspace as a StructArray. Existing
    # entries (possibly from other wrap modules) are merged, not overwritten.
    rtti_register = textwrap.dedent('''\
            void _{module_name}_RTTIRegister(matlab::engine::MATLABEngine* engine) {{
              bool alreadyCreated = false;
              try {{
                engine->getVariable(u"gtsam_{module_name}_rttiRegistry_created",
                                    matlab::engine::WorkspaceType::GLOBAL);
                alreadyCreated = true;
              }} catch(const std::exception&) {{
                alreadyCreated = false;
              }}

              if(!alreadyCreated) {{
                std::map<std::string, std::string> types;
            {rtti_classes}
                matlab::data::ArrayFactory f_;
                try {{
                  matlab::data::StructArray existing(engine->getVariable(
                    u"gtsamwrap_rttiRegistry", matlab::engine::WorkspaceType::GLOBAL));
                  for(const auto& field: existing.getFieldNames()) {{
                    std::string key(field);
                    types[key] = matlab::data::CharArray(existing[0][field]).toAscii();
                  }}
                }} catch(const std::exception&) {{}}

                std::vector<std::string> fieldNames;
                for(const auto& rtti_matlab: types) fieldNames.push_back(rtti_matlab.first);
                matlab::data::StructArray registry = f_.createStructArray({{1, 1}}, fieldNames);
                for(const auto& rtti_matlab: types) {{
                  registry[0][rtti_matlab.first] = f_.createCharArray(rtti_matlab.second);
                }}
                engine->setVariable(u"gtsamwrap_rttiRegistry", registry,
                                    matlab::engine::WorkspaceType::GLOBAL);
                engine->setVariable(u"gtsam_{module_name}_rttiRegistry_created",
                                    f_.createScalar<std::int8_t>(0),
                                    matlab::engine::WorkspaceType::GLOBAL);
              }}
            }}
        ''')

    collector_function_upcast_from_void = textwrap.dedent('''\
            void {class_name}_upcastFromVoid_{id}(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in) {{
              typedef std::shared_ptr<{cpp_name}> Shared;
              std::shared_ptr<void> *asVoid = get_handle<std::shared_ptr<void>>(in[0]);
              Shared *self = new Shared(std::static_pointer_cast<{cpp_name}>(*asVoid));
              collector_{collector_name}.insert(self);
              out[0] = make_handle<Shared>(self);
            }}\n
        ''')

    # Entry point: a matlab::mex::Function whose lifetime owns the collectors.
    # mexAdapter.hpp is included once via matlab_cpp.h (before this class). The
    # per-call Context carries the engine + factory into the helpers, so there
    # is no process-global engine state.
    mex_function = textwrap.dedent('''
            class MexFunction : public matlab::mex::Function {{
             public:
              ~MexFunction() override {{
                try {{
                  // getEngine() hands back a fresh shared_ptr; keep it alive
                  // for the duration of cleanup. Destructors must not throw
                  // during clear/exit.
                  std::shared_ptr<matlab::engine::MATLABEngine> engine = getEngine();
                  _deleteAllObjects(engine.get());
                }} catch(...) {{
                  // Best-effort cleanup at unload: suppress engine/printing
                  // failures because throwing from a destructor can terminate
                  // MATLAB.
                }}
              }}

              void operator()(matlab::mex::ArgumentList outputs,
                              matlab::mex::ArgumentList inputs) override {{
                std::shared_ptr<matlab::engine::MATLABEngine> engine = getEngine();
                Context ctx{{engine.get(), matlab::data::ArrayFactory()}};
                if(!rttiRegistered_) {{
                  _{module_name}_RTTIRegister(ctx.engine);
                  rttiRegistered_ = true;
                }}

                gtwrap::CoutRedirect coutRedirect(ctx.engine);

                int id = unwrap<int>(inputs[0]);
                WrapIn in(inputs, 1);
                OutputList out;

                try {{
                  switch(id) {{
            {cases}      }}
                }} catch(const matlab::engine::MATLABException&) {{
                  throw;  // already a MATLAB error; report it with its identifier
                }} catch(const std::exception& e) {{
                  gtwrap::MexErrMsgTxt(("Exception from wrapped C++ code:\\n" + std::string(e.what()) + "\\n").c_str());
                }}

                for(size_t i = 0; i < out.v.size(); ++i)
                  outputs[i] = out.v[i];
              }}

             private:
              bool rttiRegistered_ = false;
            }};
        ''')
