#include <wrap/matlab_cpp.h>
#include <map>







void _deleteAllObjects(matlab::engine::MATLABEngine* engine)
{
  bool anyDeleted = false;

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

void _functions_RTTIRegister(matlab::engine::MATLABEngine* engine) {
  bool alreadyCreated = false;
  try {
    engine->getVariable(u"gtsam_functions_rttiRegistry_created",
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
    engine->setVariable(u"gtsam_functions_rttiRegistry_created",
                        f_.createScalar<std::int8_t>(0),
                        matlab::engine::WorkspaceType::GLOBAL);
  }
}

void load2D_0(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("load2D",nargout,nargin,5);
  string filename = unwrap< string >(in[0]);
  std::shared_ptr<Test> model = unwrap_shared_ptr< Test >(ctx, in[1], "ptr_Test");
  int maxID = unwrap< int >(in[2]);
  bool addNoise = unwrap< bool >(in[3]);
  bool smart = unwrap< bool >(in[4]);
  auto pairResult = load2D(filename,model,maxID,addNoise,smart);
  out[0] = wrap_shared_ptr(ctx, pairResult.first,"gtsam.NonlinearFactorGraph", false);
  out[1] = wrap_shared_ptr(ctx, pairResult.second,"gtsam.Values", false);
}
void load2D_1(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("load2D",nargout,nargin,5);
  string filename = unwrap< string >(in[0]);
  std::shared_ptr<gtsam::noiseModel::Diagonal> model = unwrap_shared_ptr< gtsam::noiseModel::Diagonal >(ctx, in[1], "ptr_gtsamnoiseModelDiagonal");
  int maxID = unwrap< int >(in[2]);
  bool addNoise = unwrap< bool >(in[3]);
  bool smart = unwrap< bool >(in[4]);
  auto pairResult = load2D(filename,model,maxID,addNoise,smart);
  out[0] = wrap_shared_ptr(ctx, pairResult.first,"gtsam.NonlinearFactorGraph", false);
  out[1] = wrap_shared_ptr(ctx, pairResult.second,"gtsam.Values", false);
}
void load2D_2(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("load2D",nargout,nargin,2);
  string filename = unwrap< string >(in[0]);
  gtsam::noiseModel::Diagonal* model = unwrap_ptr< gtsam::noiseModel::Diagonal >(ctx, in[1], "ptr_gtsamnoiseModelDiagonal");
  auto pairResult = load2D(filename,model);
  out[0] = wrap_shared_ptr(ctx, pairResult.first,"gtsam.NonlinearFactorGraph", false);
  out[1] = wrap_shared_ptr(ctx, pairResult.second,"gtsam.Values", false);
}
void aGlobalFunction_3(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("aGlobalFunction",nargout,nargin,0);
  out[0] = wrap< Vector >(aGlobalFunction());
}
void overloadedGlobalFunction_4(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("overloadedGlobalFunction",nargout,nargin,1);
  int a = unwrap< int >(in[0]);
  out[0] = wrap< Vector >(overloadedGlobalFunction(a));
}
void overloadedGlobalFunction_5(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("overloadedGlobalFunction",nargout,nargin,2);
  int a = unwrap< int >(in[0]);
  double b = unwrap< double >(in[1]);
  out[0] = wrap< Vector >(overloadedGlobalFunction(a,b));
}
void MultiTemplatedFunctionStringSize_tDouble_6(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("MultiTemplatedFunctionStringSize_tDouble",nargout,nargin,2);
  string x = unwrap< string >(in[0]);
  size_t y = unwrap< size_t >(in[1]);
  out[0] = wrap< double >(MultiTemplatedFunction<string,size_t,double>(x,y));
}
void MultiTemplatedFunctionDoubleSize_tDouble_7(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("MultiTemplatedFunctionDoubleSize_tDouble",nargout,nargin,2);
  double x = unwrap< double >(in[0]);
  size_t y = unwrap< size_t >(in[1]);
  out[0] = wrap< double >(MultiTemplatedFunction<double,size_t,double>(x,y));
}
void DefaultFuncInt_8(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("DefaultFuncInt",nargout,nargin,2);
  int a = unwrap< int >(in[0]);
  int b = unwrap< int >(in[1]);
  DefaultFuncInt(a,b);
}
void DefaultFuncInt_9(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("DefaultFuncInt",nargout,nargin,1);
  int a = unwrap< int >(in[0]);
  DefaultFuncInt(a,0);
}
void DefaultFuncInt_10(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("DefaultFuncInt",nargout,nargin,0);
  DefaultFuncInt(123,0);
}
void DefaultFuncString_11(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("DefaultFuncString",nargout,nargin,2);
  string s = unwrap< string >(in[0]);
  string name = unwrap< string >(in[1]);
  DefaultFuncString(s,name);
}
void DefaultFuncString_12(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("DefaultFuncString",nargout,nargin,1);
  string s = unwrap< string >(in[0]);
  DefaultFuncString(s,"");
}
void DefaultFuncString_13(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("DefaultFuncString",nargout,nargin,0);
  DefaultFuncString("hello","");
}
void DefaultFuncObj_14(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("DefaultFuncObj",nargout,nargin,1);
  gtsam::KeyFormatter& keyFormatter = *unwrap_shared_ptr< gtsam::KeyFormatter >(ctx, in[0], "ptr_gtsamKeyFormatter");
  DefaultFuncObj(keyFormatter);
}
void DefaultFuncObj_15(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("DefaultFuncObj",nargout,nargin,0);
  DefaultFuncObj(gtsam::DefaultKeyFormatter);
}
void DefaultFuncZero_16(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("DefaultFuncZero",nargout,nargin,5);
  int a = unwrap< int >(in[0]);
  int b = unwrap< int >(in[1]);
  double c = unwrap< double >(in[2]);
  int d = unwrap< int >(in[3]);
  bool e = unwrap< bool >(in[4]);
  DefaultFuncZero(a,b,c,d,e);
}
void DefaultFuncZero_17(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("DefaultFuncZero",nargout,nargin,4);
  int a = unwrap< int >(in[0]);
  int b = unwrap< int >(in[1]);
  double c = unwrap< double >(in[2]);
  int d = unwrap< int >(in[3]);
  DefaultFuncZero(a,b,c,d,false);
}
void DefaultFuncZero_18(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("DefaultFuncZero",nargout,nargin,3);
  int a = unwrap< int >(in[0]);
  int b = unwrap< int >(in[1]);
  double c = unwrap< double >(in[2]);
  DefaultFuncZero(a,b,c,0,false);
}
void DefaultFuncZero_19(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("DefaultFuncZero",nargout,nargin,2);
  int a = unwrap< int >(in[0]);
  int b = unwrap< int >(in[1]);
  DefaultFuncZero(a,b,0.0,0,false);
}
void DefaultFuncVector_20(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("DefaultFuncVector",nargout,nargin,2);
  std::vector<int>& i = *unwrap_shared_ptr< std::vector<int> >(ctx, in[0], "ptr_stdvectorint");
  std::vector<string>& s = *unwrap_shared_ptr< std::vector<string> >(ctx, in[1], "ptr_stdvectorstring");
  DefaultFuncVector(i,s);
}
void DefaultFuncVector_21(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("DefaultFuncVector",nargout,nargin,1);
  std::vector<int>& i = *unwrap_shared_ptr< std::vector<int> >(ctx, in[0], "ptr_stdvectorint");
  DefaultFuncVector(i,{"borglab", "gtsam"});
}
void DefaultFuncVector_22(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("DefaultFuncVector",nargout,nargin,0);
  DefaultFuncVector({1, 2, 3},{"borglab", "gtsam"});
}
void setPose_23(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("setPose",nargout,nargin,1);
  gtsam::Pose3& pose = *unwrap_shared_ptr< gtsam::Pose3 >(ctx, in[0], "ptr_gtsamPose3");
  setPose(pose);
}
void setPose_24(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("setPose",nargout,nargin,0);
  setPose(gtsam::Pose3());
}
void EliminateDiscrete_25(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("EliminateDiscrete",nargout,nargin,2);
  gtsam::DiscreteFactorGraph& factors = *unwrap_shared_ptr< gtsam::DiscreteFactorGraph >(ctx, in[0], "ptr_gtsamDiscreteFactorGraph");
  gtsam::Ordering& frontalKeys = *unwrap_shared_ptr< gtsam::Ordering >(ctx, in[1], "ptr_gtsamOrdering");
  auto pairResult = EliminateDiscrete(factors,frontalKeys);
  out[0] = wrap_shared_ptr(ctx, pairResult.first,"gtsam.DiscreteConditional", false);
  out[1] = wrap_shared_ptr(ctx, pairResult.second,"gtsam.DecisionTreeFactor", false);
}
void triangulatePoint3Cal3_S2_26(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("triangulatePoint3Cal3_S2",nargout,nargin,6);
  gtsam::Pose3Vector& poses = *unwrap_shared_ptr< gtsam::Pose3Vector >(ctx, in[0], "ptr_gtsamPose3Vector");
  std::shared_ptr<gtsam::Cal3_S2> sharedCal = unwrap_shared_ptr< gtsam::Cal3_S2 >(ctx, in[1], "ptr_gtsamCal3_S2");
  gtsam::Point2Vector& measurements = *unwrap_shared_ptr< gtsam::Point2Vector >(ctx, in[2], "ptr_gtsamPoint2Vector");
  double rank_tol = unwrap< double >(in[3]);
  bool optimize = unwrap< bool >(in[4]);
  gtsam::SharedNoiseModel& model = *unwrap_shared_ptr< gtsam::SharedNoiseModel >(ctx, in[5], "ptr_gtsamSharedNoiseModel");
  out[0] = wrap< Point3 >(triangulatePoint3<gtsam::Cal3_S2>(poses,sharedCal,measurements,rank_tol,optimize,model));
}
void triangulatePoint3Cal3_S2_27(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("triangulatePoint3Cal3_S2",nargout,nargin,5);
  gtsam::Pose3Vector& poses = *unwrap_shared_ptr< gtsam::Pose3Vector >(ctx, in[0], "ptr_gtsamPose3Vector");
  std::shared_ptr<gtsam::Cal3_S2> sharedCal = unwrap_shared_ptr< gtsam::Cal3_S2 >(ctx, in[1], "ptr_gtsamCal3_S2");
  gtsam::Point2Vector& measurements = *unwrap_shared_ptr< gtsam::Point2Vector >(ctx, in[2], "ptr_gtsamPoint2Vector");
  double rank_tol = unwrap< double >(in[3]);
  bool optimize = unwrap< bool >(in[4]);
  out[0] = wrap< Point3 >(triangulatePoint3<gtsam::Cal3_S2>(poses,sharedCal,measurements,rank_tol,optimize,nullptr));
}
void FindKarcherMeanPoint3_28(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("FindKarcherMeanPoint3",nargout,nargin,1);
  std::vector<gtsam::Point3>& elements = *unwrap_shared_ptr< std::vector<gtsam::Point3> >(ctx, in[0], "ptr_stdvectorgtsam::Point3");
  out[0] = wrap< Point3 >(FindKarcherMean<gtsam::Point3>(elements));
}
void FindKarcherMeanSO3_29(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("FindKarcherMeanSO3",nargout,nargin,1);
  std::vector<gtsam::SO3>& elements = *unwrap_shared_ptr< std::vector<gtsam::SO3> >(ctx, in[0], "ptr_stdvectorgtsam::SO3");
  out[0] = wrap_shared_ptr(ctx, std::make_shared<gtsam::SO3>(FindKarcherMean<gtsam::SO3>(elements)),"gtsam.SO3", false);
}
void FindKarcherMeanSO4_30(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("FindKarcherMeanSO4",nargout,nargin,1);
  std::vector<gtsam::SO4>& elements = *unwrap_shared_ptr< std::vector<gtsam::SO4> >(ctx, in[0], "ptr_stdvectorgtsam::SO4");
  out[0] = wrap_shared_ptr(ctx, std::make_shared<gtsam::SO4>(FindKarcherMean<gtsam::SO4>(elements)),"gtsam.SO4", false);
}
void FindKarcherMeanPose3_31(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("FindKarcherMeanPose3",nargout,nargin,1);
  std::vector<gtsam::Pose3>& elements = *unwrap_shared_ptr< std::vector<gtsam::Pose3> >(ctx, in[0], "ptr_stdvectorgtsam::Pose3");
  out[0] = wrap_shared_ptr(ctx, std::make_shared<gtsam::Pose3>(FindKarcherMean<gtsam::Pose3>(elements)),"gtsam.Pose3", false);
}
void TemplatedFunctionRot3_32(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("TemplatedFunctionRot3",nargout,nargin,1);
  gtsam::Rot3& t = *unwrap_shared_ptr< gtsam::Rot3 >(ctx, in[0], "ptr_gtsamRot3");
  TemplatedFunction<gtsam::Rot3>(t);
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
      _functions_RTTIRegister(ctx.engine);
      rttiRegistered_ = true;
    }

    gtwrap::CoutRedirect coutRedirect(ctx.engine);

    int id = unwrap<int>(inputs[0]);
    WrapIn in(inputs, 1);
    OutputList out;

    try {
      switch(id) {
        case 0:
          load2D_0(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 1:
          load2D_1(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 2:
          load2D_2(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 3:
          aGlobalFunction_3(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 4:
          overloadedGlobalFunction_4(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 5:
          overloadedGlobalFunction_5(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 6:
          MultiTemplatedFunctionStringSize_tDouble_6(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 7:
          MultiTemplatedFunctionDoubleSize_tDouble_7(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 8:
          DefaultFuncInt_8(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 9:
          DefaultFuncInt_9(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 10:
          DefaultFuncInt_10(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 11:
          DefaultFuncString_11(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 12:
          DefaultFuncString_12(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 13:
          DefaultFuncString_13(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 14:
          DefaultFuncObj_14(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 15:
          DefaultFuncObj_15(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 16:
          DefaultFuncZero_16(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 17:
          DefaultFuncZero_17(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 18:
          DefaultFuncZero_18(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 19:
          DefaultFuncZero_19(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 20:
          DefaultFuncVector_20(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 21:
          DefaultFuncVector_21(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 22:
          DefaultFuncVector_22(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 23:
          setPose_23(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 24:
          setPose_24(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 25:
          EliminateDiscrete_25(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 26:
          triangulatePoint3Cal3_S2_26(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 27:
          triangulatePoint3Cal3_S2_27(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 28:
          FindKarcherMeanPoint3_28(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 29:
          FindKarcherMeanSO3_29(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 30:
          FindKarcherMeanSO4_30(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 31:
          FindKarcherMeanPose3_31(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 32:
          TemplatedFunctionRot3_32(ctx, (int)outputs.size(), out, (int)in.size(), in);
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
