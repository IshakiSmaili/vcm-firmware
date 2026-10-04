#include "esp32-hal-gpio.h"
#include <Arduino.h>

#include <Vcm.h>

using namespace vcm;

Vcm _vcm;

constexpr uint8_t LED_PIN = 2;

McuRegisterProvider::RegisterMap mcuRegisters = {
    {LED_PIN,
     {
         LED_PIN,
         GPIO_OUT_REG,
         (1UL << LED_PIN),
         RegisterType::DIGITAL_OUTPUT,
         0,
         1,
     }},
};

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  _vcm.initMapRegister(mcuRegisters);
}

void loop() {
  _vcm.processIncoming();

  // Firmware logic

  _vcm.sendRegisters();
}
