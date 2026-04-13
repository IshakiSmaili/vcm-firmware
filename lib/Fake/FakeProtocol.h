#ifndef FAKE_PROTOCOL_H
#define FAKE_PROTOCOL_H

#include "FakeProtocolTyes.h"
#include <vector>
#include <cstdint>
namespace fake
{
    // دالة مساعدة لحساب CRC
    uint8_t calculateCRC(const uint8_t *data, size_t len);

    class FakeProtocol
    {
    public:
        static std::vector<uint8_t> createRegBatch(const Register *registers, uint8_t length, uint32_t timestamp);
        static std::vector<uint8_t> createAckNak(PacketTypeMCU type, uint32_t timestamp);
        static bool validateCRC(const std::vector<uint8_t> &packet);
    };
}
#endif // FAKE_PACKET_GENERATOR_H