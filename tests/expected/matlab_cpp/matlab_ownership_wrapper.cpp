#include <wrap/matlab_cpp.h>
#include <map>





typedef std::set<std::shared_ptr<OwnedThing>*> Collector_OwnedThing;
static Collector_OwnedThing collector_OwnedThing;
typedef std::set<std::shared_ptr<VirtualBase>*> Collector_VirtualBase;
static Collector_VirtualBase collector_VirtualBase;
typedef std::set<std::shared_ptr<VirtualDerived>*> Collector_VirtualDerived;
static Collector_VirtualDerived collector_VirtualDerived;
typedef std::set<std::shared_ptr<wraptest::NamespacedVirtualBase>*> Collector_wraptestNamespacedVirtualBase;
static Collector_wraptestNamespacedVirtualBase collector_wraptestNamespacedVirtualBase;
typedef std::set<std::shared_ptr<wraptest::NamespacedVirtualDerived>*> Collector_wraptestNamespacedVirtualDerived;
static Collector_wraptestNamespacedVirtualDerived collector_wraptestNamespacedVirtualDerived;


void _deleteAllObjects(matlab::engine::MATLABEngine* engine)
{
  bool anyDeleted = false;
  { for(Collector_OwnedThing::iterator iter = collector_OwnedThing.begin();
      iter != collector_OwnedThing.end(); ) {
    delete *iter;
    collector_OwnedThing.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_VirtualBase::iterator iter = collector_VirtualBase.begin();
      iter != collector_VirtualBase.end(); ) {
    delete *iter;
    collector_VirtualBase.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_VirtualDerived::iterator iter = collector_VirtualDerived.begin();
      iter != collector_VirtualDerived.end(); ) {
    delete *iter;
    collector_VirtualDerived.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_wraptestNamespacedVirtualBase::iterator iter = collector_wraptestNamespacedVirtualBase.begin();
      iter != collector_wraptestNamespacedVirtualBase.end(); ) {
    delete *iter;
    collector_wraptestNamespacedVirtualBase.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_wraptestNamespacedVirtualDerived::iterator iter = collector_wraptestNamespacedVirtualDerived.begin();
      iter != collector_wraptestNamespacedVirtualDerived.end(); ) {
    delete *iter;
    collector_wraptestNamespacedVirtualDerived.erase(iter++);
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

void _matlab_ownership_RTTIRegister(matlab::engine::MATLABEngine* engine) {
  bool alreadyCreated = false;
  try {
    engine->getVariable(u"gtsam_matlab_ownership_rttiRegistry_created",
                        matlab::engine::WorkspaceType::GLOBAL);
    alreadyCreated = true;
  } catch(const std::exception&) {
    alreadyCreated = false;
  }

  if(!alreadyCreated) {
    std::map<std::string, std::string> types;
    types.insert(std::make_pair(typeid(VirtualBase).name(), "VirtualBase"));
    types.insert(std::make_pair(typeid(VirtualDerived).name(), "VirtualDerived"));
    types.insert(std::make_pair(typeid(wraptest::NamespacedVirtualBase).name(), "wraptestNamespacedVirtualBase"));
    types.insert(std::make_pair(typeid(wraptest::NamespacedVirtualDerived).name(), "wraptestNamespacedVirtualDerived"));

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
    engine->setVariable(u"gtsam_matlab_ownership_rttiRegistry_created",
                        f_.createScalar<std::int8_t>(0),
                        matlab::engine::WorkspaceType::GLOBAL);
  }
}

void OwnedThing_collectorInsertAndMakeBase_0(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<OwnedThing> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_OwnedThing.insert(self);
}

void OwnedThing_constructor_1(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<OwnedThing> Shared;

  Shared *self = new Shared(new OwnedThing());
  collector_OwnedThing.insert(self);
  out[0] = make_handle<Shared>(self);
}

void OwnedThing_deconstructor_2(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<OwnedThing> Shared;
  checkArguments("delete_OwnedThing",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_OwnedThing::iterator item;
  item = collector_OwnedThing.find(self);
  if(item == collector_OwnedThing.end()) {
    return;
  }
  collector_OwnedThing.erase(item);
  delete self;
}

void VirtualBase_collectorInsertAndMakeBase_3(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<VirtualBase> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_VirtualBase.insert(self);
}

void VirtualBase_upcastFromVoid_4(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in) {
  typedef std::shared_ptr<VirtualBase> Shared;
  std::shared_ptr<void> *asVoid = get_handle<std::shared_ptr<void>>(in[0]);
  Shared *self = new Shared(std::static_pointer_cast<VirtualBase>(*asVoid));
  collector_VirtualBase.insert(self);
  out[0] = make_handle<Shared>(self);
}

void VirtualBase_deconstructor_5(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<VirtualBase> Shared;
  checkArguments("delete_VirtualBase",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_VirtualBase::iterator item;
  item = collector_VirtualBase.find(self);
  if(item == collector_VirtualBase.end()) {
    return;
  }
  collector_VirtualBase.erase(item);
  delete self;
}

void VirtualDerived_collectorInsertAndMakeBase_6(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<VirtualDerived> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_VirtualDerived.insert(self);

  typedef std::shared_ptr<VirtualBase> SharedBase;
  out[0] = make_handle<SharedBase>(new SharedBase(*self));
}

void VirtualDerived_upcastFromVoid_7(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in) {
  typedef std::shared_ptr<VirtualDerived> Shared;
  std::shared_ptr<void> *asVoid = get_handle<std::shared_ptr<void>>(in[0]);
  Shared *self = new Shared(std::static_pointer_cast<VirtualDerived>(*asVoid));
  collector_VirtualDerived.insert(self);
  out[0] = make_handle<Shared>(self);
}

void VirtualDerived_constructor_8(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<VirtualDerived> Shared;

  Shared *self = new Shared(new VirtualDerived());
  collector_VirtualDerived.insert(self);
  out[0] = make_handle<Shared>(self);

  typedef std::shared_ptr<VirtualBase> SharedBase;
  out[1] = make_handle<SharedBase>(new SharedBase(*self));
}

void VirtualDerived_deconstructor_9(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<VirtualDerived> Shared;
  checkArguments("delete_VirtualDerived",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_VirtualDerived::iterator item;
  item = collector_VirtualDerived.find(self);
  if(item == collector_VirtualDerived.end()) {
    return;
  }
  collector_VirtualDerived.erase(item);
  delete self;
}

void wraptestNamespacedVirtualBase_collectorInsertAndMakeBase_10(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<wraptest::NamespacedVirtualBase> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_wraptestNamespacedVirtualBase.insert(self);
}

void NamespacedVirtualBase_upcastFromVoid_11(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in) {
  typedef std::shared_ptr<wraptest::NamespacedVirtualBase> Shared;
  std::shared_ptr<void> *asVoid = get_handle<std::shared_ptr<void>>(in[0]);
  Shared *self = new Shared(std::static_pointer_cast<wraptest::NamespacedVirtualBase>(*asVoid));
  collector_wraptestNamespacedVirtualBase.insert(self);
  out[0] = make_handle<Shared>(self);
}

void wraptestNamespacedVirtualBase_deconstructor_12(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<wraptest::NamespacedVirtualBase> Shared;
  checkArguments("delete_wraptestNamespacedVirtualBase",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_wraptestNamespacedVirtualBase::iterator item;
  item = collector_wraptestNamespacedVirtualBase.find(self);
  if(item == collector_wraptestNamespacedVirtualBase.end()) {
    return;
  }
  collector_wraptestNamespacedVirtualBase.erase(item);
  delete self;
}

void wraptestNamespacedVirtualDerived_collectorInsertAndMakeBase_13(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<wraptest::NamespacedVirtualDerived> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_wraptestNamespacedVirtualDerived.insert(self);

  typedef std::shared_ptr<NamespacedVirtualBase> SharedBase;
  out[0] = make_handle<SharedBase>(new SharedBase(*self));
}

void NamespacedVirtualDerived_upcastFromVoid_14(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in) {
  typedef std::shared_ptr<wraptest::NamespacedVirtualDerived> Shared;
  std::shared_ptr<void> *asVoid = get_handle<std::shared_ptr<void>>(in[0]);
  Shared *self = new Shared(std::static_pointer_cast<wraptest::NamespacedVirtualDerived>(*asVoid));
  collector_wraptestNamespacedVirtualDerived.insert(self);
  out[0] = make_handle<Shared>(self);
}

void wraptestNamespacedVirtualDerived_constructor_15(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<wraptest::NamespacedVirtualDerived> Shared;

  Shared *self = new Shared(new wraptest::NamespacedVirtualDerived());
  collector_wraptestNamespacedVirtualDerived.insert(self);
  out[0] = make_handle<Shared>(self);

  typedef std::shared_ptr<NamespacedVirtualBase> SharedBase;
  out[1] = make_handle<SharedBase>(new SharedBase(*self));
}

void wraptestNamespacedVirtualDerived_deconstructor_16(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<wraptest::NamespacedVirtualDerived> Shared;
  checkArguments("delete_wraptestNamespacedVirtualDerived",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_wraptestNamespacedVirtualDerived::iterator item;
  item = collector_wraptestNamespacedVirtualDerived.find(self);
  if(item == collector_wraptestNamespacedVirtualDerived.end()) {
    return;
  }
  collector_wraptestNamespacedVirtualDerived.erase(item);
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
      _matlab_ownership_RTTIRegister(ctx.engine);
      rttiRegistered_ = true;
    }

    gtwrap::CoutRedirect coutRedirect(ctx.engine);

    int id = unwrap<int>(inputs[0]);
    WrapIn in(inputs, 1);
    OutputList out;

    try {
      switch(id) {
        case 0:
          OwnedThing_collectorInsertAndMakeBase_0(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 1:
          OwnedThing_constructor_1(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 2:
          OwnedThing_deconstructor_2(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 3:
          VirtualBase_collectorInsertAndMakeBase_3(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 4:
          VirtualBase_upcastFromVoid_4(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 5:
          VirtualBase_deconstructor_5(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 6:
          VirtualDerived_collectorInsertAndMakeBase_6(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 7:
          VirtualDerived_upcastFromVoid_7(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 8:
          VirtualDerived_constructor_8(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 9:
          VirtualDerived_deconstructor_9(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 10:
          wraptestNamespacedVirtualBase_collectorInsertAndMakeBase_10(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 11:
          NamespacedVirtualBase_upcastFromVoid_11(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 12:
          wraptestNamespacedVirtualBase_deconstructor_12(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 13:
          wraptestNamespacedVirtualDerived_collectorInsertAndMakeBase_13(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 14:
          NamespacedVirtualDerived_upcastFromVoid_14(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 15:
          wraptestNamespacedVirtualDerived_constructor_15(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 16:
          wraptestNamespacedVirtualDerived_deconstructor_16(ctx, (int)outputs.size(), out, (int)in.size(), in);
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
