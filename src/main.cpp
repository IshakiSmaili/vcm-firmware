#include <Arduino.h>
#include "FakeConnection.h"

const int NUM_REGISTERS = 2;

std::vector<fake::Register> initialRegisters = {
    {1, 100, true, fake::RegisterType::DIGITAL_INPUT, 0, 255},
    {2, 50, false, fake::RegisterType::ANALOG_OUTPUT, 0, 1023}};

fake::FakeConnection *fake; // مؤشر

void setup()
{
    Serial.begin(115200);

    fake = new fake::FakeConnection(NUM_REGISTERS, initialRegisters);
}

void loop()
{
    fake->sendMapRegisters();
    
    fake->readMapRegisters();
    delay(100);
}