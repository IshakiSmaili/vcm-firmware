#include "Vcm.h"

namespace vcm {

Vcm::Vcm()
    : registerProvider(), registerBank(registerProvider),
      vcmController(serialTransport, vcmProtocol, registerBank) {}

void Vcm::initMapRegister(const McuRegisterProvider::RegisterMap &registerMap) {

  registerProvider.initMapRegister(registerMap);
  registerBank.refresh();
}

void Vcm::begin() {}

void Vcm::processIncoming() { vcmController.processIncoming(); }

void Vcm::sendRegisters() { vcmController.sendRegisters(); }

} // namespace vcm
