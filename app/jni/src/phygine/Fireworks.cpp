#ifndef PHYGINE_FIREWORK_H
#define PHYGINE_FIREWORK_H

#include <stdio.h>

#include "precision.cpp"
#include "Random.cpp"
#include "Particle.cpp"

using namespace phygine;

/**
 * Fireworks are particles, with additional data for rendering and
 * evolution.
 */
class Firework : public Particle {
public:
    /** Fireworks have an integer type, used for firework rules. */
    unsigned type;

    /**
     * The age of a firework determines when it detonates. Age gradually
     * decreases, when it passes zero the firework delivers its payload.
     * Think of age as fuse-left.
     */
    real age;

    friend std::ostream &operator<<(std::ostream &os, Firework const &firework) {
        return os << "position " << firework.position << std::endl
                  << " velocity " << firework.velocity << std::endl
                  << " acceleration " << firework.acceleration << std::endl
                  << " damping " << firework.damping << " and age " << firework.age << std::endl;
    }

    /**
     * Updates the firework by the given duration of time. Returns true
     * if the firework has reached the end of its life and needs to be
     * removed.
     */
    bool update(real duration) {
        // Update our physical state
        integrate(duration);

        // We work backwards from our age to zero.
        age -= duration;
        return (age < 0) || (position.y < 0);
    }
};

/**
 * Firework rules control the length of a firework's fuse and the
 * particles it should evolve into.
 */
struct FireworkRule {
    /** The type of firework that is managed by this rule. */
    unsigned type;

    /** The minimum length of the fuse. */
    real minAge;

    /** The maximum legnth of the fuse. */
    real maxAge;

    /** The minimum relative velocity of this firework. */
    Vector3 minVelocity;

    /** The maximum relative velocity of this firework. */
    Vector3 maxVelocity;

    /** The damping of this firework type. */
    real damping;

    Uint8 r;
    Uint8 g;
    Uint8 b;

    unsigned x_repartition;
    unsigned y_repartition;

    /**
     * The payload is the new firework type to create when this
     * firework's fuse is over.
     */
    struct Payload {
        /** The type of the new particle to create. */
        unsigned type;

        /** The number of particles in this payload. */
        unsigned count;

        /** Sets the payload properties in one go. */
        void set(unsigned type, unsigned count) {
            Payload::type = type;
            Payload::count = count;
        }
    };

    /** The number of payloads for this firework type. */
    unsigned payloadCount;

    /** The set of payloads. */
    Payload *payloads;

    FireworkRule() : payloadCount(0), payloads(nullptr) {}

    void init(unsigned payloadCount) {
        FireworkRule::payloadCount = payloadCount;
        payloads = new Payload[payloadCount];
    }

    ~FireworkRule() {
        delete[] payloads;
    }

    /**
     * Set all the rule parameters in one go.
     */
    void setParameters(
            unsigned type, real minAge, real maxAge,
            const Vector3 &minVelocity, const Vector3 &maxVelocity, real damping,
            const unsigned x_repartition, const unsigned y_repartition,
            const Uint8 r, const Uint8 g, const Uint8 b
    ) {
        FireworkRule::type = type;
        FireworkRule::minAge = minAge;
        FireworkRule::maxAge = maxAge;
        FireworkRule::minVelocity = minVelocity;
        FireworkRule::maxVelocity = maxVelocity;
        FireworkRule::damping = damping;

        FireworkRule::x_repartition = x_repartition;
        FireworkRule::y_repartition = y_repartition;

        FireworkRule::r = r;
        FireworkRule::g = g;
        FireworkRule::b = b;
    }

    /**
     * Creates a new firework of this type and writes it into the given
     * instance. The optional parent firework is used to base position
     * and velocity on.
     */
    void create(Firework *firework, const Firework *parent = nullptr) const {
        firework->type = type;
        firework->age = Random::r.randomReal(minAge, maxAge);

        Vector3 vel;
        if (parent) {
            real x = (real) Random::r.randomInt(x_repartition * 2) - x_repartition;
            real y = (real) Random::r.randomInt(x_repartition * 2) - x_repartition;
            // The position and velocity are based on the parent.
            firework->position = parent->position + Vector3{x, y, 1};
        } else {
            Vector3 start;
            int x = (int) Random::r.randomInt(200) + 20;
            start.x = real(x);

            start.y = 0;
            start.z = 0;
            firework->position = start;
        }

        vel += Random::r.randomVector(minVelocity, maxVelocity);
        firework->velocity = vel;

        // We use a mass of one in all cases (no point having fireworks
        // with different masses, since they are only under the influence
        // of gravity).
        firework->setMass(1);
        firework->damping = damping;
        firework->acceleration = Vector3::GRAVITY;
        firework->forceAccum.clear();
    }
};

class FireworksDemo {
    /** Holds the index of the next firework slot to use. */

    /** Holds the maximum number of fireworks that can be in use. */
    const static unsigned maxFireworks = 1024;

    /** Holds the firework data. */
    Firework fireworks[maxFireworks];

    /** Holds the index of the next firework slot to use. */
    unsigned nextFirework;

    /** And the number of rules. */
    const static unsigned ruleCount = 9;

    /** Holds the set of rules. */
    FireworkRule rules[ruleCount];

private:
    /** Creates the rules. */
    void _initFireworkRules() {
        // Go through the firework types and _create their rules.
        rules[0].init(1);
        rules[0].setParameters(
                1, // type
                1.5f, 1.9f, // age range
                Vector3(-5, 300, 1), // min velocity
                Vector3(5, 320, 1), // max velocity
                0.6, // damping
                1, 1, // repartition of particules
                0xFF, 0x00, 0x00 // Colors
        );
        rules[0].payloads[0].set(1, 7);

        rules[1].init(1);
        rules[1].setParameters(
                2, // type
                0.8f, 0.9f, // age range
                Vector3(-50, -50, 1), // min velocity
                Vector3(50, 50, 1), // max velocity
                0.2, // damping
                50, 50, // repartition of particules
                0xFF, 0x00, 0x00 // Colors
        );
        rules[1].payloads[0].set(2, 50);

        rules[2].init(0);
        rules[2].setParameters(
                3, // type
                0.5f, 0.6f, // age range
                Vector3(-5, -5, 1), // min velocity
                Vector3(5, 5, 1), // max velocity
                0.1, // damping
                10, 10, // repartition of particules
                0xFF, 0x00, 0x00 // Colors
        );

        rules[3].init(0);
        rules[3].setParameters(
                4, // type
                0.7f, 0.75f, // age range
                Vector3(-40, -20, 1), // min velocity
                Vector3(40, 20, 1), // max velocity
                0.2, // damping
                40, 40, // repartition of particules
                0xFF, 0x00, 0x00 // Colors
        );

        rules[4].init(0);
        rules[4].setParameters(
                5, // type
                0.5f, 1.0f, // age range
                Vector3(0, 200, 1), // min velocity
                Vector3(0, 201, 1), // max velocity
                0.01, // damping
                1, 1, // repartition of particules
                0x00, 0xFF, 0x00 // Colors
        );
    }

    /** Dispatches a firework from the origin. */
    void _create(unsigned type, const Firework *parent) {
        // Get the rule needed to _create this firework
        FireworkRule *rule = rules + type;

        // Create the firework
        rule->create(fireworks + nextFirework, parent);

        // Increment the index for the next firework
        nextFirework = (nextFirework + 1) % maxFireworks;
    }

    /** Dispatches the given number of fireworks from the given parent. */
    void _create(unsigned type, unsigned number, const Firework *parent) {
        for (unsigned i = 0; i < number; i++) {
            _create(type, parent);
        }
    }

public:
    /** Creates a new demo object. */
    FireworksDemo() : nextFirework(0) {
        // Make all shots unused
        for (Firework *firework = fireworks; firework < fireworks + maxFireworks; firework++) {
            firework->type = 0;
        }

        // Create the firework types
        _initFireworkRules();
    }

    ~FireworksDemo() = default;

    /** Update the particle positions. */
    void update(float lastFrameDuration) {
        if (lastFrameDuration <= 0.0f) return;

        for (Firework *firework = fireworks; firework < fireworks + maxFireworks; firework++) {
            // Check if we need to process this firework.
            if (firework->type > 0) {
                // Does it need removing?
                if (firework->update(lastFrameDuration)) {
                    // Find the appropriate rule
                    FireworkRule *rule = rules + (firework->type - 1);

                    // Delete the current firework (this doesn't affect its
                    // position and velocity for passing to the _create function,
                    // just whether or not it is processed for rendering or
                    // physics.
                    firework->type = 0;

                    // Add the payload
                    for (unsigned i = 0; i < rule->payloadCount; i++) {
                        FireworkRule::Payload *payload = rule->payloads + i;
                        _create(payload->type, payload->count, firework);
                    }
                }
            }
        }
    }

    /** Display the particle positions. */
    void display(SDL_Renderer *renderer) {
        const static int size = 5;

        for (Firework *firework = fireworks; firework < fireworks + maxFireworks; firework++) {
            // Check if we need to process this firework.
            if (firework->type > 0) {
                PP &pp = PP::getInstance();

                const Vector3 &pos = firework->position;
                FireworkRule *rule = rules + (firework->type - 1);

                pp.render_pixel(
                        renderer,
                        rule->r, rule->g, rule->b, 0xFF,
                        static_cast<int>(pos.x), static_cast<int>(pos.y), size, size
                );
            }
        }
    }

    /** Handle a keypress. */
    void key(SDL_Scancode key) {
        switch (key) {
            case SDL_SCANCODE_KP_1:
                _create(0, 1, nullptr);
                break;
            case SDL_SCANCODE_KP_2:
                _create(1, 1, nullptr);
                break;
//            case '3':
//                _create(3, 1, nullptr);
//                break;
//            case '4':
//                _create(4, 1, nullptr);
//                break;
//            case '5':
//                _create(5, 1, nullptr);
//                break;
//            case '6':
//                _create(6, 1, nullptr);
//                break;
//            case '7':
//                _create(7, 1, nullptr);
//                break;
//            case '8':
//                _create(8, 1, nullptr);
//                break;
//            case '9':
//                _create(9, 1, nullptr);
//                break;
        }
    }
};

#endif // PHYGINE_FIREWORK_H