#ifndef PHYGINE_VECTOR3_H
#define PHYGINE_VECTOR3_H

#include "precision.cpp"

namespace phygine {
    class Vector3 {
    private:
        /** Padding to ensure 4 word alignment. */
        real pad;

    public:
        /** Holds the value along the x axis. */
        real x;

        /** Holds the value along the y axis. */
        real y;

        /** Holds the value along the z axis. */
        real z;

        /** The default constructor creates a zero vector. */
        Vector3() : x(0), y(0), z(0) {}

        /**
         * The explicit constructor creates a vector with the given
         * components.
         */
        Vector3(const real x, const real y, const real z)
                : x(x), y(y), z(z) {}

        const static Vector3 GRAVITY;

        friend std::ostream& operator<<(std::ostream& os, Vector3 const & vector) {
            return os << "x: " << vector.x << " | y: " << vector.y << " | z: " << vector.z;
        }

        real operator[](unsigned i) const {
            if (i == 0) return x;
            if (i == 1) return y;
            return z;
        }

        real &operator[](unsigned i) {
            if (i == 0) return x;
            if (i == 1) return y;
            return z;
        }

        /** Adds the given vector to this. */
        void operator+=(const Vector3 &v) {
            x += v.x;
            y += v.y;
            z += v.z;
        }

        /**
         * Returns the value of the given vector added to this.
         */
        Vector3 operator+(const Vector3 &v) const {
            return {x + v.x, y + v.y, z + v.z};
        }

        /** Subtracts the given vector from this. */
        void operator-=(const Vector3 &v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;
        }

        /**
         * Returns the value of the given vector subtracted from this.
         */
        Vector3 operator-(const Vector3 &v) const {
            return {x - v.x, y - v.y, z - v.z};
        }

        /** Multiplies this vector by the given scalar. */
        void operator*=(const real value) {
            x *= value;
            y *= value;
            z *= value;
        }

        /** Returns a copy of this vector scaled the given value. */
        Vector3 operator*(const real value) const {
            return Vector3(x * value, y * value, z * value);
        }

        /**
         * Calculates and returns a component-wise product of this
         * vector with the given vector.
         */
        Vector3 componentProduct(const Vector3 &vector) const {
            return Vector3(x * vector.x, y * vector.y, z * vector.z);
        }

        /**
         * Performs a component-wise product with the given vector and
         * sets this vector to its result.
         */
        void componentProductUpdate(const Vector3 &vector) {
            x *= vector.x;
            y *= vector.y;
            z *= vector.z;
        }

        /**
         * Calculates and returns the vector product of this vector
         * with the given vector.
         */
        Vector3 vectorProduct(const Vector3 &vector) const {
            return Vector3(y * vector.z - z * vector.y,
                           z * vector.x - x * vector.z,
                           x * vector.y - y * vector.x);
        }

        /**
         * Updates this vector to be the vector product of its current
         * value and the given vector.
         */
        void operator%=(const Vector3 &vector) {
            *this = vectorProduct(vector);
        }

        /**
         * Calculates and returns the vector product of this vector
         * with the given vector.
         */
        Vector3 operator%(const Vector3 &vector) const {
            return Vector3(y * vector.z - z * vector.y,
                           z * vector.x - x * vector.z,
                           x * vector.y - y * vector.x);
        }

        /**
         * Calculates and returns the scalar product of this vector
         * with the given vector.
         */
        real scalarProduct(const Vector3 &vector) const {
            return x * vector.x + y * vector.y + z * vector.z;
        }

        /**
         * Calculates and returns the scalar product of this vector
         * with the given vector.
         */
        real operator*(const Vector3 &vector) const {
            return x * vector.x + y * vector.y + z * vector.z;
        }

        /**
         * Adds the given vector to this, scaled by the given amount.
         */
        void addScaledVector(const Vector3 &vector, real scale) {
            x += vector.x * scale;
            y += vector.y * scale;
            z += vector.z * scale;
        }

        /** Gets the magnitude of this vector. */
        real magnitude() const {
            return real_sqrt(x * x + y * y + z * z);
        }

        /** Gets the squared magnitude of this vector. */
        real squareMagnitude() const {
            return x * x + y * y + z * z;
        }

        /** Limits the size of the vector to the given maximum. */
        void trim(real size) {
            if (squareMagnitude() > size * size) {
                normalise();
                x *= size;
                y *= size;
                z *= size;
            }
        }

        /** Turns a non-zero vector into a vector of unit length. */
        void normalise() {
            real l = magnitude();
            if (l > 0) {
                (*this) *= ((real) 1) / l;
            }
        }

        /** Returns the normalised version of a vector. */
        Vector3 unit() const {
            Vector3 result = *this;
            result.normalise();
            return result;
        }

        /** Checks if the two vectors have identical components. */
        bool operator==(const Vector3 &other) const {
            return x == other.x &&
                   y == other.y &&
                   z == other.z;
        }

        /** Checks if the two vectors have non-identical components. */
        bool operator!=(const Vector3 &other) const {
            return !(*this == other);
        }

        /**
         * Checks if this vector is component-by-component less than
         * the other.
         *
         * @note This does not behave like a single-value comparison:
         * !(a < b) does not imply (b >= a).
         */
        bool operator<(const Vector3 &other) const {
            return x < other.x && y < other.y && z < other.z;
        }

        /**
         * Checks if this vector is component-by-component less than
         * the other.
         *
         * @note This does not behave like a single-value comparison:
         * !(a < b) does not imply (b >= a).
         */
        bool operator>(const Vector3 &other) const {
            return x > other.x && y > other.y && z > other.z;
        }

        /**
         * Checks if this vector is component-by-component less than
         * the other.
         *
         * @note This does not behave like a single-value comparison:
         * !(a <= b) does not imply (b > a).
         */
        bool operator<=(const Vector3 &other) const {
            return x <= other.x && y <= other.y && z <= other.z;
        }

        /**
         * Checks if this vector is component-by-component less than
         * the other.
         *
         * @note This does not behave like a single-value comparison:
         * !(a <= b) does not imply (b > a).
         */
        bool operator>=(const Vector3 &other) const {
            return x >= other.x && y >= other.y && z >= other.z;
        }

        /** Zero all the components of the vector. */
        void clear() {
            x = y = z = 0;
        }

        /** Flips all the components of the vector. */
        void invert() {
            x = -x;
            y = -y;
            z = -z;
        }

    };

    const Vector3 Vector3::GRAVITY = {0, -9.81, 0};
}

#endif // PHYGINE_VECTOR3_H