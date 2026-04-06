#include "FakeConnection.h"
#include <FakePacketGenerator.h>

FakeConnection::FakeConnection(int lengthRegisters, const std::vector<Register> &initialRegisters) : lengthRegisters(lengthRegisters), registers(initialRegisters) {}

void FakeConnection::sendMapRegistersToPC()
{
    timestamp = millis();
    std::vector<uint8_t> packet = FakePacketGenerator::createRegBatch(registers.data(), lengthRegisters, timestamp);
    Serial.write(packet.data(), packet.size());
}
