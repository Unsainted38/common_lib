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

## Usage

The library is intended to be used as a shared codebase for C++/Qt services that communicate with hardware devices over TCP or serial interfaces.
