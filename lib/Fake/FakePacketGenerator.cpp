#include "FakePacketGenerator.h"

uint8_t calculateCRC(const uint8_t* data, size_t len) {
    uint8_t crc = 0;
    for (size_t i = 0; i < len; ++i) {
        crc ^= data[i];
    }
    return crc;
}

std::vector<uint8_t> FakePacketGenerator::createRegBatch(const Register* registers, uint8_t length, uint32_t timestamp) {
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
    for (uint8_t i = 0; i < length; ++i) {
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

std::vector<uint8_t> FakePacketGenerator::createAckNak(PacketTypeMCU type) {
    std::vector<uint8_t> packet;
    packet.push_back(START_BYTE);
    packet.push_back(static_cast<uint8_t>(type));
    uint8_t crc = calculateCRC(&packet[1], packet.size() - 1);
    packet.push_back(crc);
    packet.push_back(END_BYTE);
    return packet;
}