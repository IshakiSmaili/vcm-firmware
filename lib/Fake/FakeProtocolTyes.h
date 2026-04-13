#ifndef FAKE_PROTOCOL_TYPES_H
#define FAKE_PROTOCOL_TYPES_H

#include <cstdint>
namespace fake
{
    const uint8_t START_BYTE = 0xAA;
    const uint8_t END_BYTE = 0x55;

    enum class PacketTypeMCU : uint8_t
    {
        REG_BATCH = 0x01,
        ACK = 0x02,
        NAK = 0x03
    };

    enum class PacketTypePC : uint8_t
    {
        REG_UPDATE = 0x04,
    };

    enum class RegisterType : uint8_t
    {
        DIGITAL_INPUT = 0x01,
        DIGITAL_OUTPUT = 0x02,
        ANALOG_INPUT = 0x03,
        ANALOG_OUTPUT = 0x04,
        PWM_OUTPUT = 0x05,
        SPECIAL_FUNCTION = 0x06
    };

    struct Register
    {
        uint8_t pinID;     // Identifier for the pin/register
        uint16_t value;    // Current value
        bool isInput;      // true: read-only, false: writeable
        RegisterType type; // Type of the pin/register
        uint16_t minValue; // Minimum allowed value
        uint16_t maxValue; // Maximum allowed value
    };

    struct MapRegistersPacket
    {
        PacketTypeMCU type;
        uint8_t length;
        Register *Registers;
        uint8_t crc;
    };

    struct RegisterUpdatePacket
    {
        PacketTypePC type;
        uint8_t length;
        Register *registers;
        uint8_t crc;
    };
}
#endif