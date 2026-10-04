#include "VcmController.h"

#include <Arduino.h>

namespace vcm {

VcmController::VcmController(IVcmTransport &transport, VcmProtocol &protocol,
                             RegisterBank &registerBank)
    : transport(transport), protocol(protocol), registerBank(registerBank),
      lastSendTime(0), sendIntervalMs(1000) {}

void VcmController::sendRegisters() {
  uint32_t nowTime = millis();

  if (nowTime - lastSendTime < sendIntervalMs)
    return;

  lastSendTime = nowTime;

  const auto &registers = registerBank.all();

  std::vector<uint8_t> packet = protocol.createRegBatch(
      registers.data(), static_cast<uint8_t>(registers.size()), nowTime);

  transport.send(packet);
}

void VcmController::processIncoming() {
  std::vector<uint8_t> packet = transport.receive();

  if (packet.empty())
    return;

  if (!protocol.validateCRC(packet)) {
    sendAckNak(PacketTypeMCU::NAK);
    return;
  }

  if (packet[1] != static_cast<uint8_t>(PacketTypePC::REG_UPDATE)) {
    sendAckNak(PacketTypeMCU::NAK);
    return;
  }

  RegisterUpdatePacket update;

  if (!protocol.parseRegisterUpdate(packet, update)) {
    sendAckNak(PacketTypeMCU::NAK);
    return;
  }

  for (const auto &item : update.registers) {
    if (!registerBank.update(item.pinID, item.value)) {
      sendAckNak(PacketTypeMCU::NAK);
      return;
    }
  }

  sendAckNak(PacketTypeMCU::ACK);
}

void VcmController::sendAckNak(PacketTypeMCU type) {
  uint32_t timestamp = millis();

  std::vector<uint8_t> packet = protocol.createAckNak(type, timestamp);

  transport.send(packet);
}

void VcmController::setSendInterval(uint32_t interval) {
  sendIntervalMs = interval;
}

} // namespace vcm
