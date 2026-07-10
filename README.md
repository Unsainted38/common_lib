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

## Tech Stack

- C++
- Qt Core
- Qt Network
- QMake
- TCP/serial communication
- Modbus

## Architecture

Application
   ↓
Proxy server / requester
   ↓
Network transport
   ↓
Command / parser layer
   ↓
Hardware device or external service

## Used By

This library is used by hardware-oriented C++/Qt services and applications:

- `bks_server` — TCP proxy server for hardware device control.
- `Htra_console` — console service for spectrum analyzer control, telemetry publishing and command processing.

These projects use `common_lib` as a shared foundation for transport abstractions, request/response processing, device communication and protocol-related utilities.
