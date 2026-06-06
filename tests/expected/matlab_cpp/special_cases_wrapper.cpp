#include <wrap/matlab_cpp.h>
#include <map>

#include <gtsam/geometry/Cal3Bundler.h>

typedef gtsam::PinholeCamera<gtsam::Cal3Bundler> PinholeCameraCal3Bundler;
typedef gtsam::GeneralSFMFactor<gtsam::PinholeCamera<gtsam::Cal3Bundler>, gtsam::Point3> GeneralSFMFactorCal3Bundler;

typedef std::set<std::shared_ptr<gtsam::NonlinearFactorGraph>*> Collector_gtsamNonlinearFactorGraph;
static Collector_gtsamNonlinearFactorGraph collector_gtsamNonlinearFactorGraph;
typedef std::set<std::shared_ptr<gtsam::SfmTrack>*> Collector_gtsamSfmTrack;
static Collector_gtsamSfmTrack collector_gtsamSfmTrack;
typedef std::set<std::shared_ptr<PinholeCameraCal3Bundler>*> Collector_gtsamPinholeCameraCal3Bundler;
static Collector_gtsamPinholeCameraCal3Bundler collector_gtsamPinholeCameraCal3Bundler;
typedef std::set<std::shared_ptr<GeneralSFMFactorCal3Bundler>*> Collector_gtsamGeneralSFMFactorCal3Bundler;
static Collector_gtsamGeneralSFMFactorCal3Bundler collector_gtsamGeneralSFMFactorCal3Bundler;


void _deleteAllObjects(matlab::engine::MATLABEngine* engine)
{
  bool anyDeleted = false;
  { for(Collector_gtsamNonlinearFactorGraph::iterator iter = collector_gtsamNonlinearFactorGraph.begin();
      iter != collector_gtsamNonlinearFactorGraph.end(); ) {
    delete *iter;
    collector_gtsamNonlinearFactorGraph.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_gtsamSfmTrack::iterator iter = collector_gtsamSfmTrack.begin();
      iter != collector_gtsamSfmTrack.end(); ) {
    delete *iter;
    collector_gtsamSfmTrack.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_gtsamPinholeCameraCal3Bundler::iterator iter = collector_gtsamPinholeCameraCal3Bundler.begin();
      iter != collector_gtsamPinholeCameraCal3Bundler.end(); ) {
    delete *iter;
    collector_gtsamPinholeCameraCal3Bundler.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_gtsamGeneralSFMFactorCal3Bundler::iterator iter = collector_gtsamGeneralSFMFactorCal3Bundler.begin();
      iter != collector_gtsamGeneralSFMFactorCal3Bundler.end(); ) {
    delete *iter;
    collector_gtsamGeneralSFMFactorCal3Bundler.erase(iter++);
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

void _special_cases_RTTIRegister(matlab::engine::MATLABEngine* engine) {
  bool alreadyCreated = false;
  try {
    engine->getVariable(u"gtsam_special_cases_rttiRegistry_created",
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
    engine->setVariable(u"gtsam_special_cases_rttiRegistry_created",
                        f_.createScalar<std::int8_t>(0),
                        matlab::engine::WorkspaceType::GLOBAL);
  }
}

void gtsamNonlinearFactorGraph_collectorInsertAndMakeBase_0(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::NonlinearFactorGraph> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_gtsamNonlinearFactorGraph.insert(self);
}

void gtsamNonlinearFactorGraph_deconstructor_1(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::NonlinearFactorGraph> Shared;
  checkArguments("delete_gtsamNonlinearFactorGraph",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_gtsamNonlinearFactorGraph::iterator item;
  item = collector_gtsamNonlinearFactorGraph.find(self);
  if(item == collector_gtsamNonlinearFactorGraph.end()) {
    return;
  }
  collector_gtsamNonlinearFactorGraph.erase(item);
  delete self;
}

void gtsamNonlinearFactorGraph_addPrior_2(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("addPriorPinholeCameraCal3Bundler",nargout,nargin-1,3);
  auto obj = unwrap_shared_ptr<gtsam::NonlinearFactorGraph>(ctx, in[0], "ptr_gtsamNonlinearFactorGraph");
  size_t key = unwrap< size_t >(in[1]);
  gtsam::PinholeCamera<gtsam::Cal3Bundler>& prior = *unwrap_shared_ptr< gtsam::PinholeCamera<gtsam::Cal3Bundler> >(ctx, in[2], "ptr_gtsamPinholeCameraCal3Bundler");
  std::shared_ptr<gtsam::noiseModel::Base> noiseModel = unwrap_shared_ptr< gtsam::noiseModel::Base >(ctx, in[3], "ptr_gtsamnoiseModelBase");
  obj->addPrior<gtsam::PinholeCamera<gtsam::Cal3Bundler>>(key,prior,noiseModel);
}

void gtsamSfmTrack_collectorInsertAndMakeBase_3(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::SfmTrack> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_gtsamSfmTrack.insert(self);
}

void gtsamSfmTrack_deconstructor_4(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::SfmTrack> Shared;
  checkArguments("delete_gtsamSfmTrack",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_gtsamSfmTrack::iterator item;
  item = collector_gtsamSfmTrack.find(self);
  if(item == collector_gtsamSfmTrack.end()) {
    return;
  }
  collector_gtsamSfmTrack.erase(item);
  delete self;
}

void gtsamSfmTrack_get_measurements_5(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("measurements",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<gtsam::SfmTrack>(ctx, in[0], "ptr_gtsamSfmTrack");
  out[0] = wrap_shared_ptr(ctx, std::make_shared<std::vector<std::pair<size_t,Point2>>>(obj->measurements),"std.vectorpairsize_tPoint2", false);
}

void gtsamSfmTrack_set_measurements_6(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("measurements",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<gtsam::SfmTrack>(ctx, in[0], "ptr_gtsamSfmTrack");
  std::shared_ptr<std::vector<std::pair<size_t,Point2>>> measurements = unwrap_shared_ptr< std::vector<std::pair<size_t,Point2>> >(ctx, in[1], "ptr_stdvectorpairsize_tPoint2");
  obj->measurements = *measurements;
}

void gtsamPinholeCameraCal3Bundler_collectorInsertAndMakeBase_7(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::PinholeCamera<gtsam::Cal3Bundler>> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_gtsamPinholeCameraCal3Bundler.insert(self);
}

void gtsamPinholeCameraCal3Bundler_deconstructor_8(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::PinholeCamera<gtsam::Cal3Bundler>> Shared;
  checkArguments("delete_gtsamPinholeCameraCal3Bundler",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_gtsamPinholeCameraCal3Bundler::iterator item;
  item = collector_gtsamPinholeCameraCal3Bundler.find(self);
  if(item == collector_gtsamPinholeCameraCal3Bundler.end()) {
    return;
  }
  collector_gtsamPinholeCameraCal3Bundler.erase(item);
  delete self;
}

void gtsamGeneralSFMFactorCal3Bundler_collectorInsertAndMakeBase_9(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::GeneralSFMFactor<gtsam::PinholeCamera<gtsam::Cal3Bundler>, gtsam::Point3>> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_gtsamGeneralSFMFactorCal3Bundler.insert(self);
}

void gtsamGeneralSFMFactorCal3Bundler_deconstructor_10(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::GeneralSFMFactor<gtsam::PinholeCamera<gtsam::Cal3Bundler>, gtsam::Point3>> Shared;
  checkArguments("delete_gtsamGeneralSFMFactorCal3Bundler",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_gtsamGeneralSFMFactorCal3Bundler::iterator item;
  item = collector_gtsamGeneralSFMFactorCal3Bundler.find(self);
  if(item == collector_gtsamGeneralSFMFactorCal3Bundler.end()) {
    return;
  }
  collector_gtsamGeneralSFMFactorCal3Bundler.erase(item);
  delete self;
}

void gtsamGeneralSFMFactorCal3Bundler_get_verbosity_11(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("verbosity",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<gtsam::GeneralSFMFactor<gtsam::PinholeCamera<gtsam::Cal3Bundler>, gtsam::Point3>>(ctx, in[0], "ptr_gtsamGeneralSFMFactorCal3Bundler");
  out[0] = wrap_enum(ctx, obj->verbosity,"gtsam.GeneralSFMFactorCal3Bundler.Verbosity");
}

void gtsamGeneralSFMFactorCal3Bundler_set_verbosity_12(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("verbosity",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<gtsam::GeneralSFMFactor<gtsam::PinholeCamera<gtsam::Cal3Bundler>, gtsam::Point3>>(ctx, in[0], "ptr_gtsamGeneralSFMFactorCal3Bundler");
  gtsam::GeneralSFMFactor<gtsam::PinholeCamera<gtsam::Cal3Bundler>, gtsam::Point3>::Verbosity verbosity = unwrap_enum<gtsam::GeneralSFMFactor<gtsam::PinholeCamera<gtsam::Cal3Bundler>, gtsam::Point3>::Verbosity>(ctx, in[1]);
  obj->verbosity = verbosity;
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
      _special_cases_RTTIRegister(ctx.engine);
      rttiRegistered_ = true;
    }

    gtwrap::CoutRedirect coutRedirect(ctx.engine);

    int id = unwrap<int>(inputs[0]);
    WrapIn in(inputs, 1);
    OutputList out;

    try {
      switch(id) {
        case 0:
          gtsamNonlinearFactorGraph_collectorInsertAndMakeBase_0(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 1:
          gtsamNonlinearFactorGraph_deconstructor_1(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 2:
          gtsamNonlinearFactorGraph_addPrior_2(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 3:
          gtsamSfmTrack_collectorInsertAndMakeBase_3(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 4:
          gtsamSfmTrack_deconstructor_4(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 5:
          gtsamSfmTrack_get_measurements_5(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 6:
          gtsamSfmTrack_set_measurements_6(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 7:
          gtsamPinholeCameraCal3Bundler_collectorInsertAndMakeBase_7(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 8:
          gtsamPinholeCameraCal3Bundler_deconstructor_8(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 9:
          gtsamGeneralSFMFactorCal3Bundler_collectorInsertAndMakeBase_9(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 10:
          gtsamGeneralSFMFactorCal3Bundler_deconstructor_10(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 11:
          gtsamGeneralSFMFactorCal3Bundler_get_verbosity_11(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 12:
          gtsamGeneralSFMFactorCal3Bundler_set_verbosity_12(ctx, (int)outputs.size(), out, (int)in.size(), in);
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
