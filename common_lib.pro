TEMPLATE = lib
CONFIG += shared
#CONFIG += staticlib
TARGET = common_lib

include($$PWD/common.pri)

QT += core network
CONFIG += c++20

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
    cmd/um2_13_command.h \
    core/control/control_service.h \
    core/event/emergency_stop_event.h \
    core/event/event_priority.h \
    core/event/event_type.h \
    core/event/failure_event.h \
    core/event/i_event.h \
    core/event/init_completed_event.h \
    core/event/reset_event.h \
    core/event/shutdown_event.h \
    core/event/start_auto_tracking_event.h \
    core/event/start_extreme_tracking_event.h \
    core/event/start_moving_event.h \
    core/event/start_moving_to_point_event.h \
    core/event/start_target_designation_tracking_event.h \
    core/event/stop_event.h \
    core/queue/priority_event_queue.h \
    core/axis.h \
    core/driver_controller.h \
    core/state_machine/state/auto_tracking_state.h \
    core/state_machine/state/extreme_tracking_state.h \
    core/state_machine/state/handle_result.h \
    core/state_machine/state/i_state.h \
    core/state_machine/state/error_state.h \
    core/state_machine/state/idle_state.h \
    core/state_machine/state/initializing_state.h \
    core/state_machine/state/moving_state.h \
    core/state_machine/state/moving_to_point_state.h \
    core/state_machine/state/operational_state.h \
    core/state_machine/state/state_id.h \
    core/state_machine/state/target_designation_tracking_state.h \
    core/state_machine/state_machine.h \
    core/state_machine_controller.h \
    devices/bks_device.h \
    devices/compass_lcc5000_device.h \
    devices/dvt_device.h \
    devices/lir919d_device.h \
    devices/mshpr_device.h \
    devices/pvt_device.h \
    devices/ubpch_device.h \
    devices/um2_13_device.h \
    drivers/i_driver.h \
    drivers/vesper_driver.h \
    encoders/i_encoder.h \
    modbus/ModBusCmdTypes.h \
    modbus/ModBusDTO.h \
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
    network_transport/udp_transport.h \
    parsers/compass_lcc5000_parser.h \
    parsers/mshpr_parser.h \
    parsers/ubpch_parser.h \
    requesters/serial_circular_requester.h \
    telemetry/i_telemetry_packet_builder.h \
    telemetry/i_telemetry_packet_source.h \
    telemetry/i_telemetry_provider.h \
    telemetry/tcp_telemetry_server.h \
    telemetry/telemetry_packet_source.h \
    uacs_network_transport/myabstractconnect.h \
    uacs_network_transport/serialconnect.h \
    uacs_network_transport/tcpconnect.h \
    utilities/bit_utils.h \
    utilities/config_helper.h \
    utilities/console_utilities.h \
    utilities/random_utils.h

SOURCES += \
    algorithm/binary_coded_decimals_converter.cpp \
    algorithm/crc16.cpp \
    algorithm/crc8.cpp \
    cmd/abstract_command.cpp \
    cmd/compass_lcc5000_command.cpp \
    cmd/mshpr_command.cpp \
    cmd/ubpch_command.cpp \
    cmd/um2_13_command.cpp \
    core/control/control_service.cpp \
    core/event/emergency_stop_event.cpp \
    core/event/failure_event.cpp \
    core/event/init_completed_event.cpp \
    core/event/reset_event.cpp \
    core/event/shutdown_event.cpp \
    core/event/start_auto_tracking_event.cpp \
    core/event/start_extreme_tracking_event.cpp \
    core/event/start_moving_event.cpp \
    core/event/start_moving_to_point_event.cpp \
    core/event/start_target_designation_tracking_event.cpp \
    core/event/stop_event.cpp \
    core/queue/priority_event_queue.cpp \
    core/axis.cpp \
    core/driver_controller.cpp \
    core/state_machine/state/auto_tracking_state.cpp \
    core/state_machine/state/error_state.cpp \
    core/state_machine/state/extreme_tracking_state.cpp \
    core/state_machine/state/idle_state.cpp \
    core/state_machine/state/initializing_state.cpp \
    core/state_machine/state/moving_state.cpp \
    core/state_machine/state/moving_to_point_state.cpp \
    core/state_machine/state/operational_state.cpp \
    core/state_machine/state/target_designation_tracking_state.cpp \
    core/state_machine/state_machine.cpp \
    core/state_machine_controller.cpp \
    devices/bks_device.cpp \
    devices/compass_lcc5000_device.cpp \
    devices/dvt_device.cpp \
    devices/lir919d_device.cpp \
    devices/mshpr_device.cpp \
    devices/pvt_device.cpp \
    devices/ubpch_device.cpp \
    devices/um2_13_device.cpp \
    drivers/vesper_driver.cpp \
    modbus/abstract_modbus_device.cpp \
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
    network_transport/udp_transport.cpp \
    parsers/compass_lcc5000_parser.cpp \
    parsers/mshpr_parser.cpp \
    parsers/ubpch_parser.cpp \
    requesters/serial_circular_requester.cpp \
    telemetry/tcp_telemetry_server.cpp \
    uacs_network_transport/myabstractconnect.cpp \
    uacs_network_transport/serialconnect.cpp \
    uacs_network_transport/tcpconnect.cpp \
    utilities/config_helper.cpp \
    utilities/console_utilities.cpp


DEFINES += COMMON_LIBRARY

DISTFILES += \
    common_lib.pri
