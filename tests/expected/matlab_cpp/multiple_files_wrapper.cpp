#include <wrap/matlab_cpp.h>
#include <map>





typedef std::set<std::shared_ptr<gtsam::Class1>*> Collector_gtsamClass1;
static Collector_gtsamClass1 collector_gtsamClass1;
typedef std::set<std::shared_ptr<gtsam::Class2>*> Collector_gtsamClass2;
static Collector_gtsamClass2 collector_gtsamClass2;
typedef std::set<std::shared_ptr<gtsam::ClassA>*> Collector_gtsamClassA;
static Collector_gtsamClassA collector_gtsamClassA;


void _deleteAllObjects(matlab::engine::MATLABEngine* engine)
{
  bool anyDeleted = false;
  { for(Collector_gtsamClass1::iterator iter = collector_gtsamClass1.begin();
      iter != collector_gtsamClass1.end(); ) {
    delete *iter;
    collector_gtsamClass1.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_gtsamClass2::iterator iter = collector_gtsamClass2.begin();
      iter != collector_gtsamClass2.end(); ) {
    delete *iter;
    collector_gtsamClass2.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_gtsamClassA::iterator iter = collector_gtsamClassA.begin();
      iter != collector_gtsamClassA.end(); ) {
    delete *iter;
    collector_gtsamClassA.erase(iter++);
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

void _multiple_files_RTTIRegister(matlab::engine::MATLABEngine* engine) {
  bool alreadyCreated = false;
  try {
    engine->getVariable(u"gtsam_multiple_files_rttiRegistry_created",
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
    engine->setVariable(u"gtsam_multiple_files_rttiRegistry_created",
                        f_.createScalar<std::int8_t>(0),
                        matlab::engine::WorkspaceType::GLOBAL);
  }
}

void gtsamClass1_collectorInsertAndMakeBase_0(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Class1> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_gtsamClass1.insert(self);
}

void gtsamClass1_constructor_1(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Class1> Shared;

  Shared *self = new Shared(new gtsam::Class1());
  collector_gtsamClass1.insert(self);
  out[0] = make_handle<Shared>(self);
}

void gtsamClass1_deconstructor_2(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Class1> Shared;
  checkArguments("delete_gtsamClass1",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_gtsamClass1::iterator item;
  item = collector_gtsamClass1.find(self);
  if(item == collector_gtsamClass1.end()) {
    return;
  }
  collector_gtsamClass1.erase(item);
  delete self;
}

void gtsamClass2_collectorInsertAndMakeBase_3(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Class2> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_gtsamClass2.insert(self);
}

void gtsamClass2_constructor_4(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Class2> Shared;

  Shared *self = new Shared(new gtsam::Class2());
  collector_gtsamClass2.insert(self);
  out[0] = make_handle<Shared>(self);
}

void gtsamClass2_deconstructor_5(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Class2> Shared;
  checkArguments("delete_gtsamClass2",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_gtsamClass2::iterator item;
  item = collector_gtsamClass2.find(self);
  if(item == collector_gtsamClass2.end()) {
    return;
  }
  collector_gtsamClass2.erase(item);
  delete self;
}

void gtsamClassA_collectorInsertAndMakeBase_6(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::ClassA> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_gtsamClassA.insert(self);
}

void gtsamClassA_constructor_7(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::ClassA> Shared;

  Shared *self = new Shared(new gtsam::ClassA());
  collector_gtsamClassA.insert(self);
  out[0] = make_handle<Shared>(self);
}

void gtsamClassA_deconstructor_8(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::ClassA> Shared;
  checkArguments("delete_gtsamClassA",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_gtsamClassA::iterator item;
  item = collector_gtsamClassA.find(self);
  if(item == collector_gtsamClassA.end()) {
    return;
  }
  collector_gtsamClassA.erase(item);
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
      _multiple_files_RTTIRegister(ctx.engine);
      rttiRegistered_ = true;
    }

    gtwrap::CoutRedirect coutRedirect(ctx.engine);

    int id = unwrap<int>(inputs[0]);
    WrapIn in(inputs, 1);
    OutputList out;

    try {
      switch(id) {
        case 0:
          gtsamClass1_collectorInsertAndMakeBase_0(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 1:
          gtsamClass1_constructor_1(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 2:
          gtsamClass1_deconstructor_2(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 3:
          gtsamClass2_collectorInsertAndMakeBase_3(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 4:
          gtsamClass2_constructor_4(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 5:
          gtsamClass2_deconstructor_5(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 6:
          gtsamClassA_collectorInsertAndMakeBase_6(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 7:
          gtsamClassA_constructor_7(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 8:
          gtsamClassA_deconstructor_8(ctx, (int)outputs.size(), out, (int)in.size(), in);
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
