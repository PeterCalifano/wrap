#include <wrap/matlab_cpp.h>
#include <map>





typedef std::set<std::shared_ptr<scalar_contracts::ScalarContract>*> Collector_scalar_contractsScalarContract;
static Collector_scalar_contractsScalarContract collector_scalar_contractsScalarContract;


void _deleteAllObjects(matlab::engine::MATLABEngine* engine)
{
  bool anyDeleted = false;
  { for(Collector_scalar_contractsScalarContract::iterator iter = collector_scalar_contractsScalarContract.begin();
      iter != collector_scalar_contractsScalarContract.end(); ) {
    delete *iter;
    collector_scalar_contractsScalarContract.erase(iter++);
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

void _matlab_scalar_contracts_RTTIRegister(matlab::engine::MATLABEngine* engine) {
  bool alreadyCreated = false;
  try {
    engine->getVariable(u"gtsam_matlab_scalar_contracts_rttiRegistry_created",
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
    engine->setVariable(u"gtsam_matlab_scalar_contracts_rttiRegistry_created",
                        f_.createScalar<std::int8_t>(0),
                        matlab::engine::WorkspaceType::GLOBAL);
  }
}

void scalar_contractsScalarContract_collectorInsertAndMakeBase_0(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<scalar_contracts::ScalarContract> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_scalar_contractsScalarContract.insert(self);
}

void scalar_contractsScalarContract_constructor_1(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<scalar_contracts::ScalarContract> Shared;

  Shared *self = new Shared(new scalar_contracts::ScalarContract());
  collector_scalar_contractsScalarContract.insert(self);
  out[0] = make_handle<Shared>(self);
}

void scalar_contractsScalarContract_deconstructor_2(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<scalar_contracts::ScalarContract> Shared;
  checkArguments("delete_scalar_contractsScalarContract",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_scalar_contractsScalarContract::iterator item;
  item = collector_scalar_contractsScalarContract.find(self);
  if(item == collector_scalar_contractsScalarContract.end()) {
    return;
  }
  collector_scalar_contractsScalarContract.erase(item);
  delete self;
}

void scalar_contractsScalarContract_echoInt16_3(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("echoInt16",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<scalar_contracts::ScalarContract>(ctx, in[0], "ptr_scalar_contractsScalarContract");
  int16_t value = unwrap< int16_t >(in[1]);
  out[0] = wrap< int16_t >(obj->echoInt16(value));
}

void scalar_contractsScalarContract_echoInt32_4(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("echoInt32",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<scalar_contracts::ScalarContract>(ctx, in[0], "ptr_scalar_contractsScalarContract");
  int32_t value = unwrap< int32_t >(in[1]);
  out[0] = wrap< int32_t >(obj->echoInt32(value));
}

void scalar_contractsScalarContract_echoInt64_5(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("echoInt64",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<scalar_contracts::ScalarContract>(ctx, in[0], "ptr_scalar_contractsScalarContract");
  int64_t value = unwrap< int64_t >(in[1]);
  out[0] = wrap< int64_t >(obj->echoInt64(value));
}

void scalar_contractsScalarContract_echoInt8_6(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("echoInt8",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<scalar_contracts::ScalarContract>(ctx, in[0], "ptr_scalar_contractsScalarContract");
  int8_t value = unwrap< int8_t >(in[1]);
  out[0] = wrap< int8_t >(obj->echoInt8(value));
}

void scalar_contractsScalarContract_echoStdUint32_7(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("echoStdUint32",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<scalar_contracts::ScalarContract>(ctx, in[0], "ptr_scalar_contractsScalarContract");
  std::uint32_t value = unwrap< std::uint32_t >(in[1]);
  out[0] = wrap< std::uint32_t >(obj->echoStdUint32(value));
}

void scalar_contractsScalarContract_echoUint16_8(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("echoUint16",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<scalar_contracts::ScalarContract>(ctx, in[0], "ptr_scalar_contractsScalarContract");
  uint16_t value = unwrap< uint16_t >(in[1]);
  out[0] = wrap< uint16_t >(obj->echoUint16(value));
}

void scalar_contractsScalarContract_echoUint32_9(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("echoUint32",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<scalar_contracts::ScalarContract>(ctx, in[0], "ptr_scalar_contractsScalarContract");
  uint32_t value = unwrap< uint32_t >(in[1]);
  out[0] = wrap< uint32_t >(obj->echoUint32(value));
}

void scalar_contractsScalarContract_echoUint64_10(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("echoUint64",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<scalar_contracts::ScalarContract>(ctx, in[0], "ptr_scalar_contractsScalarContract");
  uint64_t value = unwrap< uint64_t >(in[1]);
  out[0] = wrap< uint64_t >(obj->echoUint64(value));
}

void scalar_contractsScalarContract_echoUint8_11(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("echoUint8",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<scalar_contracts::ScalarContract>(ctx, in[0], "ptr_scalar_contractsScalarContract");
  uint8_t value = unwrap< uint8_t >(in[1]);
  out[0] = wrap< uint8_t >(obj->echoUint8(value));
}

void scalar_contractsScalarContract_getStringValue_12(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("getStringValue",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<scalar_contracts::ScalarContract>(ctx, in[0], "ptr_scalar_contractsScalarContract");
  out[0] = wrap< string >(obj->getStringValue());
}

void scalar_contractsScalarContract_setStringConstRef_13(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("setStringConstRef",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<scalar_contracts::ScalarContract>(ctx, in[0], "ptr_scalar_contractsScalarContract");
  string value = unwrap< string >(in[1]);
  obj->setStringConstRef(value);
}

void scalar_contractsScalarContract_setStringValue_14(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("setStringValue",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<scalar_contracts::ScalarContract>(ctx, in[0], "ptr_scalar_contractsScalarContract");
  string value = unwrap< string >(in[1]);
  obj->setStringValue(value);
}

void GlobalUint32_15(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("GlobalUint32",nargout,nargin,1);
  uint32_t value = unwrap< uint32_t >(in[0]);
  out[0] = wrap< uint32_t >(scalar_contracts::GlobalUint32(value));
}
void GlobalStringConstRef_16(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("GlobalStringConstRef",nargout,nargin,1);
  string value = unwrap< string >(in[0]);
  scalar_contracts::GlobalStringConstRef(value);
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
      _matlab_scalar_contracts_RTTIRegister(ctx.engine);
      rttiRegistered_ = true;
    }

    gtwrap::CoutRedirect coutRedirect(ctx.engine);

    int id = unwrap<int>(inputs[0]);
    WrapIn in(inputs, 1);
    OutputList out;

    try {
      switch(id) {
        case 0:
          scalar_contractsScalarContract_collectorInsertAndMakeBase_0(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 1:
          scalar_contractsScalarContract_constructor_1(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 2:
          scalar_contractsScalarContract_deconstructor_2(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 3:
          scalar_contractsScalarContract_echoInt16_3(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 4:
          scalar_contractsScalarContract_echoInt32_4(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 5:
          scalar_contractsScalarContract_echoInt64_5(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 6:
          scalar_contractsScalarContract_echoInt8_6(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 7:
          scalar_contractsScalarContract_echoStdUint32_7(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 8:
          scalar_contractsScalarContract_echoUint16_8(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 9:
          scalar_contractsScalarContract_echoUint32_9(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 10:
          scalar_contractsScalarContract_echoUint64_10(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 11:
          scalar_contractsScalarContract_echoUint8_11(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 12:
          scalar_contractsScalarContract_getStringValue_12(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 13:
          scalar_contractsScalarContract_setStringConstRef_13(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 14:
          scalar_contractsScalarContract_setStringValue_14(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 15:
          GlobalUint32_15(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 16:
          GlobalStringConstRef_16(ctx, (int)outputs.size(), out, (int)in.size(), in);
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
