TEMPLATE = lib
CONFIG += shared
#CONFIG += staticlib
TARGET = common_lib

include($$PWD/../common.pri)

QT += core
QT += network
CONFIG += c++23

INCLUDEPATH += \
            $$PWD \
            $$PWD/cmd \
            $$PWD/algorithm \
            $$PWD/network_transport \
            $$PWD/requesters \
            $$PWD/devices \
            $$PWD/parsers \
            $$PWD/uacs_network_transport \


HEADERS += \
    algorithm/binary_coded_decimals_converter.h \
    algorithm/crc16.h \
    algorithm/crc8.h \
    cmd/abstract_command.h \
    cmd/compass_lcc5000_command.h \
    cmd/mshpr_command.h \
    cmd/ubpch_command.h \
    devices/bks_device.h \
    devices/compass_lcc5000_device.h \
    devices/dvt_device.h \
    devices/mshpr_device.h \
    devices/ubpch_device.h \
    modbus/ModBusCmdTypes.h \
    modbus/ModBusDto.h \
    modbus/abstract_modbus_device.h \
    modbus/abstract_modbus_protocol.h \
    modbus/modbus_protocol_factory.h \
    modbus/modbus_rtu.h \
    modbus/modbus_tcp.h \
    modbus/read_coils.h \
    modbus/read_descrete_inputs.h \
    modbus/read_holding_registers.h \
    modbus/read_input_registers.h \
    modbus/write_multiple_coils.h \
    modbus/write_multiple_registers.h \
    modbus/write_single_coil.h \
    modbus/write_single_register.h \
    network_transport/abstract_network_transport.h \
    network_transport/abstract_network_transport_factory.h \
    network_transport/network_transport_locker.h \
    network_transport/serial_transport.h \
    network_transport/tcp_transport.h \
    parsers/compass_lcc5000_parser.h \
    parsers/mshpr_parser.h \
    parsers/ubpch_parser.h \
    requesters/serial_circular_requester.h \
    uacs_network_transport/myabstractconnect.h \
    uacs_network_transport/serialconnect.h \
    uacs_network_transport/tcpconnect.h \
    utilities/BitUtils.h \
    utilities/config_helper.h

SOURCES += \
    algorithm/binary_coded_decimals_converter.cpp \
    algorithm/crc16.cpp \
    algorithm/crc8.cpp \
    cmd/abstract_command.cpp \
    cmd/compass_lcc5000_command.cpp \
    cmd/mshpr_command.cpp \
    cmd/ubpch_command.cpp \
    devices/bks_device.cpp \
    devices/compass_lcc5000_device.cpp \
    devices/dvt_device.cpp \
    devices/mshpr_device.cpp \
    devices/ubpch_device.cpp \
    modbus/abstract_modbus_device.cpp \
    modbus/abstract_modbus_protocol.cpp \
    modbus/modbus_protocol_factory.cpp \
    modbus/modbus_rtu.cpp \
    modbus/modbus_tcp.cpp \
    modbus/read_coils.cpp \
    modbus/read_descrete_inputs.cpp \
    modbus/read_holding_registers.cpp \
    modbus/read_input_registers.cpp \
    modbus/write_multiple_coils.cpp \
    modbus/write_multiple_registers.cpp \
    modbus/write_single_coil.cpp \
    modbus/write_single_register.cpp \
    network_transport/abstract_network_transport.cpp \
    network_transport/abstract_network_transport_factory.cpp \
    network_transport/serial_transport.cpp \
    network_transport/tcp_transport.cpp \
    parsers/compass_lcc5000_parser.cpp \
    parsers/mshpr_parser.cpp \
    parsers/ubpch_parser.cpp \
    requesters/serial_circular_requester.cpp \
    uacs_network_transport/myabstractconnect.cpp \
    uacs_network_transport/serialconnect.cpp \
    uacs_network_transport/tcpconnect.cpp \
    utilities/config_helper.cpp


DEFINES += COMMON_LIBRARY

DISTFILES += \
    common_lib.pri
