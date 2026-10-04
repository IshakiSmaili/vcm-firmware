#ifndef VCM_PROTOCOL_TYPES_H
#define VCM_PROTOCOL_TYPES_H

#include <cstdint>

namespace vcm {

enum class RegisterType : uint8_t {
  DIGITAL_INPUT = 0x01,
  DIGITAL_OUTPUT = 0x02,
  ANALOG_INPUT = 0x03,
  ANALOG_OUTPUT = 0x04,
  PWM_OUTPUT = 0x05,
  SPECIAL_FUNCTION = 0x06
};

// Description sent to / used by the PC.
struct RegisterInfo {
  uint8_t pinID;
  uint16_t value;

  // bool isInput;
  RegisterType type;

  uint16_t minValue;
  uint16_t maxValue;
};

// Actual MCU register.
// The value is read/written using the hardware address.
struct McuRegister {
  uint8_t pinID;

  uintptr_t address;
  uint32_t mask;

  // bool isInput;
  RegisterType type;

  uint16_t minValue;
  uint16_t maxValue;
};
} // namespace vcm

#endif
