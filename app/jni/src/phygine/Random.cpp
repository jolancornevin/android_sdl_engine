#include "precision.cpp"
#include "Vector3.cpp"

namespace phygine {
    /** A particle is the simplest object that can be simulated in the physics system.*/
    class Random {
    private:
        // Internal mechanics
        int p1, p2;
        unsigned buffer[17];

        void _seed(unsigned s)
        {
            if (s == 0) {
                s = (unsigned)SDL_GetTicks();
            }

            // Fill the buffer with some basic random numbers
            for (unsigned i = 0; i < 17; i++)
            {
                // Simple linear congruential generator
                s = s * 2891336453 + 1;
                buffer[i] = s;
            }

            // Initialize pointers into the buffer
            p1 = 0;  p2 = 10;
        }

    public:
        static Random r;

        Random() {
            this->_seed(0);
        }

        real randomReal() {
            // Get the random number
            unsigned bits = randomBits();

            // Set up a reinterpret structure for manipulation
            union {
                real value;
                unsigned word;
            } convert;

            // Now assign the bits to the word. This works by fixing the ieee
            // sign and exponent bits (so that the size of the result is 1-2)
            // and using the bits to _create the fraction part of the float.
            convert.word = (bits >> 9) | 0x3f800000;

            // And return the value
            return convert.value - 1.0f;
        }

        Vector3 randomVector(const Vector3 &min, const Vector3 &max)
        {
            return {randomReal(min.x, max.x), randomReal(min.y, max.y), randomReal(min.z, max.z)};
        }

        real randomReal(real min, real max) {
            return randomReal() * (max - min) + min;
        }

        unsigned randomInt(unsigned max) {
            return rand() % max;
        }

        unsigned rotl(unsigned n, unsigned ri) {
            return (n << ri) | (n >> (32 - ri));
        }

        unsigned randomBits() {
            _seed(0);

            unsigned result;

            // Rotate the buffer and store it back to itself
            result = buffer[p1] = rotl(buffer[p2], 13) + rotl(buffer[p1], 9);

            // Rotate pointers
            if (--p1 < 0) p1 = 16;
            if (--p2 < 0) p2 = 16;

            // Return result
            return result;
        }
    };

    Random Random::r = Random();
}