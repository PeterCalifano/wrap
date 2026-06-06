#include <wrap/matlab_cpp.h>
#include <map>



typedef gtsam::Optimizer<gtsam::GaussNewtonParams> OptimizerGaussNewtonParams;

typedef std::set<std::shared_ptr<Pet>*> Collector_Pet;
static Collector_Pet collector_Pet;
typedef std::set<std::shared_ptr<gtsam::MCU>*> Collector_gtsamMCU;
static Collector_gtsamMCU collector_gtsamMCU;
typedef std::set<std::shared_ptr<OptimizerGaussNewtonParams>*> Collector_gtsamOptimizerGaussNewtonParams;
static Collector_gtsamOptimizerGaussNewtonParams collector_gtsamOptimizerGaussNewtonParams;


void _deleteAllObjects(matlab::engine::MATLABEngine* engine)
{
  bool anyDeleted = false;
  { for(Collector_Pet::iterator iter = collector_Pet.begin();
      iter != collector_Pet.end(); ) {
    delete *iter;
    collector_Pet.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_gtsamMCU::iterator iter = collector_gtsamMCU.begin();
      iter != collector_gtsamMCU.end(); ) {
    delete *iter;
    collector_gtsamMCU.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_gtsamOptimizerGaussNewtonParams::iterator iter = collector_gtsamOptimizerGaussNewtonParams.begin();
      iter != collector_gtsamOptimizerGaussNewtonParams.end(); ) {
    delete *iter;
    collector_gtsamOptimizerGaussNewtonParams.erase(iter++);
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

void _enum_RTTIRegister(matlab::engine::MATLABEngine* engine) {
  bool alreadyCreated = false;
  try {
    engine->getVariable(u"gtsam_enum_rttiRegistry_created",
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
    engine->setVariable(u"gtsam_enum_rttiRegistry_created",
                        f_.createScalar<std::int8_t>(0),
                        matlab::engine::WorkspaceType::GLOBAL);
  }
}

void Pet_collectorInsertAndMakeBase_0(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<Pet> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_Pet.insert(self);
}

void Pet_constructor_1(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<Pet> Shared;

  string name = unwrap< string >(in[0]);
  Pet::Kind type = unwrap_enum<Pet::Kind>(ctx, in[1]);
  Shared *self = new Shared(new Pet(name,type));
  collector_Pet.insert(self);
  out[0] = make_handle<Shared>(self);
}

void Pet_deconstructor_2(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<Pet> Shared;
  checkArguments("delete_Pet",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_Pet::iterator item;
  item = collector_Pet.find(self);
  if(item == collector_Pet.end()) {
    return;
  }
  collector_Pet.erase(item);
  delete self;
}

void Pet_getColor_3(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("getColor",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<Pet>(ctx, in[0], "ptr_Pet");
  out[0] = wrap_enum(ctx, obj->getColor(),"Color");
}

void Pet_setColor_4(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("setColor",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Pet>(ctx, in[0], "ptr_Pet");
  Color color = unwrap_enum<Color>(ctx, in[1]);
  obj->setColor(color);
}

void Pet_get_name_5(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("name",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<Pet>(ctx, in[0], "ptr_Pet");
  out[0] = wrap< string >(obj->name);
}

void Pet_set_name_6(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("name",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Pet>(ctx, in[0], "ptr_Pet");
  string name = unwrap< string >(in[1]);
  obj->name = name;
}

void Pet_get_type_7(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("type",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<Pet>(ctx, in[0], "ptr_Pet");
  out[0] = wrap_enum(ctx, obj->type,"Pet.Kind");
}

void Pet_set_type_8(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("type",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Pet>(ctx, in[0], "ptr_Pet");
  Pet::Kind type = unwrap_enum<Pet::Kind>(ctx, in[1]);
  obj->type = type;
}

void gtsamMCU_collectorInsertAndMakeBase_9(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::MCU> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_gtsamMCU.insert(self);
}

void gtsamMCU_constructor_10(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::MCU> Shared;

  Shared *self = new Shared(new gtsam::MCU());
  collector_gtsamMCU.insert(self);
  out[0] = make_handle<Shared>(self);
}

void gtsamMCU_deconstructor_11(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::MCU> Shared;
  checkArguments("delete_gtsamMCU",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_gtsamMCU::iterator item;
  item = collector_gtsamMCU.find(self);
  if(item == collector_gtsamMCU.end()) {
    return;
  }
  collector_gtsamMCU.erase(item);
  delete self;
}

void gtsamOptimizerGaussNewtonParams_collectorInsertAndMakeBase_12(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Optimizer<gtsam::GaussNewtonParams>> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_gtsamOptimizerGaussNewtonParams.insert(self);
}

void gtsamOptimizerGaussNewtonParams_constructor_13(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Optimizer<gtsam::GaussNewtonParams>> Shared;

  Optimizer<gtsam::GaussNewtonParams>::Verbosity verbosity = unwrap_enum<Optimizer<gtsam::GaussNewtonParams>::Verbosity>(ctx, in[0]);
  Shared *self = new Shared(new gtsam::Optimizer<gtsam::GaussNewtonParams>(verbosity));
  collector_gtsamOptimizerGaussNewtonParams.insert(self);
  out[0] = make_handle<Shared>(self);
}

void gtsamOptimizerGaussNewtonParams_deconstructor_14(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Optimizer<gtsam::GaussNewtonParams>> Shared;
  checkArguments("delete_gtsamOptimizerGaussNewtonParams",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_gtsamOptimizerGaussNewtonParams::iterator item;
  item = collector_gtsamOptimizerGaussNewtonParams.find(self);
  if(item == collector_gtsamOptimizerGaussNewtonParams.end()) {
    return;
  }
  collector_gtsamOptimizerGaussNewtonParams.erase(item);
  delete self;
}

void gtsamOptimizerGaussNewtonParams_getVerbosity_15(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("getVerbosity",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<gtsam::Optimizer<gtsam::GaussNewtonParams>>(ctx, in[0], "ptr_gtsamOptimizerGaussNewtonParams");
  out[0] = wrap_enum(ctx, obj->getVerbosity(),"gtsam.OptimizerGaussNewtonParams.Verbosity");
}

void gtsamOptimizerGaussNewtonParams_getVerbosity_16(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("getVerbosity",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<gtsam::Optimizer<gtsam::GaussNewtonParams>>(ctx, in[0], "ptr_gtsamOptimizerGaussNewtonParams");
  out[0] = wrap_enum(ctx, obj->getVerbosity(),"gtsam.VerbosityLM");
}

void gtsamOptimizerGaussNewtonParams_setVerbosity_17(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("setVerbosity",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<gtsam::Optimizer<gtsam::GaussNewtonParams>>(ctx, in[0], "ptr_gtsamOptimizerGaussNewtonParams");
  Optimizer<gtsam::GaussNewtonParams>::Verbosity value = unwrap_enum<Optimizer<gtsam::GaussNewtonParams>::Verbosity>(ctx, in[1]);
  obj->setVerbosity(value);
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
      _enum_RTTIRegister(ctx.engine);
      rttiRegistered_ = true;
    }

    gtwrap::CoutRedirect coutRedirect(ctx.engine);

    int id = unwrap<int>(inputs[0]);
    WrapIn in(inputs, 1);
    OutputList out;

    try {
      switch(id) {
        case 0:
          Pet_collectorInsertAndMakeBase_0(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 1:
          Pet_constructor_1(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 2:
          Pet_deconstructor_2(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 3:
          Pet_getColor_3(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 4:
          Pet_setColor_4(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 5:
          Pet_get_name_5(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 6:
          Pet_set_name_6(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 7:
          Pet_get_type_7(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 8:
          Pet_set_type_8(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 9:
          gtsamMCU_collectorInsertAndMakeBase_9(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 10:
          gtsamMCU_constructor_10(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 11:
          gtsamMCU_deconstructor_11(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 12:
          gtsamOptimizerGaussNewtonParams_collectorInsertAndMakeBase_12(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 13:
          gtsamOptimizerGaussNewtonParams_constructor_13(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 14:
          gtsamOptimizerGaussNewtonParams_deconstructor_14(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 15:
          gtsamOptimizerGaussNewtonParams_getVerbosity_15(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 16:
          gtsamOptimizerGaussNewtonParams_getVerbosity_16(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 17:
          gtsamOptimizerGaussNewtonParams_setVerbosity_17(ctx, (int)outputs.size(), out, (int)in.size(), in);
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
