#include "VcmProtocol.h"
#include "Register.h"

namespace vcm {

uint8_t VcmProtocol::calculateCRC(const std::vector<uint8_t> &data) const {
  uint8_t crc = 0;

  for (size_t i = 0; i < data.size(); ++i)
    crc ^= data[i];

  return crc;
}

std::vector<uint8_t> VcmProtocol::createRegBatch(const RegisterInfo *registers,
                                                 uint8_t count,
                                                 uint32_t timestamp) const {
  std::vector<uint8_t> packet;

  packet.push_back(START_BYTE);
  packet.push_back(static_cast<uint8_t>(PacketTypeMCU::REG_BATCH));

  packet.push_back(count);

  packet.push_back(static_cast<uint8_t>(timestamp & 0xFF));

  packet.push_back(static_cast<uint8_t>((timestamp >> 8) & 0xFF));

  packet.push_back(static_cast<uint8_t>((timestamp >> 16) & 0xFF));

  packet.push_back(static_cast<uint8_t>((timestamp >> 24) & 0xFF));

  for (uint8_t i = 0; i < count; ++i) {
    const RegisterInfo &reg = registers[i];

    packet.push_back(reg.pinID);

    packet.push_back(static_cast<uint8_t>(reg.value & 0xFF));

    packet.push_back(static_cast<uint8_t>((reg.value >> 8) & 0xFF));

    // packet.push_back(reg.isInput ? 0x01 : 0x00);

    packet.push_back(static_cast<uint8_t>(reg.type));

    packet.push_back(static_cast<uint8_t>(reg.minValue & 0xFF));

    packet.push_back(static_cast<uint8_t>((reg.minValue >> 8) & 0xFF));

    packet.push_back(static_cast<uint8_t>(reg.maxValue & 0xFF));

    packet.push_back(static_cast<uint8_t>((reg.maxValue >> 8) & 0xFF));
  }

  // CRC على البيانات بين START و CRC
  std::vector<uint8_t> crcData(packet.begin() + 1, packet.end());

  uint8_t crc = calculateCRC(crcData);

  packet.push_back(crc);
  packet.push_back(END_BYTE);

  return packet;
}

std::vector<uint8_t> VcmProtocol::createAckNak(PacketTypeMCU type,
                                               uint32_t timestamp) const {
  std::vector<uint8_t> packet;

  packet.push_back(START_BYTE);
  packet.push_back(static_cast<uint8_t>(type));

  packet.push_back(static_cast<uint8_t>(timestamp & 0xFF));

  packet.push_back(static_cast<uint8_t>((timestamp >> 8) & 0xFF));

  packet.push_back(static_cast<uint8_t>((timestamp >> 16) & 0xFF));

  packet.push_back(static_cast<uint8_t>((timestamp >> 24) & 0xFF));

  std::vector<uint8_t> crcData(packet.begin() + 1, packet.end());

  uint8_t crc = calculateCRC(crcData);

  packet.push_back(crc);
  packet.push_back(END_BYTE);

  return packet;
}

bool VcmProtocol::validateCRC(const std::vector<uint8_t> &packet) const {
  if (packet.size() < 3)
    return false;

  if (packet.front() != START_BYTE)
    return false;

  if (packet.back() != END_BYTE)
    return false;

  uint8_t expectedCRC = packet[packet.size() - 2];

  std::vector<uint8_t> crcData(packet.begin() + 1, packet.end() - 2);

  uint8_t actualCRC = calculateCRC(crcData);

  return expectedCRC == actualCRC;
}

bool VcmProtocol::parseRegisterUpdate(const std::vector<uint8_t> &packet,
                                      RegisterUpdatePacket &result) const {
  if (packet.size() < 4)
    return false;

  if (packet[0] != START_BYTE)
    return false;

  if (packet.back() != END_BYTE)
    return false;

  if (packet[1] != static_cast<uint8_t>(PacketTypePC::REG_UPDATE)) {
    return false;
  }

  uint8_t count = packet[2];

  size_t expectedSize = 3 + (static_cast<size_t>(count) * 3) + 2;

  if (packet.size() != expectedSize)
    return false;

  result.registers.clear();
  result.registers.reserve(count);

  for (uint8_t i = 0; i < count; ++i) {

    size_t index = 3 + (static_cast<size_t>(i) * 3);

    RegisterUpdate update;

    update.pinID = packet[index];

    update.value = static_cast<uint16_t>(packet[index + 1]) |
                   (static_cast<uint16_t>(packet[index + 2]) << 8);

    result.registers.push_back(update);
  }

  return true;
}

} // namespace vcm
