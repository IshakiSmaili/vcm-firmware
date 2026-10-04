#include "RegisterBank.h"

namespace vcm {

RegisterBank::RegisterBank(IRegisterProvider &provider) : provider(provider) {
  refresh();
}

void RegisterBank::refresh() { registers = provider.readRegisters(); }

RegisterInfo *RegisterBank::find(uint8_t pinID) {
  for (auto &reg : registers) {
    if (reg.pinID == pinID)
      return &reg;
  }

  return nullptr;
}

bool RegisterBank::update(uint8_t pinID, uint16_t value) {
  if (!provider.updateRegister(pinID, value))
    return false;

  // Re-read the actual value from MCU hardware.
  refresh();

  return true;
}

const std::vector<RegisterInfo> &RegisterBank::all() const { return registers; }

} // namespace vcm
