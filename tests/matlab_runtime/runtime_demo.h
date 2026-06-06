/* Self-contained C++ classes used to compile-and-run the generated C++ MEX
 * wrapper against a real MATLAB + libgtsam. Header-only so the runtime test
 * needs nothing but libgtsam to link. Exercises the full wrap/unwrap surface
 * plus edge cases (Eigen column/row-major, virtual RTTI upcast, enum, errors).
 */
#pragma once

#include <gtsam/base/Matrix.h>
#include <gtsam/base/Vector.h>

#include <memory>
#include <stdexcept>
#include <string>

namespace demo
{
    using ConstMatrixView = Eigen::Ref<const gtsam::Matrix>;

    enum Color
    {
        Red,
        Green,
        Blue
    };

    // Scalars, strings, Eigen, enum, error path.
    class Counter
    {
        int count_;
        std::string name_;

      public:
        Counter() : count_(0), name_("counter") {}
        explicit Counter(int start) : count_(start), name_("counter") {}

        void increment() { ++count_; }
        int count() const { return count_; }
        double half() const { return count_ / 2.0; }
        bool isPositive() const { return count_ > 0; }

        std::string name() const { return name_; }
        void setName(const std::string &n) { name_ = n; }

        // Eigen round-trips. asVector / scaledIdentity check wrap; the *Of methods
        // check unwrap. elem01 pins down (i,j) vs column-major ordering.
        gtsam::Vector asVector() const
        {
            gtsam::Vector v(3);
            v << count_, count_ + 1, count_ + 2;
            return v;
        }
        gtsam::Matrix ramp(size_t rows, size_t cols) const
        {
            gtsam::Matrix m(rows, cols);
            for (size_t i = 0; i < rows; ++i)
                for (size_t j = 0; j < cols; ++j)
                    m(i, j) = 10.0 * i + j;
            return m;
        }
        double sumVector(const gtsam::Vector &v) const { return v.sum(); }
        double traceMatrix(const gtsam::Matrix &m) const { return m.trace(); }
        double traceMatrixView(ConstMatrixView m) const { return m.trace(); }
        double elem(const gtsam::Matrix &m, int i, int j) const { return m(i, j); }

        // Trivial echoes used by the C-vs-C++ MEX benchmark (run_benchmark.m). The
        // bodies are deliberately near-empty so timing isolates wrap/unwrap
        // marshalling cost rather than computation. noop() measures pure call
        // dispatch + handle decode; echoBool() exercises the bool wrap divergence
        // (C: uint32, C++: logical).
        gtsam::Vector echoVector(const gtsam::Vector &v) const { return v; }
        gtsam::Matrix echoMatrix(const gtsam::Matrix &m) const { return m; }
        void noop() const {}
        int echoInt(int x) const { return x; }
        bool echoBool(bool b) const { return b; }

        Color favorite() const { return Green; }
        int colorValue(Color c) const { return static_cast<int>(c); }

        // Raw-pointer (non-owning) argument: exercises unwrap_ptr.
        int addCount(const Counter *other) const
        {
            return count_ + (other ? other->count() : 0);
        }

        void mayThrow(bool doit) const
        {
            if (doit)
                throw std::runtime_error("demo: intentional error from C++");
        }
    };

    // Virtual hierarchy: factory returns a base pointer to a derived object, which
    // drives the RTTI registry + upcast-from-void path.
    class Shape
    {
      public:
        virtual ~Shape() {}
        virtual double area() const = 0;
        virtual std::shared_ptr<Shape> clone() const = 0;
    };

    class Circle : public Shape
    {
        double r_;

      public:
        Circle() : r_(1.0) {}
        explicit Circle(double r) : r_(r) {}
        double area() const override { return 3.14159265358979323846 * r_ * r_; }
        double radius() const { return r_; }
        std::shared_ptr<Shape> clone() const override
        {
            return std::make_shared<Circle>(*this);
        }
    };

    inline std::shared_ptr<Shape> makeCircle(double r)
    {
        return std::make_shared<Circle>(r);
    }

} // namespace demo
