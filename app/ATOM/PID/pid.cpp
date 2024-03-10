//
// Created by toor on 3/5/24.
//

#include "pid.h"



using namespace std;

class PIDImpl
{
public:
    PIDImpl( double Kp, double Kd, double Ki );
    ~PIDImpl();
    double calculate( double setpoint, double pv , double deltaTime , double max, double min);

    void SetP(double p);
    void SetI(double i);
    void SetD(double d);

private:
    double _Kp;
    double _Kd;
    double _Ki;
    double _pre_error;
    double _integral;
};


PID::PID( double Kp, double Kd, double Ki )
{
    pimpl = new PIDImpl(Kp,Kd,Ki);
}
double PID::calculate( double setpoint, double pv , double deltaTime ,double max, double min)
{
    return pimpl->calculate(setpoint,pv , deltaTime , max, min);
}

void PID::SetP(double p) {
    pimpl->SetP(p);
}
void PID::SetI(double i) {
    pimpl->SetI(i);
}
void PID::SetD(double d) {
    pimpl->SetD(d);
}

PID::~PID()
{
    delete pimpl;
}


/**
 * Implementation
 */
PIDImpl::PIDImpl(double Kp, double Kd, double Ki ) :
    _Kp(Kp),
    _Kd(Kd),
    _Ki(Ki),
    _pre_error(0),
    _integral(0)
{
}

double PIDImpl::calculate( double setpoint, double pv , double deltaTime , double max, double min)
{

    // Calculate error
    double error = setpoint - pv;

    // Proportional term
    double Pout = _Kp * error;

    // Integral term
    _integral += error * deltaTime;
    double Iout = _Ki * _integral;

    // Derivative term
    double derivative = (error - _pre_error) / deltaTime;
    double Dout = _Kd * derivative;

    // Calculate total output
    double output = Pout + Iout + Dout;

    // Restrict to max/min
    if( output > max )
        output = max;
    else if( output < min )
        output = min;

    // Save error to previous error
    _pre_error = error;

    return output;
}

void PIDImpl::SetP(double p) {
    this->_Kp = p;
}
void PIDImpl::SetI(double i) {
    this->_Ki = i;

}
void PIDImpl::SetD(double d) {
    this->_Kd = d;

}

PIDImpl::~PIDImpl()
{
}
