#include <runtime_demo.h>

namespace demo
{
    enum Color
    {
        Red,
        Green,
        Blue
    };

    class Counter
    {
        Counter();
        Counter(int start);

        void increment();
        int count() const;
        double half() const;
        bool isPositive() const;

        string name() const;
        void setName(const string &n);

        Vector asVector() const;
        Matrix ramp(size_t rows, size_t cols) const;
        double sumVector(Vector v) const;
        double traceMatrix(Matrix m) const;
        double traceMatrixView(demo::ConstMatrixView m) const;
        double elem(Matrix m, int i, int j) const;

        Vector echoVector(Vector v) const;
        Matrix echoMatrix(Matrix m) const;
        void noop() const;
        int echoInt(int x) const;
        bool echoBool(bool b) const;

        demo::Color favorite() const;
        int colorValue(demo::Color c) const;

        int addCount(demo::Counter @other) const;

        void mayThrow(bool doit) const;
    };

    virtual class Shape
    {
        double area() const;
    };

    virtual class Circle : demo::Shape
    {
        Circle();
        Circle(double r);
        double area() const;
        double radius() const;
    };

    demo::Shape *makeCircle(double r);

} // namespace demo
