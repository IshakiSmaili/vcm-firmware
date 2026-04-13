#ifndef FAKE_CONNECTION_H
#define FAKE_CONNECTION_H



#include <vector>
#include <cstdint>
#include <Arduino.h>

#include "FakeProtocolTyes.h"
#include "FakeProtocol.h"
namespace fake
{

    class FakeConnection
    {
    public:
        int lengthRegisters;
        uint32_t lastSendTime;
        uint32_t timestamp;
        uint32_t sendIntervalMs;
        std::vector<Register> mapRegisters;

        FakeConnection(int lengthRegisters, const std::vector<Register> &initialRegisters);

        void sendMapRegisters();
        void sendAckNak(PacketTypeMCU type);

        std::vector<uint8_t> readMapRegisters();

        void setSendInterval(uint32_t interval);

    private:
        void sendToPc(std::vector<uint8_t> packet);
        std::vector<uint8_t> readFromPc();
    };
}
#endif // FAKE_CONNECTION_H