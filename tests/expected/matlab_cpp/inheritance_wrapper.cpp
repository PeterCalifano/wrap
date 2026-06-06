#include <wrap/matlab_cpp.h>
#include <map>



typedef MyTemplate<gtsam::Point2> MyTemplatePoint2;
typedef MyTemplate<gtsam::Matrix> MyTemplateMatrix;
typedef MyTemplate<A> MyTemplateA;
typedef ParentHasTemplate<double> ParentHasTemplateDouble;

typedef std::set<std::shared_ptr<MyBase>*> Collector_MyBase;
static Collector_MyBase collector_MyBase;
typedef std::set<std::shared_ptr<MyTemplatePoint2>*> Collector_MyTemplatePoint2;
static Collector_MyTemplatePoint2 collector_MyTemplatePoint2;
typedef std::set<std::shared_ptr<MyTemplateMatrix>*> Collector_MyTemplateMatrix;
static Collector_MyTemplateMatrix collector_MyTemplateMatrix;
typedef std::set<std::shared_ptr<MyTemplateA>*> Collector_MyTemplateA;
static Collector_MyTemplateA collector_MyTemplateA;
typedef std::set<std::shared_ptr<ForwardKinematicsFactor>*> Collector_ForwardKinematicsFactor;
static Collector_ForwardKinematicsFactor collector_ForwardKinematicsFactor;
typedef std::set<std::shared_ptr<ParentHasTemplateDouble>*> Collector_ParentHasTemplateDouble;
static Collector_ParentHasTemplateDouble collector_ParentHasTemplateDouble;


void _deleteAllObjects(matlab::engine::MATLABEngine* engine)
{
  bool anyDeleted = false;
  { for(Collector_MyBase::iterator iter = collector_MyBase.begin();
      iter != collector_MyBase.end(); ) {
    delete *iter;
    collector_MyBase.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_MyTemplatePoint2::iterator iter = collector_MyTemplatePoint2.begin();
      iter != collector_MyTemplatePoint2.end(); ) {
    delete *iter;
    collector_MyTemplatePoint2.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_MyTemplateMatrix::iterator iter = collector_MyTemplateMatrix.begin();
      iter != collector_MyTemplateMatrix.end(); ) {
    delete *iter;
    collector_MyTemplateMatrix.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_MyTemplateA::iterator iter = collector_MyTemplateA.begin();
      iter != collector_MyTemplateA.end(); ) {
    delete *iter;
    collector_MyTemplateA.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_ForwardKinematicsFactor::iterator iter = collector_ForwardKinematicsFactor.begin();
      iter != collector_ForwardKinematicsFactor.end(); ) {
    delete *iter;
    collector_ForwardKinematicsFactor.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_ParentHasTemplateDouble::iterator iter = collector_ParentHasTemplateDouble.begin();
      iter != collector_ParentHasTemplateDouble.end(); ) {
    delete *iter;
    collector_ParentHasTemplateDouble.erase(iter++);
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

void _inheritance_RTTIRegister(matlab::engine::MATLABEngine* engine) {
  bool alreadyCreated = false;
  try {
    engine->getVariable(u"gtsam_inheritance_rttiRegistry_created",
                        matlab::engine::WorkspaceType::GLOBAL);
    alreadyCreated = true;
  } catch(const std::exception&) {
    alreadyCreated = false;
  }

  if(!alreadyCreated) {
    std::map<std::string, std::string> types;
    types.insert(std::make_pair(typeid(MyBase).name(), "MyBase"));
    types.insert(std::make_pair(typeid(MyTemplatePoint2).name(), "MyTemplatePoint2"));
    types.insert(std::make_pair(typeid(MyTemplateMatrix).name(), "MyTemplateMatrix"));
    types.insert(std::make_pair(typeid(MyTemplateA).name(), "MyTemplateA"));
    types.insert(std::make_pair(typeid(ForwardKinematicsFactor).name(), "ForwardKinematicsFactor"));
    types.insert(std::make_pair(typeid(ParentHasTemplateDouble).name(), "ParentHasTemplateDouble"));

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
    engine->setVariable(u"gtsam_inheritance_rttiRegistry_created",
                        f_.createScalar<std::int8_t>(0),
                        matlab::engine::WorkspaceType::GLOBAL);
  }
}

void MyBase_collectorInsertAndMakeBase_0(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MyBase> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_MyBase.insert(self);
}

void MyBase_upcastFromVoid_1(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in) {
  typedef std::shared_ptr<MyBase> Shared;
  std::shared_ptr<void> *asVoid = get_handle<std::shared_ptr<void>>(in[0]);
  Shared *self = new Shared(std::static_pointer_cast<MyBase>(*asVoid));
  collector_MyBase.insert(self);
  out[0] = make_handle<Shared>(self);
}

void MyBase_deconstructor_2(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MyBase> Shared;
  checkArguments("delete_MyBase",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_MyBase::iterator item;
  item = collector_MyBase.find(self);
  if(item == collector_MyBase.end()) {
    return;
  }
  collector_MyBase.erase(item);
  delete self;
}

void MyTemplatePoint2_collectorInsertAndMakeBase_3(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MyTemplate<gtsam::Point2>> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_MyTemplatePoint2.insert(self);

  typedef std::shared_ptr<MyBase> SharedBase;
  out[0] = make_handle<SharedBase>(new SharedBase(*self));
}

void MyTemplatePoint2_upcastFromVoid_4(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in) {
  typedef std::shared_ptr<MyTemplate<gtsam::Point2>> Shared;
  std::shared_ptr<void> *asVoid = get_handle<std::shared_ptr<void>>(in[0]);
  Shared *self = new Shared(std::static_pointer_cast<MyTemplate<gtsam::Point2>>(*asVoid));
  collector_MyTemplatePoint2.insert(self);
  out[0] = make_handle<Shared>(self);
}

void MyTemplatePoint2_constructor_5(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MyTemplate<gtsam::Point2>> Shared;

  Shared *self = new Shared(new MyTemplate<gtsam::Point2>());
  collector_MyTemplatePoint2.insert(self);
  out[0] = make_handle<Shared>(self);

  typedef std::shared_ptr<MyBase> SharedBase;
  out[1] = make_handle<SharedBase>(new SharedBase(*self));
}

void MyTemplatePoint2_deconstructor_6(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MyTemplate<gtsam::Point2>> Shared;
  checkArguments("delete_MyTemplatePoint2",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_MyTemplatePoint2::iterator item;
  item = collector_MyTemplatePoint2.find(self);
  if(item == collector_MyTemplatePoint2.end()) {
    return;
  }
  collector_MyTemplatePoint2.erase(item);
  delete self;
}

void MyTemplatePoint2_accept_T_7(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("accept_T",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Point2>>(ctx, in[0], "ptr_MyTemplatePoint2");
  Point2 value = unwrap< Point2 >(in[1]);
  obj->accept_T(value);
}

void MyTemplatePoint2_accept_Tptr_8(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("accept_Tptr",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Point2>>(ctx, in[0], "ptr_MyTemplatePoint2");
  Point2 value = unwrap< Point2 >(in[1]);
  obj->accept_Tptr(value);
}

void MyTemplatePoint2_create_MixedPtrs_9(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("create_MixedPtrs",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Point2>>(ctx, in[0], "ptr_MyTemplatePoint2");
  auto pairResult = obj->create_MixedPtrs();
  out[0] = wrap< Point2 >(pairResult.first);
  {
  std::shared_ptr<Point2> shared(pairResult.second);
  out[1] = wrap_shared_ptr(ctx, shared,"Point2", false);
  }
}

void MyTemplatePoint2_create_ptrs_10(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("create_ptrs",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Point2>>(ctx, in[0], "ptr_MyTemplatePoint2");
  auto pairResult = obj->create_ptrs();
  {
  std::shared_ptr<Point2> shared(pairResult.first);
  out[0] = wrap_shared_ptr(ctx, shared,"Point2", false);
  }
  {
  std::shared_ptr<Point2> shared(pairResult.second);
  out[1] = wrap_shared_ptr(ctx, shared,"Point2", false);
  }
}

void MyTemplatePoint2_return_T_11(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_T",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Point2>>(ctx, in[0], "ptr_MyTemplatePoint2");
  Point2 value = unwrap< Point2 >(in[1]);
  out[0] = wrap< Point2 >(obj->return_T(value));
}

void MyTemplatePoint2_return_Tptr_12(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_Tptr",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Point2>>(ctx, in[0], "ptr_MyTemplatePoint2");
  Point2 value = unwrap< Point2 >(in[1]);
  {
  std::shared_ptr<Point2> shared(obj->return_Tptr(value));
  out[0] = wrap_shared_ptr(ctx, shared,"Point2", false);
  }
}

void MyTemplatePoint2_return_ptrs_13(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_ptrs",nargout,nargin-1,2);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Point2>>(ctx, in[0], "ptr_MyTemplatePoint2");
  Point2 p1 = unwrap< Point2 >(in[1]);
  Point2 p2 = unwrap< Point2 >(in[2]);
  auto pairResult = obj->return_ptrs(p1,p2);
  {
  std::shared_ptr<Point2> shared(pairResult.first);
  out[0] = wrap_shared_ptr(ctx, shared,"Point2", false);
  }
  {
  std::shared_ptr<Point2> shared(pairResult.second);
  out[1] = wrap_shared_ptr(ctx, shared,"Point2", false);
  }
}

void MyTemplatePoint2_templatedMethod_14(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("templatedMethodMatrix",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Point2>>(ctx, in[0], "ptr_MyTemplatePoint2");
  Matrix t = unwrap< Matrix >(in[1]);
  out[0] = wrap< Matrix >(obj->templatedMethod<gtsam::Matrix>(t));
}

void MyTemplatePoint2_templatedMethod_15(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("templatedMethodPoint2",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Point2>>(ctx, in[0], "ptr_MyTemplatePoint2");
  Point2 t = unwrap< Point2 >(in[1]);
  out[0] = wrap< Point2 >(obj->templatedMethod<gtsam::Point2>(t));
}

void MyTemplatePoint2_templatedMethod_16(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("templatedMethodPoint3",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Point2>>(ctx, in[0], "ptr_MyTemplatePoint2");
  Point3 t = unwrap< Point3 >(in[1]);
  out[0] = wrap< Point3 >(obj->templatedMethod<gtsam::Point3>(t));
}

void MyTemplatePoint2_templatedMethod_17(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("templatedMethodVector",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Point2>>(ctx, in[0], "ptr_MyTemplatePoint2");
  Vector t = unwrap< Vector >(in[1]);
  out[0] = wrap< Vector >(obj->templatedMethod<gtsam::Vector>(t));
}

void MyTemplatePoint2_Level_18(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("MyTemplate<gtsam::Point2>.Level",nargout,nargin,1);
  Point2 K = unwrap< Point2 >(in[0]);
  out[0] = wrap_shared_ptr(ctx, std::make_shared<MyTemplate<Point2>>(MyTemplate<gtsam::Point2>::Level(K)),"MyTemplatePoint2", false);
}

void MyTemplateMatrix_collectorInsertAndMakeBase_19(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MyTemplate<gtsam::Matrix>> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_MyTemplateMatrix.insert(self);

  typedef std::shared_ptr<MyBase> SharedBase;
  out[0] = make_handle<SharedBase>(new SharedBase(*self));
}

void MyTemplateMatrix_upcastFromVoid_20(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in) {
  typedef std::shared_ptr<MyTemplate<gtsam::Matrix>> Shared;
  std::shared_ptr<void> *asVoid = get_handle<std::shared_ptr<void>>(in[0]);
  Shared *self = new Shared(std::static_pointer_cast<MyTemplate<gtsam::Matrix>>(*asVoid));
  collector_MyTemplateMatrix.insert(self);
  out[0] = make_handle<Shared>(self);
}

void MyTemplateMatrix_constructor_21(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MyTemplate<gtsam::Matrix>> Shared;

  Shared *self = new Shared(new MyTemplate<gtsam::Matrix>());
  collector_MyTemplateMatrix.insert(self);
  out[0] = make_handle<Shared>(self);

  typedef std::shared_ptr<MyBase> SharedBase;
  out[1] = make_handle<SharedBase>(new SharedBase(*self));
}

void MyTemplateMatrix_deconstructor_22(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MyTemplate<gtsam::Matrix>> Shared;
  checkArguments("delete_MyTemplateMatrix",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_MyTemplateMatrix::iterator item;
  item = collector_MyTemplateMatrix.find(self);
  if(item == collector_MyTemplateMatrix.end()) {
    return;
  }
  collector_MyTemplateMatrix.erase(item);
  delete self;
}

void MyTemplateMatrix_accept_T_23(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("accept_T",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Matrix>>(ctx, in[0], "ptr_MyTemplateMatrix");
  Matrix value = unwrap< Matrix >(in[1]);
  obj->accept_T(value);
}

void MyTemplateMatrix_accept_Tptr_24(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("accept_Tptr",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Matrix>>(ctx, in[0], "ptr_MyTemplateMatrix");
  Matrix value = unwrap< Matrix >(in[1]);
  obj->accept_Tptr(value);
}

void MyTemplateMatrix_create_MixedPtrs_25(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("create_MixedPtrs",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Matrix>>(ctx, in[0], "ptr_MyTemplateMatrix");
  auto pairResult = obj->create_MixedPtrs();
  out[0] = wrap< Matrix >(pairResult.first);
  {
  std::shared_ptr<Matrix> shared(pairResult.second);
  out[1] = wrap_shared_ptr(ctx, shared,"Matrix", false);
  }
}

void MyTemplateMatrix_create_ptrs_26(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("create_ptrs",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Matrix>>(ctx, in[0], "ptr_MyTemplateMatrix");
  auto pairResult = obj->create_ptrs();
  {
  std::shared_ptr<Matrix> shared(pairResult.first);
  out[0] = wrap_shared_ptr(ctx, shared,"Matrix", false);
  }
  {
  std::shared_ptr<Matrix> shared(pairResult.second);
  out[1] = wrap_shared_ptr(ctx, shared,"Matrix", false);
  }
}

void MyTemplateMatrix_return_T_27(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_T",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Matrix>>(ctx, in[0], "ptr_MyTemplateMatrix");
  Matrix value = unwrap< Matrix >(in[1]);
  out[0] = wrap< Matrix >(obj->return_T(value));
}

void MyTemplateMatrix_return_Tptr_28(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_Tptr",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Matrix>>(ctx, in[0], "ptr_MyTemplateMatrix");
  Matrix value = unwrap< Matrix >(in[1]);
  {
  std::shared_ptr<Matrix> shared(obj->return_Tptr(value));
  out[0] = wrap_shared_ptr(ctx, shared,"Matrix", false);
  }
}

void MyTemplateMatrix_return_ptrs_29(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_ptrs",nargout,nargin-1,2);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Matrix>>(ctx, in[0], "ptr_MyTemplateMatrix");
  Matrix p1 = unwrap< Matrix >(in[1]);
  Matrix p2 = unwrap< Matrix >(in[2]);
  auto pairResult = obj->return_ptrs(p1,p2);
  {
  std::shared_ptr<Matrix> shared(pairResult.first);
  out[0] = wrap_shared_ptr(ctx, shared,"Matrix", false);
  }
  {
  std::shared_ptr<Matrix> shared(pairResult.second);
  out[1] = wrap_shared_ptr(ctx, shared,"Matrix", false);
  }
}

void MyTemplateMatrix_templatedMethod_30(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("templatedMethodMatrix",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Matrix>>(ctx, in[0], "ptr_MyTemplateMatrix");
  Matrix t = unwrap< Matrix >(in[1]);
  out[0] = wrap< Matrix >(obj->templatedMethod<gtsam::Matrix>(t));
}

void MyTemplateMatrix_templatedMethod_31(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("templatedMethodPoint2",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Matrix>>(ctx, in[0], "ptr_MyTemplateMatrix");
  Point2 t = unwrap< Point2 >(in[1]);
  out[0] = wrap< Point2 >(obj->templatedMethod<gtsam::Point2>(t));
}

void MyTemplateMatrix_templatedMethod_32(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("templatedMethodPoint3",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Matrix>>(ctx, in[0], "ptr_MyTemplateMatrix");
  Point3 t = unwrap< Point3 >(in[1]);
  out[0] = wrap< Point3 >(obj->templatedMethod<gtsam::Point3>(t));
}

void MyTemplateMatrix_templatedMethod_33(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("templatedMethodVector",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<gtsam::Matrix>>(ctx, in[0], "ptr_MyTemplateMatrix");
  Vector t = unwrap< Vector >(in[1]);
  out[0] = wrap< Vector >(obj->templatedMethod<gtsam::Vector>(t));
}

void MyTemplateMatrix_Level_34(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("MyTemplate<gtsam::Matrix>.Level",nargout,nargin,1);
  Matrix K = unwrap< Matrix >(in[0]);
  out[0] = wrap_shared_ptr(ctx, std::make_shared<MyTemplate<Matrix>>(MyTemplate<gtsam::Matrix>::Level(K)),"MyTemplateMatrix", false);
}

void MyTemplateA_collectorInsertAndMakeBase_35(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MyTemplate<A>> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_MyTemplateA.insert(self);

  typedef std::shared_ptr<MyBase> SharedBase;
  out[0] = make_handle<SharedBase>(new SharedBase(*self));
}

void MyTemplateA_upcastFromVoid_36(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in) {
  typedef std::shared_ptr<MyTemplate<A>> Shared;
  std::shared_ptr<void> *asVoid = get_handle<std::shared_ptr<void>>(in[0]);
  Shared *self = new Shared(std::static_pointer_cast<MyTemplate<A>>(*asVoid));
  collector_MyTemplateA.insert(self);
  out[0] = make_handle<Shared>(self);
}

void MyTemplateA_constructor_37(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MyTemplate<A>> Shared;

  Shared *self = new Shared(new MyTemplate<A>());
  collector_MyTemplateA.insert(self);
  out[0] = make_handle<Shared>(self);

  typedef std::shared_ptr<MyBase> SharedBase;
  out[1] = make_handle<SharedBase>(new SharedBase(*self));
}

void MyTemplateA_deconstructor_38(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<MyTemplate<A>> Shared;
  checkArguments("delete_MyTemplateA",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_MyTemplateA::iterator item;
  item = collector_MyTemplateA.find(self);
  if(item == collector_MyTemplateA.end()) {
    return;
  }
  collector_MyTemplateA.erase(item);
  delete self;
}

void MyTemplateA_accept_T_39(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("accept_T",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<A>>(ctx, in[0], "ptr_MyTemplateA");
  A& value = *unwrap_shared_ptr< A >(ctx, in[1], "ptr_A");
  obj->accept_T(value);
}

void MyTemplateA_accept_Tptr_40(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("accept_Tptr",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<A>>(ctx, in[0], "ptr_MyTemplateA");
  std::shared_ptr<A> value = unwrap_shared_ptr< A >(ctx, in[1], "ptr_A");
  obj->accept_Tptr(value);
}

void MyTemplateA_create_MixedPtrs_41(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("create_MixedPtrs",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<MyTemplate<A>>(ctx, in[0], "ptr_MyTemplateA");
  auto pairResult = obj->create_MixedPtrs();
  out[0] = wrap_shared_ptr(ctx, std::make_shared<A>(pairResult.first),"A", false);
  out[1] = wrap_shared_ptr(ctx, pairResult.second,"A", false);
}

void MyTemplateA_create_ptrs_42(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("create_ptrs",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<MyTemplate<A>>(ctx, in[0], "ptr_MyTemplateA");
  auto pairResult = obj->create_ptrs();
  out[0] = wrap_shared_ptr(ctx, pairResult.first,"A", false);
  out[1] = wrap_shared_ptr(ctx, pairResult.second,"A", false);
}

void MyTemplateA_return_T_43(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_T",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<A>>(ctx, in[0], "ptr_MyTemplateA");
  A* value = unwrap_ptr< A >(ctx, in[1], "ptr_A");
  out[0] = wrap_shared_ptr(ctx, std::make_shared<A>(obj->return_T(value)),"A", false);
}

void MyTemplateA_return_Tptr_44(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_Tptr",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<A>>(ctx, in[0], "ptr_MyTemplateA");
  std::shared_ptr<A> value = unwrap_shared_ptr< A >(ctx, in[1], "ptr_A");
  out[0] = wrap_shared_ptr(ctx, obj->return_Tptr(value),"A", false);
}

void MyTemplateA_return_ptrs_45(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("return_ptrs",nargout,nargin-1,2);
  auto obj = unwrap_shared_ptr<MyTemplate<A>>(ctx, in[0], "ptr_MyTemplateA");
  std::shared_ptr<A> p1 = unwrap_shared_ptr< A >(ctx, in[1], "ptr_A");
  std::shared_ptr<A> p2 = unwrap_shared_ptr< A >(ctx, in[2], "ptr_A");
  auto pairResult = obj->return_ptrs(p1,p2);
  out[0] = wrap_shared_ptr(ctx, pairResult.first,"A", false);
  out[1] = wrap_shared_ptr(ctx, pairResult.second,"A", false);
}

void MyTemplateA_templatedMethod_46(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("templatedMethodMatrix",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<A>>(ctx, in[0], "ptr_MyTemplateA");
  Matrix t = unwrap< Matrix >(in[1]);
  out[0] = wrap< Matrix >(obj->templatedMethod<gtsam::Matrix>(t));
}

void MyTemplateA_templatedMethod_47(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("templatedMethodPoint2",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<A>>(ctx, in[0], "ptr_MyTemplateA");
  Point2 t = unwrap< Point2 >(in[1]);
  out[0] = wrap< Point2 >(obj->templatedMethod<gtsam::Point2>(t));
}

void MyTemplateA_templatedMethod_48(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("templatedMethodPoint3",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<A>>(ctx, in[0], "ptr_MyTemplateA");
  Point3 t = unwrap< Point3 >(in[1]);
  out[0] = wrap< Point3 >(obj->templatedMethod<gtsam::Point3>(t));
}

void MyTemplateA_templatedMethod_49(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("templatedMethodVector",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<MyTemplate<A>>(ctx, in[0], "ptr_MyTemplateA");
  Vector t = unwrap< Vector >(in[1]);
  out[0] = wrap< Vector >(obj->templatedMethod<gtsam::Vector>(t));
}

void MyTemplateA_Level_50(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("MyTemplate<A>.Level",nargout,nargin,1);
  A& K = *unwrap_shared_ptr< A >(ctx, in[0], "ptr_A");
  out[0] = wrap_shared_ptr(ctx, std::make_shared<MyTemplate<A>>(MyTemplate<A>::Level(K)),"MyTemplateA", false);
}

void ForwardKinematicsFactor_collectorInsertAndMakeBase_51(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ForwardKinematicsFactor> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_ForwardKinematicsFactor.insert(self);

  typedef std::shared_ptr<gtsam::BetweenFactor<gtsam::Pose3>> SharedBase;
  out[0] = make_handle<SharedBase>(new SharedBase(*self));
}

void ForwardKinematicsFactor_upcastFromVoid_52(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in) {
  typedef std::shared_ptr<ForwardKinematicsFactor> Shared;
  std::shared_ptr<void> *asVoid = get_handle<std::shared_ptr<void>>(in[0]);
  Shared *self = new Shared(std::static_pointer_cast<ForwardKinematicsFactor>(*asVoid));
  collector_ForwardKinematicsFactor.insert(self);
  out[0] = make_handle<Shared>(self);
}

void ForwardKinematicsFactor_deconstructor_53(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ForwardKinematicsFactor> Shared;
  checkArguments("delete_ForwardKinematicsFactor",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_ForwardKinematicsFactor::iterator item;
  item = collector_ForwardKinematicsFactor.find(self);
  if(item == collector_ForwardKinematicsFactor.end()) {
    return;
  }
  collector_ForwardKinematicsFactor.erase(item);
  delete self;
}

void ParentHasTemplateDouble_collectorInsertAndMakeBase_54(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ParentHasTemplate<double>> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_ParentHasTemplateDouble.insert(self);

  typedef std::shared_ptr<MyTemplate<double>> SharedBase;
  out[0] = make_handle<SharedBase>(new SharedBase(*self));
}

void ParentHasTemplateDouble_upcastFromVoid_55(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in) {
  typedef std::shared_ptr<ParentHasTemplate<double>> Shared;
  std::shared_ptr<void> *asVoid = get_handle<std::shared_ptr<void>>(in[0]);
  Shared *self = new Shared(std::static_pointer_cast<ParentHasTemplate<double>>(*asVoid));
  collector_ParentHasTemplateDouble.insert(self);
  out[0] = make_handle<Shared>(self);
}

void ParentHasTemplateDouble_deconstructor_56(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<ParentHasTemplate<double>> Shared;
  checkArguments("delete_ParentHasTemplateDouble",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_ParentHasTemplateDouble::iterator item;
  item = collector_ParentHasTemplateDouble.find(self);
  if(item == collector_ParentHasTemplateDouble.end()) {
    return;
  }
  collector_ParentHasTemplateDouble.erase(item);
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
      _inheritance_RTTIRegister(ctx.engine);
      rttiRegistered_ = true;
    }

    gtwrap::CoutRedirect coutRedirect(ctx.engine);

    int id = unwrap<int>(inputs[0]);
    WrapIn in(inputs, 1);
    OutputList out;

    try {
      switch(id) {
        case 0:
          MyBase_collectorInsertAndMakeBase_0(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 1:
          MyBase_upcastFromVoid_1(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 2:
          MyBase_deconstructor_2(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 3:
          MyTemplatePoint2_collectorInsertAndMakeBase_3(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 4:
          MyTemplatePoint2_upcastFromVoid_4(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 5:
          MyTemplatePoint2_constructor_5(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 6:
          MyTemplatePoint2_deconstructor_6(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 7:
          MyTemplatePoint2_accept_T_7(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 8:
          MyTemplatePoint2_accept_Tptr_8(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 9:
          MyTemplatePoint2_create_MixedPtrs_9(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 10:
          MyTemplatePoint2_create_ptrs_10(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 11:
          MyTemplatePoint2_return_T_11(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 12:
          MyTemplatePoint2_return_Tptr_12(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 13:
          MyTemplatePoint2_return_ptrs_13(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 14:
          MyTemplatePoint2_templatedMethod_14(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 15:
          MyTemplatePoint2_templatedMethod_15(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 16:
          MyTemplatePoint2_templatedMethod_16(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 17:
          MyTemplatePoint2_templatedMethod_17(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 18:
          MyTemplatePoint2_Level_18(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 19:
          MyTemplateMatrix_collectorInsertAndMakeBase_19(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 20:
          MyTemplateMatrix_upcastFromVoid_20(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 21:
          MyTemplateMatrix_constructor_21(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 22:
          MyTemplateMatrix_deconstructor_22(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 23:
          MyTemplateMatrix_accept_T_23(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 24:
          MyTemplateMatrix_accept_Tptr_24(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 25:
          MyTemplateMatrix_create_MixedPtrs_25(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 26:
          MyTemplateMatrix_create_ptrs_26(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 27:
          MyTemplateMatrix_return_T_27(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 28:
          MyTemplateMatrix_return_Tptr_28(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 29:
          MyTemplateMatrix_return_ptrs_29(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 30:
          MyTemplateMatrix_templatedMethod_30(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 31:
          MyTemplateMatrix_templatedMethod_31(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 32:
          MyTemplateMatrix_templatedMethod_32(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 33:
          MyTemplateMatrix_templatedMethod_33(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 34:
          MyTemplateMatrix_Level_34(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 35:
          MyTemplateA_collectorInsertAndMakeBase_35(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 36:
          MyTemplateA_upcastFromVoid_36(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 37:
          MyTemplateA_constructor_37(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 38:
          MyTemplateA_deconstructor_38(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 39:
          MyTemplateA_accept_T_39(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 40:
          MyTemplateA_accept_Tptr_40(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 41:
          MyTemplateA_create_MixedPtrs_41(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 42:
          MyTemplateA_create_ptrs_42(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 43:
          MyTemplateA_return_T_43(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 44:
          MyTemplateA_return_Tptr_44(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 45:
          MyTemplateA_return_ptrs_45(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 46:
          MyTemplateA_templatedMethod_46(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 47:
          MyTemplateA_templatedMethod_47(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 48:
          MyTemplateA_templatedMethod_48(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 49:
          MyTemplateA_templatedMethod_49(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 50:
          MyTemplateA_Level_50(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 51:
          ForwardKinematicsFactor_collectorInsertAndMakeBase_51(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 52:
          ForwardKinematicsFactor_upcastFromVoid_52(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 53:
          ForwardKinematicsFactor_deconstructor_53(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 54:
          ParentHasTemplateDouble_collectorInsertAndMakeBase_54(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 55:
          ParentHasTemplateDouble_upcastFromVoid_55(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 56:
          ParentHasTemplateDouble_deconstructor_56(ctx, (int)outputs.size(), out, (int)in.size(), in);
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
