#pragma once

#include "Register.h"
#include "VcmPackets.h"

#include <cstdint>
#include <vector>

namespace vcm {

class VcmProtocol {
public:
  std::vector<uint8_t> createRegBatch(const RegisterInfo *registers,
                                      uint8_t count, uint32_t timestamp) const;

  std::vector<uint8_t> createAckNak(PacketTypeMCU type,
                                    uint32_t timestamp) const;

  bool validateCRC(const std::vector<uint8_t> &packet) const;

  bool parseRegisterUpdate(const std::vector<uint8_t> &packet,
                           RegisterUpdatePacket &result) const;

private:
  uint8_t calculateCRC(const std::vector<uint8_t> &packet) const;
};

} // namespace vcm
