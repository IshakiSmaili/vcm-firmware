#pragma once

#include "IRegisterProvider.h"

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace vcm {

class McuRegisterProvider : public IRegisterProvider {
public:
  using RegisterMap = std::unordered_map<uint8_t, McuRegister>;

  McuRegisterProvider();

  void initMapRegister(const RegisterMap &registerMap);

  std::vector<RegisterInfo> readRegisters() override;

  McuRegister *findRegister(uint8_t pinID) override;

  bool updateRegister(uint8_t pinID, uint16_t value) override;

private:
  RegisterMap registers;
};

} // namespace vcm
