#include <arduino.h>
#include "PID.h"

PID::PID(double kP, double kI, double kD) : KP(kP), KI(kI), KD(kD) 
{
  this->p = 0;
  this->i = 0;
  this->d = 0;
  this->out = 0;
  this->prevErr = 0;
}

void PID::reset(void)
{
  this->p = 0;
  this->i = 0;
  this->d = 0;
  this->out = 0;
}

double PID::compute(double setpoint, double input, double dt)
{
  double err = setpoint - input;
  this->p = this->KP * err;
  this->i = i + this->KI * err * dt;
  this->d = this->KD * (err - this->prevErr) / dt;

  if(this->i > 255) this->i = 255;
  else if(this->i < 0) this->i = 0;

  this->out = this->p + this->i + this->d;

  return this->out;
}