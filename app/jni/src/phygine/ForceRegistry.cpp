#ifndef PHYGINE_FORCE_REGISTRY
#define PHYGINE_FORCE_REGISTRY

#include "Particle.cpp"
#include "ForceGenerator.cpp"

namespace phygine {
    /**
     * Holds all the force generators and the particles they apply to.
     */
    class ForceRegistry {
    protected:
        /**
         * Keeps track of one force generator and the particle it
         * applies to.
         */
        struct ParticleForceRegistration {
            Particle *particle;
            ParticleForceGenerator *fg;
        };
        typedef std::vector <ParticleForceRegistration> Registry;
        Registry registrations;

    public:
        /**
         * Registers the given force generator to apply to the
         * given particle.
         */
        void add(Particle *particle, ParticleForceGenerator *fg) {
            ParticleForceRegistration reg;
            reg.particle = particle;
            reg.fg = fg;

            this->registrations.push_back(reg);
        }

        /**
         * Calls all the force generators to update the forces of
         * their corresponding particles.
         */
        void updateForces(real duration) {
            for (auto &reg : this->registrations) // access by reference to avoid copying
            {
                reg.fg->updateForce(reg.particle, duration);
            }
        }

        /**
         * Removes the given registered pair from the registry.
         * If the pair is not registered, this method will have
         * no effect.
         */
        void remove(Particle *particle, ParticleForceGenerator *fg) {
            this->registrations.erase(
                    std::remove_if(
                            this->registrations.begin(),
                            this->registrations.end(),
                            [particle, fg](const ParticleForceRegistration& reg) {
                                return reg.particle == particle && reg.fg == fg;
                            }
                    ),
                    this->registrations.end()
            );
        }

        /**
         * Clears all registrations from the registry. This will
         * not delete the particles or the force generators
         * themselves, just the records of their connection.
         */
        void clear() {
            this->registrations.clear();
        }
    };
}

#endif // PHYGINE_FORCE_REGISTRY