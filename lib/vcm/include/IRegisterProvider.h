#pragma once

#include "Register.h"

#include <cstdint>
#include <vector>

namespace vcm {

class IRegisterProvider {
public:
  virtual ~IRegisterProvider() = default;

  // Read the MCU registers and return snapshots
  // containing the current hardware values.
  virtual std::vector<RegisterInfo> readRegisters() = 0;

  // Find the actual MCU register.
  virtual McuRegister *findRegister(uint8_t pinID) = 0;

  // Update the actual MCU hardware register.
  virtual bool updateRegister(uint8_t pinID, uint16_t value) = 0;
};

} // namespace vcm
