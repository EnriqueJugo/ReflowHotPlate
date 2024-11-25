#ifndef PID_H
#define PID_H

#include <stdint.h>

class PID
{
  private:
    const double KP, KI, KD;
    double p, i ,d;
    double out;
    double prevErr;
  public:
    PID(double KP, double KI, double KD);
    void reset(void);
    double compute(double setpoint, double input, double dt);
};
#endif /* PID_H */