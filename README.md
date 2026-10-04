# VCM Firmware - Virtual Components for Microcontrollers

## Overview

**VCM (Virtual Components for Microcontrollers)** is a lightweight middleware layer for microcontrollers that enables firmware to interact with virtualized hardware components through a PC.

VCM allows embedded firmware to be tested and developed with virtual inputs and simulated components while keeping the application logic on the microcontroller.

> **The MCU owns the application logic.**
> **The PC provides virtual component data and monitors the MCU state.**

The goal is to make embedded firmware testable without requiring every physical component during development.


## Current Scope

This repository contains the **firmware-side implementation of VCM**.

* VCM middleware
* Register map
* Communication protocol
* MCU ↔ PC communication
* Register validation
* Additional virtual components
* GUI — maintained separately in [`vcm-gui`](../vcm-gui)

---

## 🧩 Architecture

```text
                 PC
                  │
        Virtual Components
                  │
                  ▼
          ┌───────────────┐
          │   VCM Layer   │
          │               │
          │ Protocol      │
          │ Register Map  │
          │ Validation    │
          └───────┬───────┘
                  │
                  ▼
          Original Firmware
                  │
                  ▼
             MCU Hardware
```

VCM sits between the communication layer and the application firmware.

The application firmware remains responsible for its own logic and behavior.


## How It Works

VCM operates alongside the normal MCU firmware loop.

```cpp
void loop() {
    vcm.processIncoming();       // Process virtual component data
    yourFirmware();              // Application firmware logic
    vcm.sendRegisters();         // Send MCU state to the PC
}
```

VCM does not replace the application's logic.

Instead, it provides a bridge between virtual components and the firmware's I/O state.

---

## 🧱 Core Concept: Register Map

VCM represents MCU I/O through a structured register map.

```cpp
enum RegisterType : uint8_t {
    DIGITAL_INPUT,
    DIGITAL_OUTPUT,
    ANALOG_INPUT,
    ANALOG_OUTPUT,
    PWM_OUTPUT,
    SPECIAL_FUNCTION
};
```

Each register describes an I/O value and its properties.

```cpp
struct Register {
    uint8_t pinID;
    uint16_t value;
    RegisterType type;
    uint16_t minValue;
    uint16_t maxValue;
};
```

### Purpose

The register map provides:

* A unified representation of MCU I/O
* Structured communication with the PC
* Value validation
* Type information
* Support for batch updates
* A clear boundary between VCM and application firmware

---

## Data Flow

### PC → MCU

The PC can provide values representing virtual component states.

For example:

```text
Virtual Button
      │
      ▼
   GUI / PC
      │
      ▼
 REG_UPDATE
      │
      ▼
    VCM
      │
      ▼
 Register Map
      │
      ▼
 Application Firmware
```

The MCU validates incoming values before applying them.

---

### MCU → PC

The MCU periodically publishes its current register state.

```text
Application Firmware
        │
        ▼
   Register Map
        │
        ▼
    REG_BATCH
        │
        ▼
       PC
        │
        ▼
 GUI / Monitoring
```

This allows the PC to observe the behavior of the firmware in real time.


## Design Principles

### MCU Owns the Logic

VCM does not move application logic to the PC.

The firmware remains responsible for:

* State transitions
* Control logic
* Timing
* Decision making
* Hardware behavior

### PC Provides Virtual Components

The PC can represent components such as:

* Buttons
* Switches
* Sensors
* Analog inputs
* Other virtual inputs

The GUI communicates these values to VCM rather than directly controlling the application.

## Why VCM?

VCM is designed to make embedded development easier by reducing the dependency on physical hardware during development and testing.

### Benefits

* Test firmware without connecting every physical component
* Simulate sensors and inputs
* Observe MCU state in real time
* Reduce hardware dependency during development
* Test firmware behavior repeatedly
* Keep application logic on the MCU
* Separate virtual components from firmware logic
* Provide a consistent communication layer

## Future Work
Build a GUI system that makes creating and configuring custom virtual components easy.


## Contributing

VCM is intended to become an open-source platform for developing and testing embedded firmware with virtual hardware components.

Contributions are welcome in areas such as:

* Protocol improvements
* Firmware architecture
* Performance optimization
* New virtual component types
* Communication transports
* Testing
* Documentation

---

## Philosophy

> **The MCU owns the logic.**
> **VCM connects the firmware to virtual components.**
> **The PC provides the environment.**
