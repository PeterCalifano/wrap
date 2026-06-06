#include <wrap/matlab_cpp.h>
#include <map>

#include <folder/path/to/Test.h>

typedef Fun<double> FunDouble;
typedef PrimitiveRef<double> PrimitiveRefDouble;
typedef MyVector<3> MyVector3;
typedef MyVector<12> MyVector12;
typedef MultipleTemplates<int, double> MultipleTemplatesIntDouble;
typedef MultipleTemplates<int, float> MultipleTemplatesIntFloat;
typedef SmartProjectionRigFactor<gtsam::PinholeCamera<gtsam::Cal3_S2>> SmartProjectionRigFactorPinholeCameraCal3_S2;
typedef MyFactor<gtsam::Pose2, gtsam::Matrix> MyFactorPosePoint2;

typedef std::set<std::shared_ptr<FunRange>*> Collector_FunRange;
static Collector_FunRange collector_FunRange;
typedef std::set<std::shared_ptr<FunDouble>*> Collector_FunDouble;
static Collector_FunDouble collector_FunDouble;
typedef std::set<std::shared_ptr<Test>*> Collector_Test;
static Collector_Test collector_Test;
typedef std::set<std::shared_ptr<PrimitiveRefDouble>*> Collector_PrimitiveRefDouble;
static Collector_PrimitiveRefDouble collector_PrimitiveRefDouble;
typedef std::set<std::shared_ptr<MyVector3>*> Collector_MyVector3;
static Collector_MyVector3 collector_MyVector3;
typedef std::set<std::shared_ptr<MyVector12>*> Collector_MyVector12;
static Collector_MyVector12 collector_MyVector12;
typedef std::set<std::shared_ptr<MultipleTemplatesIntDouble>*> Collector_MultipleTemplatesIntDouble;
static Collector_MultipleTemplatesIntDouble collector_MultipleTemplatesIntDouble;
typedef std::set<std::shared_ptr<MultipleTemplatesIntFloat>*> Collector_MultipleTemplatesIntFloat;
static Collector_MultipleTemplatesIntFloat collector_MultipleTemplatesIntFloat;
typedef std::set<std::shared_ptr<ForwardKinematics>*> Collector_ForwardKinematics;
static Collector_ForwardKinematics collector_ForwardKinematics;
typedef std::set<std::shared_ptr<TemplatedConstructor>*> Collector_TemplatedConstructor;
static Collector_TemplatedConstructor collector_TemplatedConstructor;
typedef std::set<std::shared_ptr<FastSet>*> Collector_FastSet;
static Collector_FastSet collector_FastSet;
typedef std::set<std::shared_ptr<HessianFactor>*> Collector_HessianFactor;
static Collector_HessianFactor collector_HessianFactor;
typedef std::set<std::shared_ptr<SmartProjectionRigFactorPinholeCameraCal3_S2>*> Collector_SmartProjectionRigFactorPinholeCameraCal3_S2;
static Collector_SmartProjectionRigFactorPinholeCameraCal3_S2 collector_SmartProjectionRigFactorPinholeCameraCal3_S2;
typedef std::set<std::shared_ptr<MyFactorPosePoint2>*> Collector_MyFactorPosePoint2;
static Collector_MyFactorPosePoint2 collector_MyFactorPosePoint2;


void _deleteAllObjects(matlab::engine::MATLABEngine* engine)
{
  bool anyDeleted = false;
  { for(Collector_FunRange::iterator iter = collector_FunRange.begin();
      iter != collector_FunRange.end(); ) {
    delete *iter;
    collector_FunRange.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_FunDouble::iterator iter = collector_FunDouble.begin();
      iter != collector_FunDouble.end(); ) {
    delete *iter;
    collector_FunDouble.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_Test::iterator iter = collector_Test.begin();
      iter != collector_Test.end(); ) {
    delete *iter;
    collector_Test.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_PrimitiveRefDouble::iterator iter = collector_PrimitiveRefDouble.begin();
      iter != collector_PrimitiveRefDouble.end(); ) {
    delete *iter;
    collector_PrimitiveRefDouble.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_MyVector3::iterator iter = collector_MyVector3.begin();
      iter != collector_MyVector3.end(); ) {
    delete *iter;
    collector_MyVector3.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_MyVector12::iterator iter = collector_MyVector12.begin();
      iter != collector_MyVector12.end(); ) {
    delete *iter;
    collector_MyVector12.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_MultipleTemplatesIntDouble::iterator iter = collector_MultipleTemplatesIntDouble.begin();
      iter != collector_MultipleTemplatesIntDouble.end(); ) {
    delete *iter;
    collector_MultipleTemplatesIntDouble.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_MultipleTemplatesIntFloat::iterator iter = collector_MultipleTemplatesIntFloat.begin();
      iter != collector_MultipleTemplatesIntFloat.end(); ) {
    delete *iter;
    collector_MultipleTemplatesIntFloat.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_ForwardKinematics::iterator iter = collector_ForwardKinematics.begin();
      iter != collector_ForwardKinematics.end(); ) {
    delete *iter;
    collector_ForwardKinematics.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_TemplatedConstructor::iterator iter = collector_TemplatedConstructor.begin();
      iter != collector_TemplatedConstructor.end(); ) {
    delete *iter;
    collector_TemplatedConstructor.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_FastSet::iterator iter = collector_FastSet.begin();
      iter != collector_FastSet.end(); ) {
    delete *iter;
    collector_FastSet.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_HessianFactor::iterator iter = collector_HessianFactor.begin();
      iter != collector_HessianFactor.end(); ) {
    delete *iter;
    collector_HessianFactor.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_SmartProjectionRigFactorPinholeCameraCal3_S2::iterator iter = collector_SmartProjectionRigFactorPinholeCameraCal3_S2.begin();
      iter != collector_SmartProjectionRigFactorPinholeCameraCal3_S2.end(); ) {
    delete *iter;
    collector_SmartProjectionRigFactorPinholeCameraCal3_S2.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_MyFactorPosePoint2::iterator iter = collector_MyFactorPosePoint2.begin();
      iter != collector_MyFactorPosePoint2.end(); ) {
    delete *iter;
    collector_MyFactorPosePoint2.erase(iter++);
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

void _class_RTTIRegister(matlab::engine::MATLABEngine* engine) {
  bool alreadyCreated = false;
  try {
    engine->getVariable(u"gtsam_class_rttiRegistry_created",
                        matlab::engine::WorkspaceType::GLOBAL);
    alreadyCreated = true;
  } catch(const std::exception&) {
    alreadyCreated = false;
  }

  if(!alreadyCreated) {
    std::map<std::string, std::string> types;
    types.insert(std::make_pair(typeid(HessianFactor).name(), "HessianFactor"));

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
    engine->setVariable(u"gtsam_class_rttiRegistry_created",
                        f_.createScalar<std::int8_t>(0),
                        matlab::engine::WorkspaceType::GLOBAL);
  }
}

void FunRange_collectorInsertAndMakeBase_0(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<FunRange> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_FunRange.insert(self);
}

void FunRange_constructor_1(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<FunRange> Shared;

  Shared *self = new Shared(new FunRange());
  collector_FunRange.insert(self);
  out[0] = make_handle<Shared>(self);
}

void FunRange_deconstructor_2(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<FunRange> Shared;
  checkArguments("delete_FunRange",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_FunRange::iterator item;
  item = collector_FunRange.find(self);
  if(item == collector_FunRange.end()) {
    return;
  }
  collector_FunRange.erase(item);
  delete self;
}

void FunRange_range_3(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("range",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<FunRange>(ctx, in[0], "ptr_FunRange");
  double d = unwrap< double >(in[1]);
  out[0] = wrap_shared_ptr(ctx, std::make_shared<FunRange>(obj->range(d)),"FunRange", false);
}

void FunRange_create_4(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("FunRange.create",nargout,nargin,0);
  out[0] = wrap_shared_ptr(ctx, std::make_shared<FunRange>(FunRange::create()),"FunRange", false);
}

void FunDouble_collectorInsertAndMakeBase_5(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<Fun<double>> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_FunDouble.insert(self);
}

void FunDouble_deconstructor_6(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<Fun<double>> Shared;
  checkArguments("delete_FunDouble",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_FunDouble::iterator item;
  item = collector_FunDouble.find(self);
  if(item == collector_FunDouble.end()) {
    return;
  }
  collector_FunDouble.erase(item);
  delete self;
}

void FunDouble_multiTemplatedMethod_7(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("multiTemplatedMethodStringSize_t",nargout,nargin-1,3);
  auto obj = unwrap_shared_ptr<Fun<double>>(ctx, in[0], "ptr_FunDouble");
  double d = unwrap< double >(in[1]);
  string t = unwrap< string >(in[2]);
  size_t u = unwrap< size_t >(in[3]);
  out[0] = wrap_shared_ptr(ctx, std::make_shared<Fun<double>>(obj->multiTemplatedMethod<string,size_t>(d,t,u)),"Fun<double>", false);
}

void FunDouble_sets_8(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("sets",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<Fun<double>>(ctx, in[0], "ptr_FunDouble");
  out[0] = wrap_shared_ptr(ctx, std::make_shared<std::map<double,Fun<double>::double>>(obj->sets()),"std.mapdoubledouble", false);
}

void FunDouble_templatedMethod_9(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("templatedMethodString",nargout,nargin-1,2);
  auto obj = unwrap_shared_ptr<Fun<double>>(ctx, in[0], "ptr_FunDouble");
  double d = unwrap< double >(in[1]);
  string t = unwrap< string >(in[2]);
  out[0] = wrap_shared_ptr(ctx, std::make_shared<Fun<double>>(obj->templatedMethod<string>(d,t)),"Fun<double>", false);
}

void FunDouble_staticMethodWithThis_10(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("Fun<double>.staticMethodWithThis",nargout,nargin,0);
  out[0] = wrap_shared_ptr(ctx, std::make_shared<Fun<double>>(Fun<double>::staticMethodWithThis()),"Fundouble", false);
}

void FunDouble_templatedStaticMethodInt_11(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("Fun<double>.templatedStaticMethodInt",nargout,nargin,1);
  int m = unwrap< int >(in[0]);
  out[0] = wrap< double >(Fun<double>::templatedStaticMethod(m));
}

void Test_collectorInsertAndMakeBase_12(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<Test> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_Test.insert(self);
}

void Test_constructor_13(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<Test> Shared;

  Shared *self = new Shared(new Test());
  collector_Test.insert(self);
  out[0] = make_handle<Shared>(self);
}

void Test_constructor_14(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<Test> Shared;

  double a = unwrap< double >(in[0]);
  Matrix b = unwrap< Matrix >(in[1]);
  Shared *self = new Shared(new Test(a,b));
  collector_Test.insert(self);
  out[0] = make_handle<Shared>(self);
}

void Test_deconstructor_15(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<Test> Shared;
  checkArguments("delete_Test",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_Test::iterator item;
  item = collector_Test.find(self);
  if(item == collector_Test.end()) {
    return;
  }
  collector_Test.erase(item);
  delete self;
}

void Test_arg_EigenConstRef_16(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("arg_EigenConstRef",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  Matrix value = unwrap< Matrix >(in[1]);
  obj->arg_EigenConstRef(value);
}

void Test_create_MixedPtrs_17(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("create_MixedPtrs",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  auto pairResult = obj->create_MixedPtrs();
  out[0] = wrap_shared_ptr(ctx, std::make_shared<Test>(pairResult.first),"Test", false);
  out[1] = wrap_shared_ptr(ctx, pairResult.second,"Test", false);
}

void Test_create_ptrs_18(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("create_ptrs",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  auto pairResult = obj->create_ptrs();
  out[0] = wrap_shared_ptr(ctx, pairResult.first,"Test", false);
  out[1] = wrap_shared_ptr(ctx, pairResult.second,"Test", false);
}

void Test_get_container_19(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("get_container",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  out[0] = wrap_shared_ptr(ctx, std::make_shared<std::vector<testing::Test>>(obj->get_container()),"std.vectorTest", false);
}

void Test_lambda_20(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("lambda",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  obj->lambda();
}

void Test_markdown_21(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("markdown",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  gtsam::KeyFormatter& keyFormatter = *unwrap_shared_ptr< gtsam::KeyFormatter >(ctx, in[1], "ptr_gtsamKeyFormatter");
  out[0] = wrap< string >(obj->markdown(keyFormatter));
}

void Test_markdown_22(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("markdown",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  out[0] = wrap< string >(obj->markdown(gtsam::DefaultKeyFormatter));
}

void Test_print_23(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("print",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  obj->print();
}

void Test_push_back_24(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("push_back",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  gtsam::Key key = unwrap< gtsam::Key >(in[1]);
  obj->push_back(key);
}

void Test_return_Point2Ptr_25(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_Point2Ptr",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  bool value = unwrap< bool >(in[1]);
  {
  std::shared_ptr<Point2> shared(obj->return_Point2Ptr(value));
  out[0] = wrap_shared_ptr(ctx, shared,"Point2", false);
  }
}

void Test_return_Test_26(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_Test",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  std::shared_ptr<Test> value = unwrap_shared_ptr< Test >(ctx, in[1], "ptr_Test");
  out[0] = wrap_shared_ptr(ctx, std::make_shared<Test>(obj->return_Test(value)),"Test", false);
}

void Test_return_TestPtr_27(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_TestPtr",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  std::shared_ptr<Test> value = unwrap_shared_ptr< Test >(ctx, in[1], "ptr_Test");
  out[0] = wrap_shared_ptr(ctx, obj->return_TestPtr(value),"Test", false);
}

void Test_return_bool_28(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_bool",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  bool value = unwrap< bool >(in[1]);
  out[0] = wrap< bool >(obj->return_bool(value));
}

void Test_return_double_29(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_double",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  double value = unwrap< double >(in[1]);
  out[0] = wrap< double >(obj->return_double(value));
}

void Test_return_field_30(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_field",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  Test& t = *unwrap_shared_ptr< Test >(ctx, in[1], "ptr_Test");
  out[0] = wrap< bool >(obj->return_field(t));
}

void Test_return_int_31(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_int",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  int value = unwrap< int >(in[1]);
  out[0] = wrap< int >(obj->return_int(value));
}

void Test_return_matrix1_32(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_matrix1",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  Matrix value = unwrap< Matrix >(in[1]);
  out[0] = wrap< Matrix >(obj->return_matrix1(value));
}

void Test_return_matrix2_33(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_matrix2",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  Matrix value = unwrap< Matrix >(in[1]);
  out[0] = wrap< Matrix >(obj->return_matrix2(value));
}

void Test_return_matrix2_34(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_matrix2",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  Matrix value = unwrap< Matrix >(in[1]);
  out[0] = wrap< Matrix >(obj->return_matrix2(value));
}

void Test_return_pair_35(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_pair",nargout,nargin-1,2);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  Vector v = unwrap< Vector >(in[1]);
  Matrix A = unwrap< Matrix >(in[2]);
  auto pairResult = obj->return_pair(v,A);
  out[0] = wrap< Vector >(pairResult.first);
  out[1] = wrap< Matrix >(pairResult.second);
}

void Test_return_pair_36(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_pair",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  Vector v = unwrap< Vector >(in[1]);
  auto pairResult = obj->return_pair(v);
  out[0] = wrap< Vector >(pairResult.first);
  out[1] = wrap< Matrix >(pairResult.second);
}

void Test_return_ptrs_37(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_ptrs",nargout,nargin-1,2);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  std::shared_ptr<Test> p1 = unwrap_shared_ptr< Test >(ctx, in[1], "ptr_Test");
  std::shared_ptr<Test> p2 = unwrap_shared_ptr< Test >(ctx, in[2], "ptr_Test");
  auto pairResult = obj->return_ptrs(p1,p2);
  out[0] = wrap_shared_ptr(ctx, pairResult.first,"Test", false);
  out[1] = wrap_shared_ptr(ctx, pairResult.second,"Test", false);
}

void Test_return_size_t_38(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_size_t",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  size_t value = unwrap< size_t >(in[1]);
  out[0] = wrap< size_t >(obj->return_size_t(value));
}

void Test_return_string_39(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_string",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  string value = unwrap< string >(in[1]);
  out[0] = wrap< string >(obj->return_string(value));
}

void Test_return_vector1_40(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_vector1",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  Vector value = unwrap< Vector >(in[1]);
  out[0] = wrap< Vector >(obj->return_vector1(value));
}

void Test_return_vector2_41(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_vector2",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  Vector value = unwrap< Vector >(in[1]);
  out[0] = wrap< Vector >(obj->return_vector2(value));
}

void Test_return_vector2_42(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_vector2",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  Vector value = unwrap< Vector >(in[1]);
  out[0] = wrap< Vector >(obj->return_vector2(value));
}

void Test_set_container_43(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("set_container",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  std::shared_ptr<std::vector<testing::Test>> container = unwrap_shared_ptr< std::vector<testing::Test> >(ctx, in[1], "ptr_stdvectorTest");
  obj->set_container(*container);
}

void Test_set_container_44(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("set_container",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  std::shared_ptr<std::vector<testing::Test>> container = unwrap_shared_ptr< std::vector<testing::Test> >(ctx, in[1], "ptr_stdvectorTest");
  obj->set_container(*container);
}

void Test_set_container_45(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("set_container",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  std::shared_ptr<std::vector<testing::Test>> container = unwrap_shared_ptr< std::vector<testing::Test> >(ctx, in[1], "ptr_stdvectorTest");
  obj->set_container(*container);
}

void Test_get_model_ptr_46(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("model_ptr",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  out[0] = wrap_shared_ptr(ctx, obj->model_ptr,"gtsam.noiseModel.Base", false);
}

void Test_set_model_ptr_47(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("model_ptr",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  std::shared_ptr<gtsam::noiseModel::Base> model_ptr = unwrap_shared_ptr< gtsam::noiseModel::Base >(ctx, in[1], "ptr_gtsamnoiseModelBase");
  obj->model_ptr = *model_ptr;
}

void Test_get_value_48(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("value",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  out[0] = wrap< double >(obj->value);
}

void Test_set_value_49(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("value",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  double value = unwrap< double >(in[1]);
  obj->value = value;
}

void Test_get_name_50(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("name",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  out[0] = wrap< string >(obj->name);
}

void Test_set_name_51(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("name",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<Test>(ctx, in[0], "ptr_Test");
  string name = unwrap< string >(in[1]);
  obj->name = name;
}

void PrimitiveRefDouble_collectorInsertAndMakeBase_52(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<PrimitiveRef<double>> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_PrimitiveRefDouble.insert(self);
}

void PrimitiveRefDouble_constructor_53(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<PrimitiveRef<double>> Shared;

  Shared *self = new Shared(new PrimitiveRef<double>());
  collector_PrimitiveRefDouble.insert(self);
  out[0] = make_handle<Shared>(self);
}

void PrimitiveRefDouble_deconstructor_54(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<PrimitiveRef<double>> Shared;
  checkArguments("delete_PrimitiveRefDouble",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_PrimitiveRefDouble::iterator item;
  item = collector_PrimitiveRefDouble.find(self);
  if(item == collector_PrimitiveRefDouble.end()) {
    return;
  }
  collector_PrimitiveRefDouble.erase(item);
  delete self;
}

void PrimitiveRefDouble_Brutal_55(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("PrimitiveRef<double>.Brutal",nargout,nargin,1);
  double t = unwrap< double >(in[0]);
  out[0] = wrap_shared_ptr(ctx, std::make_shared<PrimitiveRef<double>>(PrimitiveRef<double>::Brutal(t)),"PrimitiveRefdouble", false);
}

void MyVector3_collectorInsertAndMakeBase_56(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MyVector<3>> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_MyVector3.insert(self);
}

void MyVector3_constructor_57(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MyVector<3>> Shared;

  Shared *self = new Shared(new MyVector<3>());
  collector_MyVector3.insert(self);
  out[0] = make_handle<Shared>(self);
}

void MyVector3_deconstructor_58(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MyVector<3>> Shared;
  checkArguments("delete_MyVector3",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_MyVector3::iterator item;
  item = collector_MyVector3.find(self);
  if(item == collector_MyVector3.end()) {
    return;
  }
  collector_MyVector3.erase(item);
  delete self;
}

void MyVector12_collectorInsertAndMakeBase_59(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MyVector<12>> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_MyVector12.insert(self);
}

void MyVector12_constructor_60(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MyVector<12>> Shared;

  Shared *self = new Shared(new MyVector<12>());
  collector_MyVector12.insert(self);
  out[0] = make_handle<Shared>(self);
}

void MyVector12_deconstructor_61(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MyVector<12>> Shared;
  checkArguments("delete_MyVector12",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_MyVector12::iterator item;
  item = collector_MyVector12.find(self);
  if(item == collector_MyVector12.end()) {
    return;
  }
  collector_MyVector12.erase(item);
  delete self;
}

void MultipleTemplatesIntDouble_collectorInsertAndMakeBase_62(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MultipleTemplates<int, double>> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_MultipleTemplatesIntDouble.insert(self);
}

void MultipleTemplatesIntDouble_deconstructor_63(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MultipleTemplates<int, double>> Shared;
  checkArguments("delete_MultipleTemplatesIntDouble",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_MultipleTemplatesIntDouble::iterator item;
  item = collector_MultipleTemplatesIntDouble.find(self);
  if(item == collector_MultipleTemplatesIntDouble.end()) {
    return;
  }
  collector_MultipleTemplatesIntDouble.erase(item);
  delete self;
}

void MultipleTemplatesIntFloat_collectorInsertAndMakeBase_64(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MultipleTemplates<int, float>> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_MultipleTemplatesIntFloat.insert(self);
}

void MultipleTemplatesIntFloat_deconstructor_65(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MultipleTemplates<int, float>> Shared;
  checkArguments("delete_MultipleTemplatesIntFloat",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_MultipleTemplatesIntFloat::iterator item;
  item = collector_MultipleTemplatesIntFloat.find(self);
  if(item == collector_MultipleTemplatesIntFloat.end()) {
    return;
  }
  collector_MultipleTemplatesIntFloat.erase(item);
  delete self;
}

void ForwardKinematics_collectorInsertAndMakeBase_66(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ForwardKinematics> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_ForwardKinematics.insert(self);
}

void ForwardKinematics_constructor_67(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ForwardKinematics> Shared;

  gtdynamics::Robot& robot = *unwrap_shared_ptr< gtdynamics::Robot >(ctx, in[0], "ptr_gtdynamicsRobot");
  string start_link_name = unwrap< string >(in[1]);
  string end_link_name = unwrap< string >(in[2]);
  gtsam::Values& joint_angles = *unwrap_shared_ptr< gtsam::Values >(ctx, in[3], "ptr_gtsamValues");
  gtsam::Pose3& l2Tp = *unwrap_shared_ptr< gtsam::Pose3 >(ctx, in[4], "ptr_gtsamPose3");
  Shared *self = new Shared(new ForwardKinematics(robot,start_link_name,end_link_name,joint_angles,l2Tp));
  collector_ForwardKinematics.insert(self);
  out[0] = make_handle<Shared>(self);
}

void ForwardKinematics_constructor_68(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ForwardKinematics> Shared;

  gtdynamics::Robot& robot = *unwrap_shared_ptr< gtdynamics::Robot >(ctx, in[0], "ptr_gtdynamicsRobot");
  string start_link_name = unwrap< string >(in[1]);
  string end_link_name = unwrap< string >(in[2]);
  gtsam::Values& joint_angles = *unwrap_shared_ptr< gtsam::Values >(ctx, in[3], "ptr_gtsamValues");
  Shared *self = new Shared(new ForwardKinematics(robot,start_link_name,end_link_name,joint_angles,gtsam::Pose3()));
  collector_ForwardKinematics.insert(self);
  out[0] = make_handle<Shared>(self);
}

void ForwardKinematics_deconstructor_69(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ForwardKinematics> Shared;
  checkArguments("delete_ForwardKinematics",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_ForwardKinematics::iterator item;
  item = collector_ForwardKinematics.find(self);
  if(item == collector_ForwardKinematics.end()) {
    return;
  }
  collector_ForwardKinematics.erase(item);
  delete self;
}

void TemplatedConstructor_collectorInsertAndMakeBase_70(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<TemplatedConstructor> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_TemplatedConstructor.insert(self);
}

void TemplatedConstructor_constructor_71(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<TemplatedConstructor> Shared;

  Shared *self = new Shared(new TemplatedConstructor());
  collector_TemplatedConstructor.insert(self);
  out[0] = make_handle<Shared>(self);
}

void TemplatedConstructor_constructor_72(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<TemplatedConstructor> Shared;

  string arg = unwrap< string >(in[0]);
  Shared *self = new Shared(new TemplatedConstructor(arg));
  collector_TemplatedConstructor.insert(self);
  out[0] = make_handle<Shared>(self);
}

void TemplatedConstructor_constructor_73(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<TemplatedConstructor> Shared;

  int arg = unwrap< int >(in[0]);
  Shared *self = new Shared(new TemplatedConstructor(arg));
  collector_TemplatedConstructor.insert(self);
  out[0] = make_handle<Shared>(self);
}

void TemplatedConstructor_constructor_74(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<TemplatedConstructor> Shared;

  double arg = unwrap< double >(in[0]);
  Shared *self = new Shared(new TemplatedConstructor(arg));
  collector_TemplatedConstructor.insert(self);
  out[0] = make_handle<Shared>(self);
}

void TemplatedConstructor_deconstructor_75(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
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

void FastSet_collectorInsertAndMakeBase_76(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<FastSet> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_FastSet.insert(self);
}

void FastSet_constructor_77(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<FastSet> Shared;

  Shared *self = new Shared(new FastSet());
  collector_FastSet.insert(self);
  out[0] = make_handle<Shared>(self);
}

void FastSet_deconstructor_78(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<FastSet> Shared;
  checkArguments("delete_FastSet",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_FastSet::iterator item;
  item = collector_FastSet.find(self);
  if(item == collector_FastSet.end()) {
    return;
  }
  collector_FastSet.erase(item);
  delete self;
}

void HessianFactor_collectorInsertAndMakeBase_79(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<HessianFactor> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_HessianFactor.insert(self);

  typedef std::shared_ptr<gtsam::GaussianFactor> SharedBase;
  out[0] = make_handle<SharedBase>(new SharedBase(*self));
}

void HessianFactor_upcastFromVoid_80(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in) {
  typedef std::shared_ptr<HessianFactor> Shared;
  std::shared_ptr<void> *asVoid = get_handle<std::shared_ptr<void>>(in[0]);
  Shared *self = new Shared(std::static_pointer_cast<HessianFactor>(*asVoid));
  collector_HessianFactor.insert(self);
  out[0] = make_handle<Shared>(self);
}

void HessianFactor_constructor_81(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<HessianFactor> Shared;

  gtsam::KeyVector& js = *unwrap_shared_ptr< gtsam::KeyVector >(ctx, in[0], "ptr_gtsamKeyVector");
  std::vector<Matrix>& Gs = *unwrap_shared_ptr< std::vector<Matrix> >(ctx, in[1], "ptr_stdvectorMatrix");
  std::vector<Vector>& gs = *unwrap_shared_ptr< std::vector<Vector> >(ctx, in[2], "ptr_stdvectorVector");
  double f = unwrap< double >(in[3]);
  Shared *self = new Shared(new HessianFactor(js,Gs,gs,f));
  collector_HessianFactor.insert(self);
  out[0] = make_handle<Shared>(self);

  typedef std::shared_ptr<gtsam::GaussianFactor> SharedBase;
  out[1] = make_handle<SharedBase>(new SharedBase(*self));
}

void HessianFactor_deconstructor_82(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<HessianFactor> Shared;
  checkArguments("delete_HessianFactor",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_HessianFactor::iterator item;
  item = collector_HessianFactor.find(self);
  if(item == collector_HessianFactor.end()) {
    return;
  }
  collector_HessianFactor.erase(item);
  delete self;
}

void SmartProjectionRigFactorPinholeCameraCal3_S2_collectorInsertAndMakeBase_83(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<SmartProjectionRigFactor<gtsam::PinholeCamera<gtsam::Cal3_S2>>> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_SmartProjectionRigFactorPinholeCameraCal3_S2.insert(self);

  typedef std::shared_ptr<gtsam::SmartProjectionFactor<gtsam::PinholeCamera<gtsam::Cal3_S2>>> SharedBase;
  out[0] = make_handle<SharedBase>(new SharedBase(*self));
}

void SmartProjectionRigFactorPinholeCameraCal3_S2_deconstructor_84(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<SmartProjectionRigFactor<gtsam::PinholeCamera<gtsam::Cal3_S2>>> Shared;
  checkArguments("delete_SmartProjectionRigFactorPinholeCameraCal3_S2",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_SmartProjectionRigFactorPinholeCameraCal3_S2::iterator item;
  item = collector_SmartProjectionRigFactorPinholeCameraCal3_S2.find(self);
  if(item == collector_SmartProjectionRigFactorPinholeCameraCal3_S2.end()) {
    return;
  }
  collector_SmartProjectionRigFactorPinholeCameraCal3_S2.erase(item);
  delete self;
}

void SmartProjectionRigFactorPinholeCameraCal3_S2_add_85(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("add",nargout,nargin-1,3);
  auto obj = unwrap_shared_ptr<SmartProjectionRigFactor<gtsam::PinholeCamera<gtsam::Cal3_S2>>>(ctx, in[0], "ptr_SmartProjectionRigFactorPinholeCameraCal3_S2");
  gtsam::PinholeCamera<gtsam::Cal3_S2>::Measurement& measured = *unwrap_shared_ptr< gtsam::PinholeCamera<gtsam::Cal3_S2>::Measurement >(ctx, in[1], "ptr_gtsamPinholeCamera<gtsam::Cal3_S2>::Measurement");
  gtsam::Key poseKey = unwrap< gtsam::Key >(in[2]);
  size_t cameraId = unwrap< size_t >(in[3]);
  obj->add(measured,poseKey,cameraId);
}

void SmartProjectionRigFactorPinholeCameraCal3_S2_add_86(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("add",nargout,nargin-1,2);
  auto obj = unwrap_shared_ptr<SmartProjectionRigFactor<gtsam::PinholeCamera<gtsam::Cal3_S2>>>(ctx, in[0], "ptr_SmartProjectionRigFactorPinholeCameraCal3_S2");
  gtsam::PinholeCamera<gtsam::Cal3_S2>::Measurement& measured = *unwrap_shared_ptr< gtsam::PinholeCamera<gtsam::Cal3_S2>::Measurement >(ctx, in[1], "ptr_gtsamPinholeCamera<gtsam::Cal3_S2>::Measurement");
  gtsam::Key poseKey = unwrap< gtsam::Key >(in[2]);
  obj->add(measured,poseKey,0);
}

void MyFactorPosePoint2_collectorInsertAndMakeBase_87(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MyFactor<gtsam::Pose2, gtsam::Matrix>> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_MyFactorPosePoint2.insert(self);
}

void MyFactorPosePoint2_constructor_88(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MyFactor<gtsam::Pose2, gtsam::Matrix>> Shared;

  size_t key1 = unwrap< size_t >(in[0]);
  size_t key2 = unwrap< size_t >(in[1]);
  double measured = unwrap< double >(in[2]);
  std::shared_ptr<gtsam::noiseModel::Base> noiseModel = unwrap_shared_ptr< gtsam::noiseModel::Base >(ctx, in[3], "ptr_gtsamnoiseModelBase");
  Shared *self = new Shared(new MyFactor<gtsam::Pose2, gtsam::Matrix>(key1,key2,measured,noiseModel));
  collector_MyFactorPosePoint2.insert(self);
  out[0] = make_handle<Shared>(self);
}

void MyFactorPosePoint2_deconstructor_89(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MyFactor<gtsam::Pose2, gtsam::Matrix>> Shared;
  checkArguments("delete_MyFactorPosePoint2",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_MyFactorPosePoint2::iterator item;
  item = collector_MyFactorPosePoint2.find(self);
  if(item == collector_MyFactorPosePoint2.end()) {
    return;
  }
  collector_MyFactorPosePoint2.erase(item);
  delete self;
}

void MyFactorPosePoint2_print_90(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("print",nargout,nargin-1,2);
  auto obj = unwrap_shared_ptr<MyFactor<gtsam::Pose2, gtsam::Matrix>>(ctx, in[0], "ptr_MyFactorPosePoint2");
  string s = unwrap< string >(in[1]);
  gtsam::KeyFormatter& keyFormatter = *unwrap_shared_ptr< gtsam::KeyFormatter >(ctx, in[2], "ptr_gtsamKeyFormatter");
  obj->print(s,keyFormatter);
}

void MyFactorPosePoint2_print_91(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("print",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyFactor<gtsam::Pose2, gtsam::Matrix>>(ctx, in[0], "ptr_MyFactorPosePoint2");
  string s = unwrap< string >(in[1]);
  obj->print(s,gtsam::DefaultKeyFormatter);
}

void MyFactorPosePoint2_print_92(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("print",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<MyFactor<gtsam::Pose2, gtsam::Matrix>>(ctx, in[0], "ptr_MyFactorPosePoint2");
  obj->print("factor: ",gtsam::DefaultKeyFormatter);
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
      _class_RTTIRegister(ctx.engine);
      rttiRegistered_ = true;
    }

    gtwrap::CoutRedirect coutRedirect(ctx.engine);

    int id = unwrap<int>(inputs[0]);
    WrapIn in(inputs, 1);
    OutputList out;

    try {
      switch(id) {
        case 0:
          FunRange_collectorInsertAndMakeBase_0(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 1:
          FunRange_constructor_1(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 2:
          FunRange_deconstructor_2(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 3:
          FunRange_range_3(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 4:
          FunRange_create_4(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 5:
          FunDouble_collectorInsertAndMakeBase_5(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 6:
          FunDouble_deconstructor_6(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 7:
          FunDouble_multiTemplatedMethod_7(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 8:
          FunDouble_sets_8(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 9:
          FunDouble_templatedMethod_9(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 10:
          FunDouble_staticMethodWithThis_10(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 11:
          FunDouble_templatedStaticMethodInt_11(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 12:
          Test_collectorInsertAndMakeBase_12(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 13:
          Test_constructor_13(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 14:
          Test_constructor_14(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 15:
          Test_deconstructor_15(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 16:
          Test_arg_EigenConstRef_16(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 17:
          Test_create_MixedPtrs_17(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 18:
          Test_create_ptrs_18(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 19:
          Test_get_container_19(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 20:
          Test_lambda_20(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 21:
          Test_markdown_21(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 22:
          Test_markdown_22(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 23:
          Test_print_23(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 24:
          Test_push_back_24(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 25:
          Test_return_Point2Ptr_25(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 26:
          Test_return_Test_26(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 27:
          Test_return_TestPtr_27(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 28:
          Test_return_bool_28(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 29:
          Test_return_double_29(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 30:
          Test_return_field_30(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 31:
          Test_return_int_31(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 32:
          Test_return_matrix1_32(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 33:
          Test_return_matrix2_33(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 34:
          Test_return_matrix2_34(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 35:
          Test_return_pair_35(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 36:
          Test_return_pair_36(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 37:
          Test_return_ptrs_37(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 38:
          Test_return_size_t_38(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 39:
          Test_return_string_39(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 40:
          Test_return_vector1_40(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 41:
          Test_return_vector2_41(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 42:
          Test_return_vector2_42(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 43:
          Test_set_container_43(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 44:
          Test_set_container_44(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 45:
          Test_set_container_45(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 46:
          Test_get_model_ptr_46(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 47:
          Test_set_model_ptr_47(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 48:
          Test_get_value_48(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 49:
          Test_set_value_49(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 50:
          Test_get_name_50(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 51:
          Test_set_name_51(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 52:
          PrimitiveRefDouble_collectorInsertAndMakeBase_52(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 53:
          PrimitiveRefDouble_constructor_53(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 54:
          PrimitiveRefDouble_deconstructor_54(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 55:
          PrimitiveRefDouble_Brutal_55(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 56:
          MyVector3_collectorInsertAndMakeBase_56(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 57:
          MyVector3_constructor_57(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 58:
          MyVector3_deconstructor_58(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 59:
          MyVector12_collectorInsertAndMakeBase_59(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 60:
          MyVector12_constructor_60(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 61:
          MyVector12_deconstructor_61(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 62:
          MultipleTemplatesIntDouble_collectorInsertAndMakeBase_62(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 63:
          MultipleTemplatesIntDouble_deconstructor_63(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 64:
          MultipleTemplatesIntFloat_collectorInsertAndMakeBase_64(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 65:
          MultipleTemplatesIntFloat_deconstructor_65(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 66:
          ForwardKinematics_collectorInsertAndMakeBase_66(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 67:
          ForwardKinematics_constructor_67(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 68:
          ForwardKinematics_constructor_68(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 69:
          ForwardKinematics_deconstructor_69(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 70:
          TemplatedConstructor_collectorInsertAndMakeBase_70(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 71:
          TemplatedConstructor_constructor_71(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 72:
          TemplatedConstructor_constructor_72(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 73:
          TemplatedConstructor_constructor_73(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 74:
          TemplatedConstructor_constructor_74(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 75:
          TemplatedConstructor_deconstructor_75(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 76:
          FastSet_collectorInsertAndMakeBase_76(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 77:
          FastSet_constructor_77(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 78:
          FastSet_deconstructor_78(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 79:
          HessianFactor_collectorInsertAndMakeBase_79(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 80:
          HessianFactor_upcastFromVoid_80(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 81:
          HessianFactor_constructor_81(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 82:
          HessianFactor_deconstructor_82(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 83:
          SmartProjectionRigFactorPinholeCameraCal3_S2_collectorInsertAndMakeBase_83(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 84:
          SmartProjectionRigFactorPinholeCameraCal3_S2_deconstructor_84(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 85:
          SmartProjectionRigFactorPinholeCameraCal3_S2_add_85(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 86:
          SmartProjectionRigFactorPinholeCameraCal3_S2_add_86(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 87:
          MyFactorPosePoint2_collectorInsertAndMakeBase_87(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 88:
          MyFactorPosePoint2_constructor_88(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 89:
          MyFactorPosePoint2_deconstructor_89(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 90:
          MyFactorPosePoint2_print_90(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 91:
          MyFactorPosePoint2_print_91(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 92:
          MyFactorPosePoint2_print_92(ctx, (int)outputs.size(), out, (int)in.size(), in);
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
