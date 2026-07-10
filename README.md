# common_lib

Reusable C++/Qt library for hardware-oriented applications, network communication, protocol parsing and device command processing.

## Overview

`common_lib` contains reusable modules used in C++/Qt applications for device communication, network transport, Modbus commands, request/response processing and binary protocol parsing.

## Modules

- `network_transport` — TCP/serial transport abstractions.
- `modbus` — Modbus-related commands and helpers.
- `parsers` — binary protocol parsing.
- `requesters` — request/response processing.
- `proxy_servers` — proxy server components.
- `devices` — device-level abstractions.
- `utilities` — shared utility functions.
- `algorithm` — common algorithms and helpers.

Typical use cases:

- TCP proxy server between an external client and a hardware device.
- Serial device communication with request/response processing.
- UDP/TCP transport layer for telemetry and command exchange.
- Modbus-based command processing.
- Binary protocol parsing.
- Shared device abstractions for multiple Qt services.
- Telemetry data collection and packet building.

## Tech Stack

- C++
- Qt Core
- Qt Network
- QMake
- TCP/serial communication
- Modbus

## Architecture

Typical architecture of an application using `common_lib`:

```text
External client / control system
        ↓
Application service
        ↓
Proxy server / requester
        ↓
Network transport
        ↓
Command / parser layer
        ↓
Hardware device or external service
```

## Used By

This library is used by hardware-oriented C++/Qt services and applications:

- `bks_server` — TCP proxy server for hardware device control.
- `Htra_console` — console service for spectrum analyzer control, telemetry publishing and command processing.

These projects use `common_lib` as a shared foundation for transport abstractions, request/response processing, device communication and protocol-related utilities.

## Example Usage

A typical service based on `common_lib` may create a transport object, pass it to a requester and use command objects to communicate with a device.

Conceptual example:

```cpp
#include <QCoreApplication>

#include "devices/bks_device.h"
#include "requesters/serial_circular_requester.h"
#include "network_transport/abstract_network_transport_factory.h"

// Project-specific includes depend on the selected transport/requester/device type.
// This example demonstrates the intended usage pattern.

int main(int argc, char *argv[])
{
   QCoreApplication app(argc, argv);
   // AbstractNetworkTransport implements an interface for interacting with specific transport-layer network protocols.
   AbstractNetworkTransport *transport = AbstractNetworkTransportFactory::getInstance("Path/to/config.ini", "NameOfConfigSection");
   NetworkTransportLocker *locker = new NetworkTransportLocker(MaxResponceWaitingTime, MinTransportWritingTimeout);
   // SerialCircularRequester contains List<AbstractCommand*> and provides consistent sending commands via AbstractNetworkTransport
   SerialCircularRequester *requester = new SerialCircularRequester(transport, locker);
   BksDevice *m_device = new BksDevice(requester, "/home/user/develop/playground/etc/device/config.ini", "BksDevice");
   // Device contains AbstractCommand* type objects. Each of them represents information about specific command to write in transport
   // and stores execution result.

   return app.exec();
}
```
