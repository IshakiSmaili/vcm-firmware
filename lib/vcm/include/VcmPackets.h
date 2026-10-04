#pragma once

#include <cstdint>
#include <vector>

namespace vcm {

constexpr uint8_t START_BYTE = 0xAA;
constexpr uint8_t END_BYTE = 0x55;

enum class PacketTypeMCU : uint8_t { REG_BATCH = 0x01, ACK = 0x02, NAK = 0x03 };

enum class PacketTypePC : uint8_t { REG_UPDATE = 0x04 };

struct RegisterUpdate {
  uint8_t pinID;
  uint16_t value;
};

struct RegisterUpdatePacket {
  std::vector<RegisterUpdate> registers;
};

} // namespace vcm
