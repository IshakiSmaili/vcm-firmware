# Fake Drivers – Virtual I/O & Register Injection Layer for MCUs

## 📌 Overview

**Fake Drivers** is a middleware layer that runs on top of the MCU, designed to simulate hardware devices and enable real-time interaction with MCU registers — **without modifying the original firmware**.

> 🚨 The PC is **NOT a controller**.
> It acts only as a **sensor/monitor**, meaning any data sent from the PC is treated strictly as **register value updates**.

---

## 🎯 Current Scope

🚧 This repository currently implements:

* ✅ FakeLayer (MCU Middleware)
* ❌ GUI (planned, external)
* ❌ Full device simulation (future work)

---

## 🧩 Architecture

```
PC (Sensor/Monitor)
        │
        ▼
FakeLayer (Middleware on MCU)
        │
        ▼
Map Registers (Validated State)
        │
        ▼
Original Firmware (Unmodified)
```

---

## ⚙️ How It Works

The FakeLayer operates inside the MCU main loop:

```cpp
void loop() {
    readPcFromPC();           // Apply register updates from PC
    yourFirmware();           // Original firmware logic (unchanged)
    sendMapRegistersToPc();   // Send full state to PC
}
```

---

## 🧱 Core Concept: Map Registers

All interactions are done through a structured register map.

```cpp
enum RegisterType : uint8_t {
    DIGITAL_INPUT,
    DIGITAL_OUTPUT,
    ANALOG_INPUT,
    ANALOG_OUTPUT,
    PWM_OUTPUT,
    SPECIAL_FUNCTION
};

struct Register {
    uint8_t pinID;
    uint16_t value;
    bool isInput;
    RegisterType type;
    uint16_t minValue;
    uint16_t maxValue;
};
```

### ✨ Purpose

* Represent all MCU I/O in one unified structure
* Allow batch communication with PC
* Validate all incoming data (min/max, type, access)
* Keep firmware fully isolated from simulation logic

---

## 🔁 Data Flow

### PC → MCU

* Sends **register updates only**
* No commands, no control logic

### MCU → PC

* Sends full **Map Registers batch**
* Used for monitoring and visualization

---

## 📡 Communication Protocol

### Message Types

| Type       | Direction | Description              |
| ---------- | --------- | ------------------------ |
| REG_UPDATE | PC → MCU  | Update register values   |
| REG_BATCH  | MCU → PC  | Send all register states |
| ACK        | MCU → PC  | Acknowledge valid update |
| NAK        | MCU → PC  | Reject invalid update    |

---

### REG_UPDATE

```
[START][TYPE][NUM][DATA...][CHECKSUM]
```

```cpp
struct RegUpdate {
    uint8_t pinID;
    uint16_t value;
};
```

---

### REG_BATCH

```cpp
struct RegFull {
    uint8_t pinID;
    uint16_t value;
    uint8_t type;
    bool isInput;
    uint16_t minValue;
    uint16_t maxValue;
};
```

---

## 🔒 Safety Rules

* ❗ PC cannot execute logic
* ❗ PC cannot send commands
* ✅ All values are validated before applying
* ✅ Firmware always reads trusted data

---

## 🚀 Why FakeLayer?

* Test firmware **without hardware**
* Simulate sensors and devices easily
* Debug register-level behavior in real time
* No need to re-flash firmware for testing
* Decoupled architecture (GUI optional)

---

## 🛠️ Current Status

* 🟢 Register Map implemented
* 🟢 Protocol structure defined
* 🟡 Serial communication (in progress)
* 🔴 GUI simulator (not started)

---

## 📦 Future Work

* External GUI (device simulation)
* Plugin system for components (TFT, buttons, sensors)
* Advanced validation (timing, dependencies)
* Multi-MCU support
* Logging & debugging tools

---

## 🤝 Contributing

This project is intended to become an open-source platform for embedded system simulation.

Contributions are welcome in:

* Protocol improvements
* Performance optimization
* GUI development
* Device simulation modules

---

## 🧠 Philosophy

> The MCU owns the logic.
> The PC only reflects reality.
