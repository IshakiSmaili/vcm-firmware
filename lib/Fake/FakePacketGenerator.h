#ifndef FAKE_PACKET_GENERATOR_H
#define FAKE_PACKET_GENERATOR_H

#include "FakeDataStructure.h"
#include <vector>
#include <cstdint>

// دالة مساعدة لحساب CRC
uint8_t calculateCRC(const uint8_t* data, size_t len);

class FakePacketGenerator {
public:
    static std::vector<uint8_t> createRegBatch(const Register* registers, uint8_t length, uint32_t timestamp);
    static std::vector<uint8_t> createAckNak(PacketTypeMCU type);
};

#endif // FAKE_PACKET_GENERATOR_H