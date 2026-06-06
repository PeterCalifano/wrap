#include <wrap/matlab_cpp.h>
#include <map>

#include <gtsam/nonlinear/Values.h>
#include <path/to/ns1.h>
#include <path/to/ns1/ClassB.h>
#include <path/to/ns2.h>
#include <path/to/ns2/ClassA.h>
#include <path/to/ns3.h>



typedef std::set<std::shared_ptr<ns1::ClassA>*> Collector_ns1ClassA;
static Collector_ns1ClassA collector_ns1ClassA;
typedef std::set<std::shared_ptr<ns1::ClassB>*> Collector_ns1ClassB;
static Collector_ns1ClassB collector_ns1ClassB;
typedef std::set<std::shared_ptr<ns2::ClassA>*> Collector_ns2ClassA;
static Collector_ns2ClassA collector_ns2ClassA;
typedef std::set<std::shared_ptr<ns2::ns3::ClassB>*> Collector_ns2ns3ClassB;
static Collector_ns2ns3ClassB collector_ns2ns3ClassB;
typedef std::set<std::shared_ptr<ns2::ClassC>*> Collector_ns2ClassC;
static Collector_ns2ClassC collector_ns2ClassC;
typedef std::set<std::shared_ptr<ClassD>*> Collector_ClassD;
static Collector_ClassD collector_ClassD;
typedef std::set<std::shared_ptr<gtsam::Values>*> Collector_gtsamValues;
static Collector_gtsamValues collector_gtsamValues;


void _deleteAllObjects(matlab::engine::MATLABEngine* engine)
{
  bool anyDeleted = false;
  { for(Collector_ns1ClassA::iterator iter = collector_ns1ClassA.begin();
      iter != collector_ns1ClassA.end(); ) {
    delete *iter;
    collector_ns1ClassA.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_ns1ClassB::iterator iter = collector_ns1ClassB.begin();
      iter != collector_ns1ClassB.end(); ) {
    delete *iter;
    collector_ns1ClassB.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_ns2ClassA::iterator iter = collector_ns2ClassA.begin();
      iter != collector_ns2ClassA.end(); ) {
    delete *iter;
    collector_ns2ClassA.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_ns2ns3ClassB::iterator iter = collector_ns2ns3ClassB.begin();
      iter != collector_ns2ns3ClassB.end(); ) {
    delete *iter;
    collector_ns2ns3ClassB.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_ns2ClassC::iterator iter = collector_ns2ClassC.begin();
      iter != collector_ns2ClassC.end(); ) {
    delete *iter;
    collector_ns2ClassC.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_ClassD::iterator iter = collector_ClassD.begin();
      iter != collector_ClassD.end(); ) {
    delete *iter;
    collector_ClassD.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_gtsamValues::iterator iter = collector_gtsamValues.begin();
      iter != collector_gtsamValues.end(); ) {
    delete *iter;
    collector_gtsamValues.erase(iter++);
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

void _namespaces_RTTIRegister(matlab::engine::MATLABEngine* engine) {
  bool alreadyCreated = false;
  try {
    engine->getVariable(u"gtsam_namespaces_rttiRegistry_created",
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
    engine->setVariable(u"gtsam_namespaces_rttiRegistry_created",
                        f_.createScalar<std::int8_t>(0),
                        matlab::engine::WorkspaceType::GLOBAL);
  }
}

void ns1ClassA_collectorInsertAndMakeBase_0(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ns1::ClassA> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_ns1ClassA.insert(self);
}

void ns1ClassA_constructor_1(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ns1::ClassA> Shared;

  Shared *self = new Shared(new ns1::ClassA());
  collector_ns1ClassA.insert(self);
  out[0] = make_handle<Shared>(self);
}

void ns1ClassA_deconstructor_2(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ns1::ClassA> Shared;
  checkArguments("delete_ns1ClassA",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_ns1ClassA::iterator item;
  item = collector_ns1ClassA.find(self);
  if(item == collector_ns1ClassA.end()) {
    return;
  }
  collector_ns1ClassA.erase(item);
  delete self;
}

void ns1ClassB_collectorInsertAndMakeBase_3(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ns1::ClassB> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_ns1ClassB.insert(self);
}

void ns1ClassB_constructor_4(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ns1::ClassB> Shared;

  Shared *self = new Shared(new ns1::ClassB());
  collector_ns1ClassB.insert(self);
  out[0] = make_handle<Shared>(self);
}

void ns1ClassB_deconstructor_5(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ns1::ClassB> Shared;
  checkArguments("delete_ns1ClassB",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_ns1ClassB::iterator item;
  item = collector_ns1ClassB.find(self);
  if(item == collector_ns1ClassB.end()) {
    return;
  }
  collector_ns1ClassB.erase(item);
  delete self;
}

void aGlobalFunction_6(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("aGlobalFunction",nargout,nargin,0);
  out[0] = wrap< Vector >(ns1::aGlobalFunction());
}
void ns2ClassA_collectorInsertAndMakeBase_7(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ns2::ClassA> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_ns2ClassA.insert(self);
}

void ns2ClassA_constructor_8(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ns2::ClassA> Shared;

  Shared *self = new Shared(new ns2::ClassA());
  collector_ns2ClassA.insert(self);
  out[0] = make_handle<Shared>(self);
}

void ns2ClassA_deconstructor_9(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ns2::ClassA> Shared;
  checkArguments("delete_ns2ClassA",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_ns2ClassA::iterator item;
  item = collector_ns2ClassA.find(self);
  if(item == collector_ns2ClassA.end()) {
    return;
  }
  collector_ns2ClassA.erase(item);
  delete self;
}

void ns2ClassA_memberFunction_10(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("memberFunction",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<ns2::ClassA>(ctx, in[0], "ptr_ns2ClassA");
  out[0] = wrap< double >(obj->memberFunction());
}

void ns2ClassA_nsArg_11(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("nsArg",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<ns2::ClassA>(ctx, in[0], "ptr_ns2ClassA");
  ns1::ClassB& arg = *unwrap_shared_ptr< ns1::ClassB >(ctx, in[1], "ptr_ns1ClassB");
  out[0] = wrap< int >(obj->nsArg(arg));
}

void ns2ClassA_nsReturn_12(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("nsReturn",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<ns2::ClassA>(ctx, in[0], "ptr_ns2ClassA");
  double q = unwrap< double >(in[1]);
  out[0] = wrap_shared_ptr(ctx, std::make_shared<ns2::ns3::ClassB>(obj->nsReturn(q)),"ns2.ns3.ClassB", false);
}

void ns2ClassA_afunction_13(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("ns2::ClassA.afunction",nargout,nargin,0);
  out[0] = wrap< double >(ns2::ClassA::afunction());
}

void ns2ns3ClassB_collectorInsertAndMakeBase_14(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ns2::ns3::ClassB> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_ns2ns3ClassB.insert(self);
}

void ns2ns3ClassB_constructor_15(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ns2::ns3::ClassB> Shared;

  Shared *self = new Shared(new ns2::ns3::ClassB());
  collector_ns2ns3ClassB.insert(self);
  out[0] = make_handle<Shared>(self);
}

void ns2ns3ClassB_deconstructor_16(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ns2::ns3::ClassB> Shared;
  checkArguments("delete_ns2ns3ClassB",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_ns2ns3ClassB::iterator item;
  item = collector_ns2ns3ClassB.find(self);
  if(item == collector_ns2ns3ClassB.end()) {
    return;
  }
  collector_ns2ns3ClassB.erase(item);
  delete self;
}

void ns2ClassC_collectorInsertAndMakeBase_17(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ns2::ClassC> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_ns2ClassC.insert(self);
}

void ns2ClassC_constructor_18(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ns2::ClassC> Shared;

  Shared *self = new Shared(new ns2::ClassC());
  collector_ns2ClassC.insert(self);
  out[0] = make_handle<Shared>(self);
}

void ns2ClassC_deconstructor_19(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ns2::ClassC> Shared;
  checkArguments("delete_ns2ClassC",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_ns2ClassC::iterator item;
  item = collector_ns2ClassC.find(self);
  if(item == collector_ns2ClassC.end()) {
    return;
  }
  collector_ns2ClassC.erase(item);
  delete self;
}

void aGlobalFunction_20(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("aGlobalFunction",nargout,nargin,0);
  out[0] = wrap< Vector >(ns2::aGlobalFunction());
}
void overloadedGlobalFunction_21(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("overloadedGlobalFunction",nargout,nargin,1);
  ns1::ClassA& a = *unwrap_shared_ptr< ns1::ClassA >(ctx, in[0], "ptr_ns1ClassA");
  out[0] = wrap_shared_ptr(ctx, std::make_shared<ns1::ClassA>(ns2::overloadedGlobalFunction(a)),"ns1.ClassA", false);
}
void overloadedGlobalFunction_22(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("overloadedGlobalFunction",nargout,nargin,2);
  ns1::ClassA& a = *unwrap_shared_ptr< ns1::ClassA >(ctx, in[0], "ptr_ns1ClassA");
  double b = unwrap< double >(in[1]);
  out[0] = wrap_shared_ptr(ctx, std::make_shared<ns1::ClassA>(ns2::overloadedGlobalFunction(a,b)),"ns1.ClassA", false);
}
void ClassD_collectorInsertAndMakeBase_23(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ClassD> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_ClassD.insert(self);
}

void ClassD_constructor_24(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ClassD> Shared;

  Shared *self = new Shared(new ClassD());
  collector_ClassD.insert(self);
  out[0] = make_handle<Shared>(self);
}

void ClassD_deconstructor_25(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ClassD> Shared;
  checkArguments("delete_ClassD",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_ClassD::iterator item;
  item = collector_ClassD.find(self);
  if(item == collector_ClassD.end()) {
    return;
  }
  collector_ClassD.erase(item);
  delete self;
}

void gtsamValues_collectorInsertAndMakeBase_26(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Values> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_gtsamValues.insert(self);
}

void gtsamValues_constructor_27(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Values> Shared;

  Shared *self = new Shared(new gtsam::Values());
  collector_gtsamValues.insert(self);
  out[0] = make_handle<Shared>(self);
}

void gtsamValues_constructor_28(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Values> Shared;

  gtsam::Values& other = *unwrap_shared_ptr< gtsam::Values >(ctx, in[0], "ptr_gtsamValues");
  Shared *self = new Shared(new gtsam::Values(other));
  collector_gtsamValues.insert(self);
  out[0] = make_handle<Shared>(self);
}

void gtsamValues_deconstructor_29(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Values> Shared;
  checkArguments("delete_gtsamValues",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_gtsamValues::iterator item;
  item = collector_gtsamValues.find(self);
  if(item == collector_gtsamValues.end()) {
    return;
  }
  collector_gtsamValues.erase(item);
  delete self;
}

void gtsamValues_insert_30(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("insert",nargout,nargin-1,2);
  auto obj = unwrap_shared_ptr<gtsam::Values>(ctx, in[0], "ptr_gtsamValues");
  size_t j = unwrap< size_t >(in[1]);
  Vector vector = unwrap< Vector >(in[2]);
  obj->insert(j,vector);
}

void gtsamValues_insert_31(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("insert",nargout,nargin-1,2);
  auto obj = unwrap_shared_ptr<gtsam::Values>(ctx, in[0], "ptr_gtsamValues");
  size_t j = unwrap< size_t >(in[1]);
  Matrix matrix = unwrap< Matrix >(in[2]);
  obj->insert(j,matrix);
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
      _namespaces_RTTIRegister(ctx.engine);
      rttiRegistered_ = true;
    }

    gtwrap::CoutRedirect coutRedirect(ctx.engine);

    int id = unwrap<int>(inputs[0]);
    WrapIn in(inputs, 1);
    OutputList out;

    try {
      switch(id) {
        case 0:
          ns1ClassA_collectorInsertAndMakeBase_0(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 1:
          ns1ClassA_constructor_1(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 2:
          ns1ClassA_deconstructor_2(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 3:
          ns1ClassB_collectorInsertAndMakeBase_3(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 4:
          ns1ClassB_constructor_4(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 5:
          ns1ClassB_deconstructor_5(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 6:
          aGlobalFunction_6(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 7:
          ns2ClassA_collectorInsertAndMakeBase_7(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 8:
          ns2ClassA_constructor_8(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 9:
          ns2ClassA_deconstructor_9(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 10:
          ns2ClassA_memberFunction_10(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 11:
          ns2ClassA_nsArg_11(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 12:
          ns2ClassA_nsReturn_12(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 13:
          ns2ClassA_afunction_13(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 14:
          ns2ns3ClassB_collectorInsertAndMakeBase_14(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 15:
          ns2ns3ClassB_constructor_15(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 16:
          ns2ns3ClassB_deconstructor_16(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 17:
          ns2ClassC_collectorInsertAndMakeBase_17(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 18:
          ns2ClassC_constructor_18(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 19:
          ns2ClassC_deconstructor_19(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 20:
          aGlobalFunction_20(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 21:
          overloadedGlobalFunction_21(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 22:
          overloadedGlobalFunction_22(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 23:
          ClassD_collectorInsertAndMakeBase_23(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 24:
          ClassD_constructor_24(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 25:
          ClassD_deconstructor_25(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 26:
          gtsamValues_collectorInsertAndMakeBase_26(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 27:
          gtsamValues_constructor_27(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 28:
          gtsamValues_constructor_28(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 29:
          gtsamValues_deconstructor_29(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 30:
          gtsamValues_insert_30(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 31:
          gtsamValues_insert_31(ctx, (int)outputs.size(), out, (int)in.size(), in);
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
