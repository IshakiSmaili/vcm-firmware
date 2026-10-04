#include "McuRegisterProvider.h"

namespace vcm {

McuRegisterProvider::McuRegisterProvider() = default;

void McuRegisterProvider::initMapRegister(const RegisterMap &registerMap) {

  registers = registerMap;
}

std::vector<RegisterInfo> McuRegisterProvider::readRegisters() {
  std::vector<RegisterInfo> result;

  result.reserve(registers.size());

  for (const auto &entry : registers) {
    const McuRegister &reg = entry.second;

    uint16_t value = 0;

    if (reg.address != 0) {
      volatile uint32_t *hardware =
          reinterpret_cast<volatile uint32_t *>(reg.address);

      value = ((*hardware & reg.mask) != 0) ? 1 : 0;
    }

    result.push_back({
        reg.pinID,
        value,
        reg.type,
        reg.minValue,
        reg.maxValue,
    });
  }

  return result;
}

McuRegister *McuRegisterProvider::findRegister(uint8_t pinID) {
  auto it = registers.find(pinID);

  if (it == registers.end())
    return nullptr;

  return &it->second;
}

bool McuRegisterProvider::updateRegister(uint8_t pinID, uint16_t value) {

  McuRegister *reg = findRegister(pinID);

  if (reg == nullptr || reg->address == 0)
    return false;

  if (value < reg->minValue || value > reg->maxValue)
    return false;

  volatile uint32_t *hardware =
      reinterpret_cast<volatile uint32_t *>(reg->address);

  if (value != 0)
    *hardware |= reg->mask;
  else
    *hardware &= ~reg->mask;

  return true;
}

} // namespace vcm
