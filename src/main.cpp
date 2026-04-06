// #include <Arduino.h>
// // #include <FakeConnection.h>

// // FakeConnection fake;

// String generateRandomText(int length)
// {
//     String result = "";
//     const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
//     for (int i = 0; i < length; i++)
//     {
//         int index = random(0, sizeof(charset) - 1);
//         result += charset[index];
//     }
//     return result;
// }

// void sendTextToPC(String text, long time)
// {
//     Serial.println(text);
//     Serial.print(time);
// }

// #include <Arduino.h>

// void setup()
// {
//     Serial.begin(9600);
//     while (!Serial)
//     Serial.println("MCU Ready");
// }

// void loop()
// {

//     if (Serial.available() > 0)
//     {
//         String received = Serial.readStringUntil('\n');
//         received.trim();

//         unsigned long t = millis();

//         if (received.length() > 0)
//         {
//             sendTextToPC("ACK", t);
//         }
//         else
//         {
//             sendTextToPC("NAK", t);
//         }
//     }
// }


#include <Arduino.h>
#include "FakeConnection.h"

const int NUM_REGISTERS = 2;

std::vector<Register> initialRegisters = {
    { 1, 100, true, RegisterType::DIGITAL_INPUT, 0, 255 },
    { 2, 50, false, RegisterType::ANALOG_OUTPUT, 0, 1023 }
};

FakeConnection* fake; // مؤشر

void setup() {
    Serial.begin(115200);

    fake = new FakeConnection(NUM_REGISTERS, initialRegisters);
}

void loop() {
    fake->sendMapRegistersToPC();
    delay(1000);
}