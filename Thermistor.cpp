#include "Thermistor.h"
#include <math.h>

// float calculateTemperature(float resistance)
// {
//   return (BETA / (log(resistance / R_T) + (BETA / T_0))) - 273.15;
// }

Thermistor::Thermistor(float beta, float resistance) :  BETA(beta), R_T(resistance)
{
  temperature = 0;
  resistance = 0;
}

float Thermistor::getResistance()
{
  return this->resistance;
}

float Thermistor::getTemperature()
{
  return this->temperature;
}

float Thermistor::calculateResistance(uint16_t adcReading)
{
  this->resistance = (10000 * (4095 - adcReading)) / (float)adcReading;
  return this->resistance;
}

float Thermistor::calculateTemperature(float resistance)
{
  this->temperature = (this->BETA / (log(resistance / this->R_T) + (this->BETA / T_0))) - 273.15;
  return this->temperature;
}

float Thermistor::calculateTemperature(uint16_t adcReading)
{
  float r = this->calculateResistance(adcReading);
  float t = this->calculateTemperature(r);
  return t;
}