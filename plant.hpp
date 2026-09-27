#pragma once
// Your model of the actuator, reconstructed from the decoded CSVs. This is
// the Part B deliverable, alongside your written notes.
//
// Implement step(): given a commanded velocity and a timestep, return the
// measured output angle. The placeholder below is a bare integrator with
// gain 1 -- NOT the real actuator. Replace it with what the data shows
// (dynamics, gain, any nonlinearity, any lag), or the harness proves nothing.

#include <cmath>

struct Plant {
    // add whatever state your model needs (velocity, motor-side angle, ...)
    double angle = 0.0;
    double current_rate = 0.0;

    double K = 1.286;
    double tau = 0.075;

    double deadzone = 2.0;
    double slack = 0.0;
  

    // u_cmd : commanded velocity, deg/s
    // dt    : timestep, seconds
    // return: measured output angle, deg
    double step(double u_cmd, double dt) {

        slack +=u_cmd*dt;

        double u_eff = 0;

        if (slack>deadzone){

            u_eff=u_cmd;
            
            slack=deadzone;
        }
       
        
        else if(slack<-deadzone){
            u_eff=u_cmd;

            slack=-deadzone;
        }
        
        else{
            u_eff = 0.0;
        }

        
        double target_rate= K * u_eff;        
        current_rate+=(dt/tau) * (target_rate-current_rate);

        angle += current_rate * dt;

        return std::round(angle / 0.1) * 0.1;  // the sensor reads to 0.1 deg
    }

    void reset() { 
        
        current_rate=0.0;
        angle = 0.0;
        
        slack = 0.0;
    }
};
