#ifndef FAKE_CONNECTION_H
#define FAKE_CONNECTION_H

#include "FakeDataStructure.h"
#include <vector>
#include <cstdint>
#include <Arduino.h>


class FakeConnection {
public:
    // عدد السجلات
    int lengthRegisters;
    uint32_t timestamp;

    std::vector<Register> registers;

    // Constructor
    FakeConnection(int lengthRegisters, const std::vector<Register>& initialRegisters);

    // محاكاة إرسال MapRegisters إلى PC
    void sendMapRegistersToPC();

    // محاكاة استقبال بيانات من PC (مثل REG_UPDATE)
    // std::vector<uint8_t> readFromPC();
};

#endif // FAKE_CONNECTION_H