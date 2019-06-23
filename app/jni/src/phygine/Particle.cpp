#ifndef PHYGINE_PARTICLE
#define PHYGINE_PARTICLE

#include <assert.h>

#include "Vector3.cpp"
#include "precision.cpp"

namespace phygine {
    /** A particle is the simplest object that can be simulated in the physics system.*/
    class Particle {
    public:
        /*** Holds the linear position of the particle in world space.*/
        Vector3 position;

        /*** Holds the linear velocity of the particle in world space.*/
        Vector3 velocity;

        /**
         * Holds the acceleration of the particle. This value
         * can be used to set acceleration due to gravity (its primary
         * use) or any other constant acceleration.
         */
        Vector3 acceleration;

        /**
         * Holds the amount of damping applied to linear
         * motion. Damping is required to remove energy added
         * through numerical instability in the integrator.
         */
        real damping;

        /**
         * Holds the accumulated force to be applied at the next simulation iteration only.
         * This value is zeroed at each integration step.
         */
        Vector3 forceAccum;

        void setMass(real mass) {
            assert(mass != 0.0);
            this->inverseMass = 1 / mass;
        }

        void setInverseMass(real inverseMass) {
            this->inverseMass = inverseMass;
        }

        void addForce(const Vector3 &force) {
            forceAccum += force;
        }

        void clearAccumulator() {
            forceAccum.clear();
        }

        /**
         * Integrates the particle forward in time by the given amount.
         * This function uses a Newton-Euler integration method, which is a linear approximation of the correct integral.
         * For this reason it may be inaccurate in some cases.
         *
         * It first update the position with it's velocity.
         * And then update the velocity with it's acceleration.
         * and reduce it with the drag.
         *
         * Duration is in milliseconds.
         */
        void integrate(real duration) {
            assert(duration > 0.0);

            // Update linear position.
            position.addScaledVector(velocity, duration);
//            std::cout << "pos " << position;

            // Work out the acceleration from the force. (f = m*a) and thus a = 1/m * f.
            Vector3 resultingAcc = acceleration;
            resultingAcc.addScaledVector(forceAccum, inverseMass);
//            std::cout << " acc " << resultingAcc;

            // Update linear velocity from the acceleration.
            velocity.addScaledVector(resultingAcc, duration);

            // Impose drag.
            velocity *= real_pow(damping, duration);
//            std::cout << " vel " << velocity << std::endl;

            clearAccumulator();
        }

    protected:
        /**
         * Holds the inverse of the mass of the particle. It
         * is more useful to hold the inverse mass because
         * integration is simpler and because in real-time
         * simulation it is more useful to have objects with
         * infinite mass (immovable) than zero mass
         * (completely unstable in numerical simulation).
         */
        real inverseMass;
    };
}

#endif // PHYGINE_PARTICLE