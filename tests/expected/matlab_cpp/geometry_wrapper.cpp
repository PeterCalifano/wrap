#include <wrap/matlab_cpp.h>
#include <map>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/export.hpp>

#include <gtsam/geometry/Point2.h>
#include <gtsam/geometry/Point3.h>


BOOST_CLASS_EXPORT_GUID(gtsam::Point2, "gtsamPoint2");
BOOST_CLASS_EXPORT_GUID(gtsam::Point3, "gtsamPoint3");

typedef std::set<std::shared_ptr<gtsam::Point2>*> Collector_gtsamPoint2;
static Collector_gtsamPoint2 collector_gtsamPoint2;
typedef std::set<std::shared_ptr<gtsam::Point3>*> Collector_gtsamPoint3;
static Collector_gtsamPoint3 collector_gtsamPoint3;


void _deleteAllObjects(matlab::engine::MATLABEngine* engine)
{
  bool anyDeleted = false;
  { for(Collector_gtsamPoint2::iterator iter = collector_gtsamPoint2.begin();
      iter != collector_gtsamPoint2.end(); ) {
    delete *iter;
    collector_gtsamPoint2.erase(iter++);
    anyDeleted = true;
  } }
  { for(Collector_gtsamPoint3::iterator iter = collector_gtsamPoint3.begin();
      iter != collector_gtsamPoint3.end(); ) {
    delete *iter;
    collector_gtsamPoint3.erase(iter++);
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

void _geometry_RTTIRegister(matlab::engine::MATLABEngine* engine) {
  bool alreadyCreated = false;
  try {
    engine->getVariable(u"gtsam_geometry_rttiRegistry_created",
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
    engine->setVariable(u"gtsam_geometry_rttiRegistry_created",
                        f_.createScalar<std::int8_t>(0),
                        matlab::engine::WorkspaceType::GLOBAL);
  }
}

void gtsamPoint2_collectorInsertAndMakeBase_0(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Point2> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_gtsamPoint2.insert(self);
}

void gtsamPoint2_constructor_1(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Point2> Shared;

  Shared *self = new Shared(new gtsam::Point2());
  collector_gtsamPoint2.insert(self);
  out[0] = make_handle<Shared>(self);
}

void gtsamPoint2_constructor_2(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Point2> Shared;

  double x = unwrap< double >(in[0]);
  double y = unwrap< double >(in[1]);
  Shared *self = new Shared(new gtsam::Point2(x,y));
  collector_gtsamPoint2.insert(self);
  out[0] = make_handle<Shared>(self);
}

void gtsamPoint2_deconstructor_3(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Point2> Shared;
  checkArguments("delete_gtsamPoint2",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_gtsamPoint2::iterator item;
  item = collector_gtsamPoint2.find(self);
  if(item == collector_gtsamPoint2.end()) {
    return;
  }
  collector_gtsamPoint2.erase(item);
  delete self;
}

void gtsamPoint2_argChar_4(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("argChar",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<gtsam::Point2>(ctx, in[0], "ptr_gtsamPoint2");
  char a = unwrap< char >(in[1]);
  obj->argChar(a);
}

void gtsamPoint2_argChar_5(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("argChar",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<gtsam::Point2>(ctx, in[0], "ptr_gtsamPoint2");
  std::shared_ptr<char> a = unwrap_shared_ptr< char >(ctx, in[1], "ptr_char");
  obj->argChar(a);
}

void gtsamPoint2_argChar_6(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("argChar",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<gtsam::Point2>(ctx, in[0], "ptr_gtsamPoint2");
  char a = unwrap< char >(in[1]);
  obj->argChar(a);
}

void gtsamPoint2_argChar_7(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("argChar",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<gtsam::Point2>(ctx, in[0], "ptr_gtsamPoint2");
  char* a = unwrap_ptr< char >(ctx, in[1], "ptr_char");
  obj->argChar(a);
}

void gtsamPoint2_argChar_8(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("argChar",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<gtsam::Point2>(ctx, in[0], "ptr_gtsamPoint2");
  std::shared_ptr<char> a = unwrap_shared_ptr< char >(ctx, in[1], "ptr_char");
  obj->argChar(a);
}

void gtsamPoint2_argChar_9(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("argChar",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<gtsam::Point2>(ctx, in[0], "ptr_gtsamPoint2");
  char a = unwrap< char >(in[1]);
  obj->argChar(a);
}

void gtsamPoint2_argChar_10(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("argChar",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<gtsam::Point2>(ctx, in[0], "ptr_gtsamPoint2");
  char* a = unwrap_ptr< char >(ctx, in[1], "ptr_char");
  obj->argChar(a);
}

void gtsamPoint2_argUChar_11(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("argUChar",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<gtsam::Point2>(ctx, in[0], "ptr_gtsamPoint2");
  unsigned char a = unwrap< unsigned char >(in[1]);
  obj->argUChar(a);
}

void gtsamPoint2_dim_12(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("dim",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<gtsam::Point2>(ctx, in[0], "ptr_gtsamPoint2");
  out[0] = wrap< int >(obj->dim());
}

void gtsamPoint2_eigenArguments_13(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("eigenArguments",nargout,nargin-1,2);
  auto obj = unwrap_shared_ptr<gtsam::Point2>(ctx, in[0], "ptr_gtsamPoint2");
  Vector v = unwrap< Vector >(in[1]);
  Matrix m = unwrap< Matrix >(in[2]);
  obj->eigenArguments(v,m);
}

void gtsamPoint2_returnChar_14(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("returnChar",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<gtsam::Point2>(ctx, in[0], "ptr_gtsamPoint2");
  out[0] = wrap< char >(obj->returnChar());
}

void gtsamPoint2_vectorConfusion_15(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("vectorConfusion",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<gtsam::Point2>(ctx, in[0], "ptr_gtsamPoint2");
  out[0] = wrap_shared_ptr(ctx, std::make_shared<VectorNotEigen>(obj->vectorConfusion()),"VectorNotEigen", false);
}

void gtsamPoint2_x_16(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("x",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<gtsam::Point2>(ctx, in[0], "ptr_gtsamPoint2");
  out[0] = wrap< double >(obj->x());
}

void gtsamPoint2_y_17(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("y",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<gtsam::Point2>(ctx, in[0], "ptr_gtsamPoint2");
  out[0] = wrap< double >(obj->y());
}

void gtsamPoint3_collectorInsertAndMakeBase_18(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Point3> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_gtsamPoint3.insert(self);
}

void gtsamPoint3_constructor_19(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Point3> Shared;

  double x = unwrap< double >(in[0]);
  double y = unwrap< double >(in[1]);
  double z = unwrap< double >(in[2]);
  Shared *self = new Shared(new gtsam::Point3(x,y,z));
  collector_gtsamPoint3.insert(self);
  out[0] = make_handle<Shared>(self);
}

void gtsamPoint3_deconstructor_20(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Point3> Shared;
  checkArguments("delete_gtsamPoint3",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_gtsamPoint3::iterator item;
  item = collector_gtsamPoint3.find(self);
  if(item == collector_gtsamPoint3.end()) {
    return;
  }
  collector_gtsamPoint3.erase(item);
  delete self;
}

void gtsamPoint3_norm_21(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("norm",nargout,nargin-1,0);
  auto obj = unwrap_shared_ptr<gtsam::Point3>(ctx, in[0], "ptr_gtsamPoint3");
  out[0] = wrap< double >(obj->norm());
}

void gtsamPoint3_string_serialize_22(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Point3> Shared;
  checkArguments("string_serialize",nargout,nargin-1,0);
  Shared obj = unwrap_shared_ptr<gtsam::Point3>(ctx, in[0], "ptr_gtsamPoint3");
  ostringstream out_archive_stream;
  boost::archive::text_oarchive out_archive(out_archive_stream);
  out_archive << *obj;
  out[0] = wrap< string >(out_archive_stream.str());
}
void gtsamPoint3_StaticFunctionRet_23(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("gtsam::Point3.StaticFunctionRet",nargout,nargin,1);
  double z = unwrap< double >(in[0]);
  out[0] = wrap< Point3 >(gtsam::Point3::StaticFunctionRet(z));
}

void gtsamPoint3_staticFunction_24(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("gtsam::Point3.staticFunction",nargout,nargin,0);
  out[0] = wrap< double >(gtsam::Point3::staticFunction());
}

void gtsamPoint3_string_deserialize_25(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::Point3> Shared;
  checkArguments("gtsamPoint3.string_deserialize",nargout,nargin,1);
  string serialized = unwrap< string >(in[0]);
  istringstream in_archive_stream(serialized);
  boost::archive::text_iarchive in_archive(in_archive_stream);
  Shared output(new gtsam::Point3());
  in_archive >> *output;
  out[0] = wrap_shared_ptr(ctx, output,"gtsam.Point3", false);
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
      _geometry_RTTIRegister(ctx.engine);
      rttiRegistered_ = true;
    }

    gtwrap::CoutRedirect coutRedirect(ctx.engine);

    int id = unwrap<int>(inputs[0]);
    WrapIn in(inputs, 1);
    OutputList out;

    try {
      switch(id) {
        case 0:
          gtsamPoint2_collectorInsertAndMakeBase_0(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 1:
          gtsamPoint2_constructor_1(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 2:
          gtsamPoint2_constructor_2(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 3:
          gtsamPoint2_deconstructor_3(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 4:
          gtsamPoint2_argChar_4(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 5:
          gtsamPoint2_argChar_5(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 6:
          gtsamPoint2_argChar_6(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 7:
          gtsamPoint2_argChar_7(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 8:
          gtsamPoint2_argChar_8(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 9:
          gtsamPoint2_argChar_9(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 10:
          gtsamPoint2_argChar_10(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 11:
          gtsamPoint2_argUChar_11(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 12:
          gtsamPoint2_dim_12(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 13:
          gtsamPoint2_eigenArguments_13(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 14:
          gtsamPoint2_returnChar_14(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 15:
          gtsamPoint2_vectorConfusion_15(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 16:
          gtsamPoint2_x_16(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 17:
          gtsamPoint2_y_17(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 18:
          gtsamPoint3_collectorInsertAndMakeBase_18(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 19:
          gtsamPoint3_constructor_19(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 20:
          gtsamPoint3_deconstructor_20(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 21:
          gtsamPoint3_norm_21(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 22:
          gtsamPoint3_string_serialize_22(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 23:
          gtsamPoint3_StaticFunctionRet_23(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 24:
          gtsamPoint3_staticFunction_24(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 25:
          gtsamPoint3_string_deserialize_25(ctx, (int)outputs.size(), out, (int)in.size(), in);
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
