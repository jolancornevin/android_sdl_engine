#ifndef PHYGINE_BUOYANCY_H
#define PHYGINE_BUOYANCY_H

#include <stdio.h>

#include "precision.cpp"
#include "Particle.cpp"
#include "ForceGenerator.cpp"

using namespace phygine;

/**
     * A force generator that applies a buoyancy force for a plane of
     * liquid parrallel to XZ plane.
     */
class ParticleBuoyancy : public ParticleForceGenerator {
    /**
     * The maximum submersion depth of the object before
     * it generates its maximum boyancy force.
     */
    real maxDepth;

    /**
     * The volume of the object.
     */
    real volume;

    /**
     * The height of the water plane above y=0. The plane will be
     * parrallel to the XZ plane.
     */
    real waterHeight;

    /**
     * The density of the liquid. Pure water has a density of
     * 1000kg per cubic meter.
     */
    real liquidDensity;

public:
    /** Creates a new buoyancy force with the given parameters. */
    ParticleBuoyancy(real maxDepth, real volume, real waterHeight, real liquidDensity = 1000.0f) :
            maxDepth(maxDepth), volume(volume), waterHeight(waterHeight), liquidDensity(liquidDensity) {}

    /** Applies the buoyancy force to the given particle. */
    virtual void updateForce(Particle *particle, real duration) {
        // Calculate the submersion depth
        real depth = particle->getPosition().y;

        // Check if we're out of the water
        if (depth >= waterHeight + maxDepth) return;
        Vector3 force(0, 0, 0);

        // Check if we're at maximum depth
        if (depth <= waterHeight - maxDepth) {
            force.y = liquidDensity * volume;
            particle->addForce(force);
            return;
        }

        // Otherwise we are partly submerged
        force.y = liquidDensity * volume * (depth - maxDepth - waterHeight) / 2 * maxDepth;
        particle->addForce(force);
    }
};

#endif // PHYGINE_BUOYANCY_H