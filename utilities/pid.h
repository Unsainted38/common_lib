#ifndef PID_H
#define PID_H

class PID {
public:
    explicit PID(double kp, double ki, double kd) :
        kp_(kp), ki_(ki), kd_(kd) {

    }
    double calculateOP(double pv, double sp, double dt) {
        if (dt <= 0.0) {
            return 0.0;
        }
        double op = 0.0;
        double error = sp - pv;

        integral_ += error * dt;

        double derivative = 0.0;
        if (!initialized_) {
            previous_position_ = pv;
            initialized_ = true;
        }
        derivative = (pv - previous_position_) / dt;
        previous_position_ = pv;

        op = kp_ * error + ki_ * integral_ + kd_ * derivative;

        return op;
    }

    void reset_pid() {
        integral_ = 0.0;
        initialized_ = false;
    }

private:
    const double kp_, ki_, kd_;
    double integral_ = 0.0;
    double previous_position_;
    bool initialized_ = false;
};

#endif // PID_H
