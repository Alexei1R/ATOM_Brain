//
// Created by toor on 3/5/24.
//

#ifndef PID_H
#define PID_H
#include "ATOM/atompch.h"


class PIDImpl;
class PID
{
public:
    // Kp -  proportional gain
    // Ki -  Integral gain
    // Kd -  derivative gain
    // dt -  loop interval time
    // max - maximum value of manipulated variable
    // min - minimum value of manipulated variable
    PID(double Kp, double Kd, double Ki );

    // Returns the manipulated variable given a setpoint and current process value
    double calculate( double setpoint, double pv , double deltaTime,double max, double min );

    void SetP(double p);
    void SetI(double i);
    void SetD(double d);
    ~PID();

private:
    PIDImpl *pimpl;
};



#endif //PID_H
