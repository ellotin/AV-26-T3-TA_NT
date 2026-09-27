#pragma once
// Implement Controller so that, given only the target angle, the last
// measured angle, and the timestep, it drives the system to the target --
// despite whatever nonlinearity you identified from the CSVs.
//
// This is the file you submit. You can add private members, helper methods,
// filters, whatever your design needs. We will never run your internals.


#include "controller_interface.hpp"

class Controller : public IController {
private:

        double kp = 4.0;
        double ki=0.5;
        double kd=0.1;

        double integral = 0.0;
        double prev_error = 0.0;

        double integral_limit = 50.0;

public:
            
    double update(double target, double measured, double dt) override {
                
        double error = target - measured;

                double p_out = kp * error;
                
                integral += error *dt;
                if (integral > integral_limit) integral = integral_limit;
                if (integral < -integral_limit) integral = -integral_limit;
                double i_out = ki * integral;

                double deritative = 0.0;

                if(dt>0.0){

                    deritative = (error - prev_error) / dt;
                }

                double d_out = kd * deritative;

                prev_error = error;

                double u_cmd = p_out + i_out + d_out;
                return u_cmd;



            }

    void reset() override {
        double integral = 0.0;
        double prev_error =0.0;
        // TODO: reset any internal state here, if you have any.
    }

};
