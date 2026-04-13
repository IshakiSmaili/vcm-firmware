#include "FakeProtocol.h"

namespace fake
{
    uint8_t calculateCRC(const uint8_t *data, size_t len)
    {
        uint8_t crc = 0;
        for (size_t i = 0; i < len; ++i)
        {
            crc ^= data[i];
        }
        return crc;
    }

    bool validateCRC(const std::vector<uint8_t> &packet)
    {
        if (packet.size() < 4) // START + TYPE + CRC + END
            return false;

        if (packet.front() != START_BYTE || packet.back() != END_BYTE)
            return false;

        uint8_t receivedCRC = packet[packet.size() - 2];

        // نحسب CRC من TYPE إلى آخر data (بدون START و CRC و END)
        uint8_t calculatedCRC = calculateCRC(&packet[1], packet.size() - 3);

        return (receivedCRC == calculatedCRC);
    }


    std::vector<uint8_t> FakeProtocol::createRegBatch(const Register *registers, uint8_t length, uint32_t timestamp)
    {
        std::vector<uint8_t> packet;

        // START + نوع الحزمة + طول السجلات
        packet.push_back(START_BYTE);
        packet.push_back(static_cast<uint8_t>(PacketTypeMCU::REG_BATCH));
        packet.push_back(length);

        // إضافة timestamp كـ 4 بايت (little-endian)
        packet.push_back(static_cast<uint8_t>(timestamp & 0xFF));
        packet.push_back(static_cast<uint8_t>((timestamp >> 8) & 0xFF));
        packet.push_back(static_cast<uint8_t>((timestamp >> 16) & 0xFF));
        packet.push_back(static_cast<uint8_t>((timestamp >> 24) & 0xFF));

        // إضافة بيانات السجلات
        for (uint8_t i = 0; i < length; ++i)
        {
            packet.push_back(registers[i].pinID);
            packet.push_back(static_cast<uint8_t>(registers[i].value & 0xFF));
            packet.push_back(static_cast<uint8_t>((registers[i].value >> 8) & 0xFF));
            packet.push_back(registers[i].isInput ? 1 : 0);
            packet.push_back(static_cast<uint8_t>(registers[i].type));
            packet.push_back(static_cast<uint8_t>(registers[i].minValue & 0xFF));
            packet.push_back(static_cast<uint8_t>((registers[i].minValue >> 8) & 0xFF));
            packet.push_back(static_cast<uint8_t>(registers[i].maxValue & 0xFF));
            packet.push_back(static_cast<uint8_t>((registers[i].maxValue >> 8) & 0xFF));
        }

        // حساب CRC على كل البايتات بعد START
        uint8_t crc = calculateCRC(&packet[1], packet.size() - 1);
        packet.push_back(crc);

        // END_BYTE
        packet.push_back(END_BYTE);

        return packet;
    }

    std::vector<uint8_t> FakeProtocol::createAckNak(PacketTypeMCU type, uint32_t timestamp)
    {
        std::vector<uint8_t> packet;
        packet.push_back(START_BYTE);
        packet.push_back(static_cast<uint8_t>(type));
        packet.push_back(static_cast<uint8_t>(timestamp & 0xFF));
        packet.push_back(static_cast<uint8_t>((timestamp >> 8) & 0xFF));
        packet.push_back(static_cast<uint8_t>((timestamp >> 16) & 0xFF));
        packet.push_back(static_cast<uint8_t>((timestamp >> 24) & 0xFF));
        uint8_t crc = calculateCRC(&packet[1], packet.size() - 1);
        packet.push_back(crc);
        packet.push_back(END_BYTE);
        return packet;
    }
}