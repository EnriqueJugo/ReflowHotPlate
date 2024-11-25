#ifndef THERMISTOR_H
#define THERMISTORE_H

#define T_0 298.15

#include <stdint.h>


class Thermistor
{
  private:
    float temperature;
    float resistance;
    const float R_T, BETA;
  public:
    Thermistor(float beta, float resistance);
    float getTemperature();
    float getResistance();
    float calculateTemperature(float resistance);
    float calculateResistance(uint16_t adcReading);
    float calculateTemperature(uint16_t adcReading);
};
#endif