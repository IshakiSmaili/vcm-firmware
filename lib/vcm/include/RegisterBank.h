#pragma once

#include "IRegisterProvider.h"
#include "Register.h"

#include <cstdint>
#include <vector>

namespace vcm {

class RegisterBank {
public:
  explicit RegisterBank(IRegisterProvider &provider);

  void refresh();

  RegisterInfo *find(uint8_t pinID);

  bool update(uint8_t pinID, uint16_t value);

  const std::vector<RegisterInfo> &all() const;

private:
  IRegisterProvider &provider;
  std::vector<RegisterInfo> registers;
};

} // namespace vcm
