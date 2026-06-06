#include <wrap/matlab_cpp.h>
#include <map>



typedef ScopedTemplate<Result> ScopedTemplateResult;

typedef std::set<std::shared_ptr<TemplatedConstructor>*> Collector_TemplatedConstructor;
static Collector_TemplatedConstructor collector_TemplatedConstructor;
typedef std::set<std::shared_ptr<ScopedTemplateResult>*> Collector_ScopedTemplateResult;
static Collector_ScopedTemplateResult collector_ScopedTemplateResult;


void _deleteAllObjects(matlab::engine::MATLABEngine* engine)
{
  bool anyDeleted = false;
  { for(Collector_TemplatedConstructor::iterator iter = collector_TemplatedConstructor.begin();
      iter != collector_TemplatedConstructor.end(); ) {
    delete *iter;
    collector_TemplatedConstructor.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_ScopedTemplateResult::iterator iter = collector_ScopedTemplateResult.begin();
      iter != collector_ScopedTemplateResult.end(); ) {
    delete *iter;
    collector_ScopedTemplateResult.erase(iter++);
    anyDeleted = true;
  } }

  if(anyDeleted) {
    matlab::data::ArrayFactory f_;
    engine->feval(u"fprintf", 0, std::vector<matlab::data::Array>({
      f_.createScalar<double>(1.0), f_.createCharArray("%s"),
      f_.createCharArray(
        "WARNING:  Wrap modules with variables in the workspace have been reloaded due to\n"
        "calling destructors, call 'clear all' again if you plan to now recompile a wrap\n"
        "module, so that your recompiled module is used instead of the old one.\n")}));
  }
}

void _template_RTTIRegister(matlab::engine::MATLABEngine* engine) {
  bool alreadyCreated = false;
  try {
    engine->getVariable(u"gtsam_template_rttiRegistry_created",
                        matlab::engine::WorkspaceType::GLOBAL);
    alreadyCreated = true;
  } catch(const std::exception&) {
    alreadyCreated = false;
  }

  if(!alreadyCreated) {
    std::map<std::string, std::string> types;

    matlab::data::ArrayFactory f_;
    try {
      matlab::data::StructArray existing(engine->getVariable(
        u"gtsamwrap_rttiRegistry", matlab::engine::WorkspaceType::GLOBAL));
      for(const auto& field: existing.getFieldNames()) {
        std::string key(field);
        types[key] = matlab::data::CharArray(existing[0][field]).toAscii();
      }
    } catch(const std::exception&) {}

    std::vector<std::string> fieldNames;
    for(const auto& rtti_matlab: types) fieldNames.push_back(rtti_matlab.first);
    matlab::data::StructArray registry = f_.createStructArray({1, 1}, fieldNames);
    for(const auto& rtti_matlab: types) {
      registry[0][rtti_matlab.first] = f_.createCharArray(rtti_matlab.second);
    }
    engine->setVariable(u"gtsamwrap_rttiRegistry", registry,
                        matlab::engine::WorkspaceType::GLOBAL);
    engine->setVariable(u"gtsam_template_rttiRegistry_created",
                        f_.createScalar<std::int8_t>(0),
                        matlab::engine::WorkspaceType::GLOBAL);
  }
}

void TemplatedConstructor_collectorInsertAndMakeBase_0(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<TemplatedConstructor> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_TemplatedConstructor.insert(self);
}

void TemplatedConstructor_constructor_1(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<TemplatedConstructor> Shared;

  Shared *self = new Shared(new TemplatedConstructor());
  collector_TemplatedConstructor.insert(self);
  out[0] = make_handle<Shared>(self);
}

void TemplatedConstructor_constructor_2(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<TemplatedConstructor> Shared;

  string arg = unwrap< string >(in[0]);
  Shared *self = new Shared(new TemplatedConstructor(arg));
  collector_TemplatedConstructor.insert(self);
  out[0] = make_handle<Shared>(self);
}

void TemplatedConstructor_constructor_3(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<TemplatedConstructor> Shared;

  int arg = unwrap< int >(in[0]);
  Shared *self = new Shared(new TemplatedConstructor(arg));
  collector_TemplatedConstructor.insert(self);
  out[0] = make_handle<Shared>(self);
}

void TemplatedConstructor_constructor_4(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<TemplatedConstructor> Shared;

  double arg = unwrap< double >(in[0]);
  Shared *self = new Shared(new TemplatedConstructor(arg));
  collector_TemplatedConstructor.insert(self);
  out[0] = make_handle<Shared>(self);
}

void TemplatedConstructor_deconstructor_5(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<TemplatedConstructor> Shared;
  checkArguments("delete_TemplatedConstructor",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_TemplatedConstructor::iterator item;
  item = collector_TemplatedConstructor.find(self);
  if(item == collector_TemplatedConstructor.end()) {
    return;
  }
  collector_TemplatedConstructor.erase(item);
  delete self;
}

void ScopedTemplateResult_collectorInsertAndMakeBase_6(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ScopedTemplate<Result>> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_ScopedTemplateResult.insert(self);
}

void ScopedTemplateResult_constructor_7(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ScopedTemplate<Result>> Shared;

  Result::Value& arg = *unwrap_shared_ptr< Result::Value >(ctx, in[0], "ptr_Result::Value");
  Shared *self = new Shared(new ScopedTemplate<Result>(arg));
  collector_ScopedTemplateResult.insert(self);
  out[0] = make_handle<Shared>(self);
}

void ScopedTemplateResult_deconstructor_8(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ScopedTemplate<Result>> Shared;
  checkArguments("delete_ScopedTemplateResult",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_ScopedTemplateResult::iterator item;
  item = collector_ScopedTemplateResult.find(self);
  if(item == collector_ScopedTemplateResult.end()) {
    return;
  }
  collector_ScopedTemplateResult.erase(item);
  delete self;
}


class MexFunction : public matlab::mex::Function {
 public:
  ~MexFunction() override {
    try {
      // getEngine() hands back a fresh shared_ptr; keep it alive
      // for the duration of cleanup. Destructors must not throw
      // during clear/exit.
      std::shared_ptr<matlab::engine::MATLABEngine> engine = getEngine();
      _deleteAllObjects(engine.get());
    } catch(...) {
      // Best-effort cleanup at unload: suppress engine/printing
      // failures because throwing from a destructor can terminate
      // MATLAB.
    }
  }

  void operator()(matlab::mex::ArgumentList outputs,
                  matlab::mex::ArgumentList inputs) override {
    std::shared_ptr<matlab::engine::MATLABEngine> engine = getEngine();
    Context ctx{engine.get(), matlab::data::ArrayFactory()};
    if(!rttiRegistered_) {
      _template_RTTIRegister(ctx.engine);
      rttiRegistered_ = true;
    }

    gtwrap::CoutRedirect coutRedirect(ctx.engine);

    int id = unwrap<int>(inputs[0]);
    WrapIn in(inputs, 1);
    OutputList out;

    try {
      switch(id) {
        case 0:
          TemplatedConstructor_collectorInsertAndMakeBase_0(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 1:
          TemplatedConstructor_constructor_1(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 2:
          TemplatedConstructor_constructor_2(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 3:
          TemplatedConstructor_constructor_3(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 4:
          TemplatedConstructor_constructor_4(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 5:
          TemplatedConstructor_deconstructor_5(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 6:
          ScopedTemplateResult_collectorInsertAndMakeBase_6(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 7:
          ScopedTemplateResult_constructor_7(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 8:
          ScopedTemplateResult_deconstructor_8(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
      }
    } catch(const matlab::engine::MATLABException&) {
      throw;  // already a MATLAB error; report it with its identifier
    } catch(const std::exception& e) {
      gtwrap::MexErrMsgTxt(("Exception from wrapped C++ code:\n" + std::string(e.what()) + "\n").c_str());
    }

    for(size_t i = 0; i < out.v.size(); ++i)
      outputs[i] = out.v[i];
  }

 private:
  bool rttiRegistered_ = false;
};
