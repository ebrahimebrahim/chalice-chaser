#ifndef TIMER_H
#define TIMER_H

#include <Entity.h>

class Timer : public Entity {
    bool started{false};
    bool timed_out_{false};
    float time_remaining{}; /**< seconds remaining */
    float initial_time_remaining{}; /**< seconds chosen on timer when it was initially created */

public:

    Timer(float time) : Entity(), time_remaining{time}, initial_time_remaining{time} {}

    void start() {started = true;}

    void update(double delta) override {
        if (started) time_remaining -= delta;
        if (time_remaining <= 0.0f) timed_out_ = true;
    }

    void reset() {
        started = false;
        time_remaining = initial_time_remaining;
    }

    bool timed_out() const {return timed_out_;}

    float get_fraction_remaining() const {return time_remaining/initial_time_remaining;}

    void draw(glm::vec3 player_position) const override {}

};

#endif // TIMER_H