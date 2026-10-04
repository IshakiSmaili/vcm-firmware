#include "SerialTransport.h"
#include "VcmPackets.h"

#include <Arduino.h>

namespace vcm {

void SerialTransport::send(const std::vector<uint8_t> &data) {
  if (data.empty())
    return;

  Serial.write(data.data(), data.size());
}

std::vector<uint8_t> SerialTransport::receive() {

  while (Serial.available()) {

    uint8_t byte = Serial.read();

    buffer.push_back(byte);

    if (buffer.size() < 3)
      continue;

    if (buffer[0] != START_BYTE) {
      buffer.clear();
      continue;
    }

    uint8_t messageType = buffer[1];

    // Currently MCU accepts REG_UPDATE
    if (messageType != static_cast<uint8_t>(PacketTypePC::REG_UPDATE)) {
      buffer.erase(buffer.begin());

      continue;
    }

    uint8_t registerCount = buffer[2];

    size_t expectedSize =
        3 + (static_cast<size_t>(registerCount) * 3) + 2; // CRC + END

    if (buffer.size() < expectedSize)
      continue;

    if (buffer[expectedSize - 1] != END_BYTE) {
      buffer.erase(buffer.begin());

      continue;
    }

    std::vector<uint8_t> packet(buffer.begin(), buffer.begin() + expectedSize);

    buffer.erase(buffer.begin(), buffer.begin() + expectedSize);

    return packet;
  }

  return {};
}

} // namespace vcm
