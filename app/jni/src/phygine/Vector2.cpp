namespace phygine {

    class Vector2 {
    public:
        real x;
        real y;

        /** Default constructor */
        Vector2() : x(0), y(0) {}

        /** Explicit constructor. */
        Vector2(const real x, const real y) : x(x), y(y) {}

        ~Vector2() {}

        /** Flips all the components of the vector. */
        void invert() {
            x = -x;
            y = -y;
        }

        /** Gets the magnitude of this vector. */
        real magnitude() const {
            return real_sqrt(x * x + y * y + z * z);
        }
    };

}
