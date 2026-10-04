#pragma once

#include "IVcmTransport.h"
#include "RegisterBank.h"
#include "VcmProtocol.h"

#include <cstdint>

namespace vcm {

class VcmController {
public:
  VcmController(IVcmTransport &transport, VcmProtocol &protocol,
                RegisterBank &registerBank);

  void sendRegisters();

  void processIncoming();

  void setSendInterval(uint32_t interval);

private:
  void sendAckNak(PacketTypeMCU type);

private:
  IVcmTransport &transport;
  VcmProtocol &protocol;
  RegisterBank &registerBank;

  uint32_t lastSendTime;
  uint32_t sendIntervalMs;
};

} // namespace vcm
