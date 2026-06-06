#include <wrap/matlab_cpp.h>
#include <map>

#include <folder/path/to/MatrixViewFixture.h>



typedef std::set<std::shared_ptr<gtsam::MatrixViewFixture>*> Collector_gtsamMatrixViewFixture;
static Collector_gtsamMatrixViewFixture collector_gtsamMatrixViewFixture;


void _deleteAllObjects(matlab::engine::MATLABEngine* engine)
{
  bool anyDeleted = false;
  { for(Collector_gtsamMatrixViewFixture::iterator iter = collector_gtsamMatrixViewFixture.begin();
      iter != collector_gtsamMatrixViewFixture.end(); ) {
    delete *iter;
    collector_gtsamMatrixViewFixture.erase(iter++);
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

void _matrix_views_RTTIRegister(matlab::engine::MATLABEngine* engine) {
  bool alreadyCreated = false;
  try {
    engine->getVariable(u"gtsam_matrix_views_rttiRegistry_created",
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
    engine->setVariable(u"gtsam_matrix_views_rttiRegistry_created",
                        f_.createScalar<std::int8_t>(0),
                        matlab::engine::WorkspaceType::GLOBAL);
  }
}

void gtsamMatrixViewFixture_collectorInsertAndMakeBase_0(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::MatrixViewFixture> Shared;

  Shared *self = get_handle<Shared>(in[0]);
  collector_gtsamMatrixViewFixture.insert(self);
}

void gtsamMatrixViewFixture_constructor_1(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::MatrixViewFixture> Shared;

  Shared *self = new Shared(new gtsam::MatrixViewFixture());
  collector_gtsamMatrixViewFixture.insert(self);
  out[0] = make_handle<Shared>(self);
}

void gtsamMatrixViewFixture_deconstructor_2(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  typedef std::shared_ptr<gtsam::MatrixViewFixture> Shared;
  checkArguments("delete_gtsamMatrixViewFixture",nargout,nargin,1);
  Shared *self = get_handle<Shared>(in[0]);
  Collector_gtsamMatrixViewFixture::iterator item;
  item = collector_gtsamMatrixViewFixture.find(self);
  if(item == collector_gtsamMatrixViewFixture.end()) {
    return;
  }
  collector_gtsamMatrixViewFixture.erase(item);
  delete self;
}

void gtsamMatrixViewFixture_acceptView_3(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("acceptView",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<gtsam::MatrixViewFixture>(ctx, in[0], "ptr_gtsamMatrixViewFixture");
  gtsam::ConstMatrixView points = unwrapMatrixView< gtsam::ConstMatrixView >(in[1]);
  obj->acceptView(points);
}

void gtsamMatrixViewFixture_scaleView_4(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("scaleView",nargout,nargin-1,2);
  auto obj = unwrap_shared_ptr<gtsam::MatrixViewFixture>(ctx, in[0], "ptr_gtsamMatrixViewFixture");
  gtsam::ConstMatrixView points = unwrapMatrixView< gtsam::ConstMatrixView >(in[1]);
  double scale = unwrap< double >(in[2]);
  out[0] = wrap< Matrix >(obj->scaleView(points,scale));
}

void gtsamMatrixViewFixture_scaleView_5(Context& ctx, int nargout, OutputList& out, int nargin, WrapIn in)
{
  checkArguments("scaleView",nargout,nargin-1,1);
  auto obj = unwrap_shared_ptr<gtsam::MatrixViewFixture>(ctx, in[0], "ptr_gtsamMatrixViewFixture");
  gtsam::ConstMatrixView points = unwrapMatrixView< gtsam::ConstMatrixView >(in[1]);
  out[0] = wrap< Matrix >(obj->scaleView(points,1.0));
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
      _matrix_views_RTTIRegister(ctx.engine);
      rttiRegistered_ = true;
    }

    gtwrap::CoutRedirect coutRedirect(ctx.engine);

    int id = unwrap<int>(inputs[0]);
    WrapIn in(inputs, 1);
    OutputList out;

    try {
      switch(id) {
        case 0:
          gtsamMatrixViewFixture_collectorInsertAndMakeBase_0(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 1:
          gtsamMatrixViewFixture_constructor_1(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 2:
          gtsamMatrixViewFixture_deconstructor_2(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 3:
          gtsamMatrixViewFixture_acceptView_3(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 4:
          gtsamMatrixViewFixture_scaleView_4(ctx, (int)outputs.size(), out, (int)in.size(), in);
          break;
        case 5:
          gtsamMatrixViewFixture_scaleView_5(ctx, (int)outputs.size(), out, (int)in.size(), in);
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
