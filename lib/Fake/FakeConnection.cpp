# include "FakeConnection.h"
namespace fake
{

    FakeConnection::FakeConnection(int lengthRegisters, const std::vector<Register> &initialRegisters) : lengthRegisters(lengthRegisters),
                                                                                                         mapRegisters(initialRegisters),
                                                                                                         lastSendTime(0),
                                                                                                         sendIntervalMs(1000) // default 1 second
    {
    }

    void FakeConnection::sendMapRegisters()
    {
        uint32_t nowTime = millis();

        if (nowTime - lastSendTime < sendIntervalMs)
            return;

        lastSendTime = nowTime;

        std::vector<uint8_t> packet =
            FakeProtocol::createRegBatch(mapRegisters.data(), lengthRegisters, nowTime);

        sendToPc(packet);
    }

    void FakeConnection::sendAckNak(PacketTypeMCU type)
    {
        timestamp = millis();
        std::vector<uint8_t> packet = FakeProtocol::createAckNak(type, timestamp);
        sendToPc(packet);
    }

    std::vector<uint8_t> FakeConnection::readMapRegisters()
    {
        std::vector<uint8_t> packet = readFromPc();

        if (packet.empty())
            return {};

        if (!FakeProtocol::validateCRC(packet))
        {
            sendAckNak(PacketTypeMCU::NAK);
            return {};
        }

        if (packet[1] != static_cast<uint8_t>(PacketTypePC::REG_UPDATE))
        {
            sendAckNak(PacketTypeMCU::NAK);
            return {};
        }

        uint8_t numRegs = packet[2];

        if (numRegs == 0 || numRegs > lengthRegisters)
        {
            sendAckNak(PacketTypeMCU::NAK);
            return {};
        }

        sendAckNak(PacketTypeMCU::ACK);

        return packet;
    }

    void FakeConnection::sendToPc(std::vector<uint8_t> packet)
    {
        Serial.write(packet.data(), packet.size());
    }

    std::vector<uint8_t> FakeConnection::readFromPc()
    {
        static std::vector<uint8_t> buffer;

        while (Serial.available())
        {
            uint8_t byte = Serial.read();
            buffer.push_back(byte);

            if (buffer.size() < 4)
                continue;

            if (buffer[0] != START_BYTE)
            {
                buffer.clear();
                continue;
            }

            uint8_t msgType = buffer[1];

            if (msgType == static_cast<uint8_t>(PacketTypePC::REG_UPDATE))
            {
                uint8_t numRegs = buffer[2];

                size_t expectedSize = 3 + (numRegs * 3) + 1;

                if (buffer.size() < expectedSize)
                    continue;

                std::vector<uint8_t> packet(buffer.begin(), buffer.begin() + expectedSize);

                buffer.erase(buffer.begin(), buffer.begin() + expectedSize);

                return packet;
            }
        }

        return {};
    }

    void FakeConnection::setSendInterval(uint32_t interval)
    {
        sendIntervalMs = interval;
    }

}