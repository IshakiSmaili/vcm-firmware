#pragma once

// #include "IVcmTransport.h"
// #include "Register.h"
#include "RegisterBank.h"
#include "SerialTransport.h"
#include "VcmController.h"
// #include "VcmPackets.h"
#include "McuRegisterProvider.h"
#include "VcmProtocol.h"

namespace vcm {

class Vcm {
public:
  Vcm();

  void initMapRegister(const McuRegisterProvider::RegisterMap &registerMap);

  void begin();

  void processIncoming();
  void sendRegisters();

private:
  SerialTransport serialTransport;
  VcmProtocol vcmProtocol;
  McuRegisterProvider registerProvider;
  RegisterBank registerBank;
  VcmController vcmController;
};

} // namespace vcm
