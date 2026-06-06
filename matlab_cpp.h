/**
 * @file matlab_cpp.h
 * @author PeterC (petercalifano.gs@gmail.com); Claude Opus 4.8, Codex GPT 5.5
 * @brief Runtime header for MATLAB wrappers generated against the modern
 *        C++ MEX API (matlab::data / matlab::engine, R2021b+).
 *
 * The C++ MEX API counterpart of matlab.h. The wrap/unwrap template
 * mechanism is identical in spirit, but every value crossing the MATLAB
 * boundary is a reference-counted matlab::data::Array instead of a raw
 * mxArray*, and all interpreter calls go through the MATLABEngine handle
 * obtained from the matlab::mex::Function object.
 *
 * The generated *_wrapper.cpp includes this header and "mexAdapter.hpp"
 * (the latter once, after the MexFunction class definition).
 * @version 0.1
 * @date 2026-06-06
 */

#pragma once

#include <gtsam/base/Matrix.h>
#include <gtsam/base/Vector.h>
#include <gtsam/base/utilities.h>
#include <gtsam/geometry/Point2.h>
#include <gtsam/geometry/Point3.h>

using gtsam::Matrix;
using gtsam::Point2;
using gtsam::Point3;
using gtsam::Vector;

#include "MatlabDataArray.hpp"
#include "mex.hpp"
// mexAdapter.hpp provides the MEX entry-point glue and, on some releases
// (e.g. R2023b), the complete matlab::mex::ArgumentList type needed to *define*
// MexFunction::operator(). It must be included before the MexFunction class.
// It references MexFunction via deferred template instantiation, so the class
// may be defined later in the same (single) translation unit. Including it here
// means each generated wrapper - the sole includer of this header - pulls it
// exactly once.
#include "mexAdapter.hpp"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <streambuf>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>

using namespace std;

// start GTSAM Specifics /////////////////////////////////////////////////
// to enable Matrix and Vector constructor for SharedGaussian:
#define GTSAM_MAGIC_GAUSSIAN
// end GTSAM Specifics /////////////////////////////////////////////////

// "Unique" key to signal calling the matlab object constructor with a raw pointer to a shared pointer of the same C++ object type as the MATLAB type. MUST stay byte-identical to matlab.h and utilities.h.
static const std::uint64_t ptr_constructor_key =
    (std::uint64_t('G') << 56) |
    (std::uint64_t('T') << 48) |
    (std::uint64_t('S') << 40) |
    (std::uint64_t('A') << 32) |
    (std::uint64_t('M') << 24) |
    (std::uint64_t('p') << 16) |
    (std::uint64_t('t') << 8) |
    (std::uint64_t('r'));

//*****************************************************************************
// Per-call context + argument shims
//*****************************************************************************

// Per-call context: the active interpreter plus a factory for building Arrays.
// Threaded explicitly into the helpers that talk to MATLAB, so there is no
// process-global engine state. The engine pointer is owned by the live
// MexFunction object and stays valid for the duration of a call.
struct Context
{
    matlab::engine::MATLABEngine *engine;
    matlab::data::ArrayFactory factory;
};

// "out" replacement: a growable output list so generated bodies can assign
// out[0], out[1], ... without knowing the count up front (the C MEX API
// pre-sized out[] for them).
struct OutputList
{
    std::vector<matlab::data::Array> v;
    matlab::data::Array &operator[](size_t i)
    {
        if (i >= v.size())
            v.resize(i + 1);
        return v[i];
    }
};

// "in" replacement: a read-only, offset view of the input arguments. Generated
// collectors use in[0], in[1], ... exactly like the C API path, while the view
// skips the dispatch id without copying the remaining matlab::data::Array
// handles into a new vector on every call.
class WrapIn
{
  public:
    WrapIn(matlab::mex::ArgumentList &inputs, size_t offset)
        : inputs_(inputs), offset_(offset) {}

    decltype(auto) operator[](size_t i) { return inputs_[i + offset_]; }

    size_t size()
    {
        return inputs_.size() > offset_ ? inputs_.size() - offset_ : 0;
    }

  private:
    matlab::mex::ArgumentList &inputs_;
    size_t offset_;
};

//*****************************************************************************
// Console redirection + error reporting
//*****************************************************************************

// Replacement streambuf for cout that writes to the MATLAB console via fprintf.
class mstream : public std::streambuf
{
  public:
    explicit mstream(matlab::engine::MATLABEngine *engine) : engine_(engine) {}

  protected:
    std::streamsize xsputn(const char *s, std::streamsize n) override
    {
        print(std::string(s, s + n));
        return n;
    }
    int overflow(int c = EOF) override
    {
        if (c != EOF)
        {
            char ch = static_cast<char>(c);
            print(std::string(&ch, 1));
        }
        return 1;
    }

  private:
    void print(const std::string &text)
    {
        matlab::data::ArrayFactory f;
        engine_->feval(
            u"fprintf", 0,
            std::vector<matlab::data::Array>({f.createScalar<double>(1.0),
                                              f.createCharArray("%s"),
                                              f.createCharArray(text)}));
    }

    matlab::engine::MATLABEngine *engine_;
};

namespace gtwrap
{

    class CoutRedirect
    {
      public:
        explicit CoutRedirect(matlab::engine::MATLABEngine *engine)
            : mout_(engine), outbuf_(std::cout.rdbuf(&mout_)),
              previous_(activeRedirect())
        {
            activeRedirect() = this;
        }

        ~CoutRedirect() { restore(); }

        void restore()
        {
            if (outbuf_)
            {
                std::cout.rdbuf(outbuf_);
                outbuf_ = nullptr;
                if (activeRedirect() == this)
                {
                    activeRedirect() = previous_;
                }
            }
        }

        static void restoreActive()
        {
            while (activeRedirect())
            {
                activeRedirect()->restore();
            }
        }

      private:
        static CoutRedirect *&activeRedirect()
        {
            static CoutRedirect *active = nullptr;
            return active;
        }

        mstream mout_;
        std::streambuf *outbuf_;
        CoutRedirect *previous_;
    };

    // Restore cout, then raise a MATLAB error by throwing a MATLABException. The
    // MEX framework reports it to MATLAB as an MException carrying both the
    // identifier and the message - no round-trip through feval("error").
    [[noreturn]] inline void MexErrMsgIdAndTxt(const char *id, const char *str)
    {
        CoutRedirect::restoreActive();
        throw matlab::engine::MATLABException(
            id, matlab::engine::convertUTF8StringToUTF16String(std::string(str)));
    }

    [[noreturn]] inline void MexErrMsgTxt(const char *str)
    {
        MexErrMsgIdAndTxt("wrap:error", str);
    }

} // namespace gtwrap

[[noreturn]] inline void error(const char *str)
{
    gtwrap::MexErrMsgIdAndTxt("wrap:error", str);
}

//*****************************************************************************
// Argument checking
//*****************************************************************************

// Human-readable name for a MATLAB array type, used in mismatch messages.
inline const char *typeName(matlab::data::ArrayType t)
{
    using matlab::data::ArrayType;
    switch (t)
    {
    case ArrayType::DOUBLE:
        return "double";
    case ArrayType::SINGLE:
        return "single";
    case ArrayType::INT8:
        return "int8";
    case ArrayType::UINT8:
        return "uint8";
    case ArrayType::INT16:
        return "int16";
    case ArrayType::UINT16:
        return "uint16";
    case ArrayType::INT32:
        return "int32";
    case ArrayType::UINT32:
        return "uint32";
    case ArrayType::INT64:
        return "int64";
    case ArrayType::UINT64:
        return "uint64";
    case ArrayType::LOGICAL:
        return "logical";
    case ArrayType::CHAR:
        return "char";
    case ArrayType::COMPLEX_DOUBLE:
        return "complex double";
    case ArrayType::CELL:
        return "cell";
    case ArrayType::STRUCT:
        return "struct";
    case ArrayType::VALUE_OBJECT:
    case ArrayType::HANDLE_OBJECT_REF:
        return "object";
    default:
        return "unsupported type";
    }
}

// Describe an array as "<rows>x<cols> <type>" for diagnostics.
inline std::string describe(const matlab::data::Array &array)
{
    const auto dims = array.getDimensions();
    std::stringstream s;
    for (size_t i = 0; i < dims.size(); ++i)
        s << (i ? "x" : "") << dims[i];
    s << " " << typeName(array.getType());
    return s.str();
}

inline void checkScalar(const matlab::data::Array &array, const char *str)
{
    if (array.getNumberOfElements() != 1)
        gtwrap::MexErrMsgIdAndTxt(
            "wrap:notAScalar",
            (std::string(str) + ": expected a scalar, got a " + describe(array) +
             " array")
                .c_str());
}

inline void checkArguments(const string &name, int nargout, int nargin,
                           int expected)
{
    (void)nargout;
    if (nargin != expected)
    {
        stringstream err;
        err << name << " expects " << expected << " arguments, not " << nargin;
        error(err.str().c_str());
    }
}

//*****************************************************************************
// Object handles (uint64 scalar carrying a heap pointer)
//*****************************************************************************

template <typename Shared>
matlab::data::Array make_handle(Shared *self)
{
    matlab::data::ArrayFactory f;
    return f.createScalar<std::uint64_t>(reinterpret_cast<std::uint64_t>(self));
}

template <typename Shared>
Shared *get_handle(const matlab::data::Array &array)
{
    matlab::data::TypedArray<std::uint64_t> handle(array);
    return reinterpret_cast<Shared *>(static_cast<std::uint64_t>(handle[0]));
}

//*****************************************************************************
// Scalar extraction (type-aware, mirrors matlab.h::myGetScalar)
//*****************************************************************************

template <typename T>
T myGetScalar(const matlab::data::Array &array)
{
    using matlab::data::ArrayType;
    using matlab::data::TypedArray;
    switch (array.getType())
    {
    case ArrayType::DOUBLE:
        return static_cast<T>(double(TypedArray<double>(array)[0]));
    case ArrayType::SINGLE:
        return static_cast<T>(float(TypedArray<float>(array)[0]));
    case ArrayType::INT8:
        return static_cast<T>(TypedArray<std::int8_t>(array)[0]);
    case ArrayType::UINT8:
        return static_cast<T>(TypedArray<std::uint8_t>(array)[0]);
    case ArrayType::INT16:
        return static_cast<T>(TypedArray<std::int16_t>(array)[0]);
    case ArrayType::UINT16:
        return static_cast<T>(TypedArray<std::uint16_t>(array)[0]);
    case ArrayType::INT32:
        return static_cast<T>(TypedArray<std::int32_t>(array)[0]);
    case ArrayType::UINT32:
        return static_cast<T>(TypedArray<std::uint32_t>(array)[0]);
    case ArrayType::INT64:
        return static_cast<T>(std::int64_t(TypedArray<std::int64_t>(array)[0]));
    case ArrayType::UINT64:
        return static_cast<T>(std::uint64_t(TypedArray<std::uint64_t>(array)[0]));
    case ArrayType::LOGICAL:
        return static_cast<T>(bool(TypedArray<bool>(array)[0]));
    default:
        error((std::string("unwrap<scalar>: expected a numeric/logical value, "
                           "got a ") +
               typeName(array.getType()) + " array")
                  .c_str());
        return T();
    }
}

//*****************************************************************************
// wrapping C++ basic types into MATLAB arrays
//*****************************************************************************

template <typename Class>
matlab::data::Array wrap(const Class &value)
{
    if constexpr (std::is_integral<Class>::value)
    {
        matlab::data::ArrayFactory f;
        return f.createScalar<Class>(value);
    }
    else
    {
        error("wrap internal error: attempted wrap of invalid type");
        return matlab::data::Array();
    }
}

template <>
inline matlab::data::Array wrap<string>(const string &value)
{
    matlab::data::ArrayFactory f;
    return f.createCharArray(value);
}

template <>
inline matlab::data::Array wrap<char>(const char &value)
{
    matlab::data::ArrayFactory f;
    return f.createScalar<std::int8_t>(static_cast<std::int8_t>(value));
}

template <>
inline matlab::data::Array wrap<unsigned char>(const unsigned char &value)
{
    matlab::data::ArrayFactory f;
    return f.createScalar<std::uint8_t>(static_cast<std::uint8_t>(value));
}

template <>
inline matlab::data::Array wrap<bool>(const bool &value)
{
    matlab::data::ArrayFactory f;
    return f.createScalar<bool>(value);
}

template <>
inline matlab::data::Array wrap<size_t>(const size_t &value)
{
    matlab::data::ArrayFactory f;
    return f.createScalar<std::uint64_t>(static_cast<std::uint64_t>(value));
}

template <>
inline matlab::data::Array wrap<int>(const int &value)
{
    matlab::data::ArrayFactory f;
    return f.createScalar<int>(value);
}

template <>
inline matlab::data::Array wrap<double>(const double &value)
{
    matlab::data::ArrayFactory f;
    return f.createScalar<double>(value);
}

// Eigen vector -> column double array. Eigen is column-major, so is MATLAB.
inline matlab::data::Array wrap_Vector(const gtsam::Vector &v)
{
    matlab::data::ArrayFactory f;
    const size_t m = static_cast<size_t>(v.size());
    return f.createArray<double>({m, 1}, v.data(), v.data() + m);
}

template <>
inline matlab::data::Array wrap<gtsam::Vector>(const gtsam::Vector &v)
{
    return wrap_Vector(v);
}

template <>
inline matlab::data::Array wrap<gtsam::Point2>(const gtsam::Point2 &v)
{
    return wrap_Vector(v);
}

template <>
inline matlab::data::Array wrap<gtsam::Point3>(const gtsam::Point3 &v)
{
    return wrap_Vector(v);
}

// Eigen matrix -> double matrix. Both column-major: copy storage directly.
inline matlab::data::Array wrap_Matrix(const gtsam::Matrix &A)
{
    matlab::data::ArrayFactory f;
    const size_t m = static_cast<size_t>(A.rows());
    const size_t n = static_cast<size_t>(A.cols());
    return f.createArray<double>({m, n}, A.data(), A.data() + (m * n));
}

template <>
inline matlab::data::Array wrap<gtsam::Matrix>(const gtsam::Matrix &A)
{
    return wrap_Matrix(A);
}

/// Wrap a C++ enum into its MATLAB enumeration type by calling its constructor.
template <typename T>
matlab::data::Array wrap_enum(Context &ctx, const T x,
                              const std::string &classname)
{
    std::vector<matlab::data::Array> in(
        {ctx.factory.createScalar<double>(static_cast<double>(x))});
    return ctx.engine->feval(
        matlab::engine::convertUTF8StringToUTF16String(classname), in);
}

//*****************************************************************************
// unwrapping MATLAB arrays into C++ basic types
//*****************************************************************************

template <typename T>
T unwrap(const matlab::data::Array &array)
{
    if constexpr (std::is_integral<T>::value)
    {
        checkScalar(array, "unwrap<integer>");
        return myGetScalar<T>(array);
    }
    else
    {
        error("wrap internal error: attempted unwrap of invalid type");
        return T();
    }
}

/// Unwrap a MATLAB enumeration into a C++ enum (via its int32 value).
template <typename T>
T unwrap_enum(Context &ctx, const matlab::data::Array &array)
{
    std::vector<matlab::data::Array> r = ctx.engine->feval(
        u"int32", 1, std::vector<matlab::data::Array>({array}));
    return static_cast<T>(
        static_cast<std::int32_t>(matlab::data::TypedArray<std::int32_t>(r[0])[0]));
}

template <>
inline string unwrap<string>(const matlab::data::Array &array)
{
    if (array.getType() != matlab::data::ArrayType::CHAR)
        error((std::string("unwrap<string>: expected a char array, got a ") +
               typeName(array.getType()) + " array")
                  .c_str());
    return matlab::data::CharArray(array).toAscii();
}

template <>
inline bool unwrap<bool>(const matlab::data::Array &array)
{
    checkScalar(array, "unwrap<bool>");
    return myGetScalar<bool>(array);
}

template <>
inline char unwrap<char>(const matlab::data::Array &array)
{
    checkScalar(array, "unwrap<char>");
    return myGetScalar<char>(array);
}

template <>
inline unsigned char unwrap<unsigned char>(const matlab::data::Array &array)
{
    checkScalar(array, "unwrap<unsigned char>");
    return myGetScalar<unsigned char>(array);
}

template <>
inline int unwrap<int>(const matlab::data::Array &array)
{
    checkScalar(array, "unwrap<int>");
    return myGetScalar<int>(array);
}

template <>
inline size_t unwrap<size_t>(const matlab::data::Array &array)
{
    checkScalar(array, "unwrap<size_t>");
    return myGetScalar<size_t>(array);
}

template <>
inline double unwrap<double>(const matlab::data::Array &array)
{
    checkScalar(array, "unwrap<double>");
    return myGetScalar<double>(array);
}

template <>
inline gtsam::Vector unwrap<gtsam::Vector>(const matlab::data::Array &array)
{
    const auto dims = array.getDimensions();
    if (array.getType() != matlab::data::ArrayType::DOUBLE || dims[1] != 1)
        error((std::string("unwrap<vector>: expected an Nx1 double column vector, "
                           "got a ") +
               describe(array))
                  .c_str());
    // const TypedArray iterates the column-major storage read-only (no
    // copy-on-write); bulk-copy straight into the Eigen vector.
    const matlab::data::TypedArray<double> data(array);
    gtsam::Vector v(dims[0]);
    std::copy(data.begin(), data.end(), v.data());
    return v;
}

template <>
inline gtsam::Point2 unwrap<gtsam::Point2>(const matlab::data::Array &array)
{
    return unwrap<gtsam::Vector>(array);
}

template <>
inline gtsam::Point3 unwrap<gtsam::Point3>(const matlab::data::Array &array)
{
    return unwrap<gtsam::Vector>(array);
}

template <>
inline gtsam::Matrix unwrap<gtsam::Matrix>(const matlab::data::Array &array)
{
    if (array.getType() != matlab::data::ArrayType::DOUBLE)
        error((std::string("unwrap<matrix>: expected a double matrix, got a ") +
               describe(array))
                  .c_str());
    const auto dims = array.getDimensions();
    // MATLAB storage and gtsam::Matrix (Eigen) are both column-major, so the
    // read-only linear iteration maps directly onto the Eigen buffer.
    const matlab::data::TypedArray<double> data(array);
    gtsam::Matrix A(dims[0], dims[1]);
    std::copy(data.begin(), data.end(), A.data());
    return A;
}

//*****************************************************************************
// MATLAB proxy object creation and shared_ptr (un)wrapping
//*****************************************************************************

/*
 [create_object] creates a MATLAB proxy class object holding a mexhandle in its
 ptr_ property. MATLAB does not allow constructing MATLAB objects from a MEX
 file directly, so we invoke the proxy class constructor through the engine
 with the special ptr_constructor_key and the pointer itself. The special
 constructor in the generated .m code then passes the pointer back into the
 collector. This mirrors matlab.h::create_object exactly.
*/
inline matlab::data::Array create_object(Context &ctx,
                                         const std::string &classname,
                                         void *pointer, bool isVirtual,
                                         const char *rttiName)
{
    std::vector<matlab::data::Array> in;
    in.push_back(ctx.factory.createScalar<std::uint64_t>(ptr_constructor_key));
    in.push_back(ctx.factory.createScalar<std::uint64_t>(
        reinterpret_cast<std::uint64_t>(pointer)));

    std::string derivedClassName = classname;
    if (isVirtual)
    {
        std::string derived;
        try
        {
            matlab::data::Array registry = ctx.engine->getVariable(
                u"gtsamwrap_rttiRegistry", matlab::engine::WorkspaceType::GLOBAL);
            matlab::data::StructArray reg(registry);
            matlab::data::Array nameArr = reg[0][std::string(rttiName)];
            derived = matlab::data::CharArray(nameArr).toAscii();
        }
        catch (const std::exception &)
        {
            gtwrap::MexErrMsgTxt(
                ("wrap:  The derived class type " + string(rttiName) +
                 " was not found in the RTTI registry, or the registry is missing.  "
                 "Try calling 'clear all' twice consecutively. If this does not "
                 "work, this may indicate that a base class was marked virtual in "
                 "the wrap interface file but a returned derived type was not.")
                    .c_str());
        }
        derivedClassName = derived;
        in.push_back(ctx.factory.createCharArray("void"));
    }

    std::vector<matlab::data::Array> out = ctx.engine->feval(
        matlab::engine::convertUTF8StringToUTF16String(derivedClassName), 1, in);
    return out[0];
}

/*
 When a wrapped method returns a shared pointer, create a proxy object from it
 and return it to MATLAB. No mexLock is needed: the MexFunction instance owns
 the collector and is destroyed (running _deleteAllObjects) when MATLAB unloads
 the MEX file.
*/
template <typename Class>
matlab::data::Array wrap_shared_ptr(Context &ctx,
                                    std::shared_ptr<Class> shared_ptr,
                                    const std::string &matlabName,
                                    bool isVirtual)
{
    if (isVirtual)
    {
        std::shared_ptr<void> void_ptr(shared_ptr);
        return create_object(ctx, matlabName, &void_ptr, true,
                             typeid(*shared_ptr).name());
    }
    else
    {
        std::shared_ptr<Class> *heapPtr = new std::shared_ptr<Class>(shared_ptr);
        return create_object(ctx, matlabName, heapPtr, false, "");
    }
}

// Read the heap-allocated shared_ptr stored in a proxy object's handle
// property. Shared by the shared_ptr and raw-pointer unwrap paths.
template <typename Class>
std::shared_ptr<Class> *shared_ptr_handle(Context &ctx,
                                          const matlab::data::Array &obj,
                                          const string &propertyName)
{
    matlab::data::Array handle = ctx.engine->getProperty(
        obj, 0, matlab::engine::convertUTF8StringToUTF16String(propertyName));
    if (handle.getNumberOfElements() != 1)
        error(("Parameter is not a wrapped object: property " + propertyName +
               " is not a scalar handle.")
                  .c_str());
    std::shared_ptr<Class> *spp = get_handle<std::shared_ptr<Class>>(handle);
    if (!spp)
        error("Parameter is not a wrapped object: null handle.");
    return spp;
}

template <typename Class>
std::shared_ptr<Class> unwrap_shared_ptr(Context &ctx,
                                         const matlab::data::Array &obj,
                                         const string &propertyName)
{
    return *shared_ptr_handle<Class>(ctx, obj, propertyName);
}

template <typename Class>
Class *unwrap_ptr(Context &ctx, const matlab::data::Array &obj,
                  const string &propertyName)
{
    return shared_ptr_handle<Class>(ctx, obj, propertyName)->get();
}
