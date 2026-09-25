TEMPLATE = lib
CONFIG += shared
#CONFIG += staticlib
TARGET = common_lib
LIB_INSTALL_DIR = /opt/common_lib/lib/
COMMON_LIB_ROOT = $$_PRO_FILE_PWD_

include($$PWD/common_build.pri)

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
    $$COMMON_LIB_ROOT/algorithm/binary_coded_decimals_converter.h \
    $$COMMON_LIB_ROOT/algorithm/crc16.h \
    $$COMMON_LIB_ROOT/algorithm/crc8.h \
    $$COMMON_LIB_ROOT/cmd/abstract_command.h \
    $$COMMON_LIB_ROOT/cmd/compass_lcc5000_command.h \
    $$COMMON_LIB_ROOT/cmd/mshpr_command.h \
    $$COMMON_LIB_ROOT/cmd/ubpch_command.h \
    $$COMMON_LIB_ROOT/cmd/um2_13_command.h \
    $$COMMON_LIB_ROOT/core/control/control_service.h \
    $$COMMON_LIB_ROOT/core/event/emergency_stop_event.h \
    $$COMMON_LIB_ROOT/core/event/event_priority.h \
    $$COMMON_LIB_ROOT/core/event/event_type.h \
    $$COMMON_LIB_ROOT/core/event/failure_event.h \
    $$COMMON_LIB_ROOT/core/event/i_event.h \
    $$COMMON_LIB_ROOT/core/event/init_completed_event.h \
    $$COMMON_LIB_ROOT/core/event/reset_event.h \
    $$COMMON_LIB_ROOT/core/event/shutdown_event.h \
    $$COMMON_LIB_ROOT/core/event/start_auto_tracking_event.h \
    $$COMMON_LIB_ROOT/core/event/start_extreme_tracking_event.h \
    $$COMMON_LIB_ROOT/core/event/start_moving_event.h \
    $$COMMON_LIB_ROOT/core/event/start_moving_to_point_event.h \
    $$COMMON_LIB_ROOT/core/event/start_target_designation_tracking_event.h \
    $$COMMON_LIB_ROOT/core/event/stop_event.h \
    $$COMMON_LIB_ROOT/core/queue/priority_event_queue.h \
    $$COMMON_LIB_ROOT/core/axis.h \
    $$COMMON_LIB_ROOT/core/driver_controller.h \
    $$COMMON_LIB_ROOT/core/state_machine/state/auto_tracking_state.h \
    $$COMMON_LIB_ROOT/core/state_machine/state/extreme_tracking_state.h \
    $$COMMON_LIB_ROOT/core/state_machine/state/handle_result.h \
    $$COMMON_LIB_ROOT/core/state_machine/state/i_state.h \
    $$COMMON_LIB_ROOT/core/state_machine/state/error_state.h \
    $$COMMON_LIB_ROOT/core/state_machine/state/idle_state.h \
    $$COMMON_LIB_ROOT/core/state_machine/state/initializing_state.h \
    $$COMMON_LIB_ROOT/core/state_machine/state/moving_state.h \
    $$COMMON_LIB_ROOT/core/state_machine/state/moving_to_point_state.h \
    $$COMMON_LIB_ROOT/core/state_machine/state/operational_state.h \
    $$COMMON_LIB_ROOT/core/state_machine/state/state_id.h \
    $$COMMON_LIB_ROOT/core/state_machine/state/target_designation_tracking_state.h \
    $$COMMON_LIB_ROOT/core/state_machine/state_machine.h \
    $$COMMON_LIB_ROOT/core/state_machine_controller.h \
    $$COMMON_LIB_ROOT/devices/bks_device.h \
    $$COMMON_LIB_ROOT/devices/compass_lcc5000_device.h \
    $$COMMON_LIB_ROOT/devices/dvt_device.h \
    $$COMMON_LIB_ROOT/devices/lir919d_device.h \
    $$COMMON_LIB_ROOT/devices/mshpr_device.h \
    $$COMMON_LIB_ROOT/devices/pvt_device.h \
    $$COMMON_LIB_ROOT/devices/ubpch_device.h \
    $$COMMON_LIB_ROOT/devices/um2_13_device.h \
    $$COMMON_LIB_ROOT/drivers/i_driver.h \
    $$COMMON_LIB_ROOT/drivers/vesper_driver.h \
    $$COMMON_LIB_ROOT/encoders/i_encoder.h \
    $$COMMON_LIB_ROOT/modbus/ModBusCmdTypes.h \
    $$COMMON_LIB_ROOT/modbus/ModBusDTO.h \
    $$COMMON_LIB_ROOT/modbus/abstract_modbus_device.h \
    $$COMMON_LIB_ROOT/modbus/abstract_modbus_protocol.h \
    $$COMMON_LIB_ROOT/modbus/modbus_protocol_factory.h \
    $$COMMON_LIB_ROOT/modbus/modbus_rtu.h \
    $$COMMON_LIB_ROOT/modbus/modbus_tcp.h \
    $$COMMON_LIB_ROOT/modbus/read_coils.h \
    $$COMMON_LIB_ROOT/modbus/read_descrete_inputs.h \
    $$COMMON_LIB_ROOT/modbus/read_holding_registers.h \
    $$COMMON_LIB_ROOT/modbus/read_input_registers.h \
    $$COMMON_LIB_ROOT/modbus/write_multiple_coils.h \
    $$COMMON_LIB_ROOT/modbus/write_multiple_registers.h \
    $$COMMON_LIB_ROOT/modbus/write_single_coil.h \
    $$COMMON_LIB_ROOT/modbus/write_single_register.h \
    $$COMMON_LIB_ROOT/network_transport/abstract_network_transport.h \
    $$COMMON_LIB_ROOT/network_transport/abstract_network_transport_factory.h \
    $$COMMON_LIB_ROOT/network_transport/network_transport_locker.h \
    $$COMMON_LIB_ROOT/network_transport/serial_transport.h \
    $$COMMON_LIB_ROOT/network_transport/tcp_transport.h \
    $$COMMON_LIB_ROOT/network_transport/udp_transport.h \
    $$COMMON_LIB_ROOT/parsers/compass_lcc5000_parser.h \
    $$COMMON_LIB_ROOT/parsers/mshpr_parser.h \
    $$COMMON_LIB_ROOT/parsers/ubpch_parser.h \
    $$COMMON_LIB_ROOT/requesters/serial_circular_requester.h \
    $$COMMON_LIB_ROOT/telemetry/i_telemetry_packet_builder.h \
    $$COMMON_LIB_ROOT/telemetry/i_telemetry_packet_source.h \
    $$COMMON_LIB_ROOT/telemetry/i_telemetry_provider.h \
    $$COMMON_LIB_ROOT/telemetry/tcp_telemetry_server.h \
    $$COMMON_LIB_ROOT/telemetry/telemetry_packet_source.h \
    $$COMMON_LIB_ROOT/uacs_network_transport/myabstractconnect.h \
    $$COMMON_LIB_ROOT/uacs_network_transport/serialconnect.h \
    $$COMMON_LIB_ROOT/uacs_network_transport/tcpconnect.h \
    $$COMMON_LIB_ROOT/utilities/bit_utils.h \
    $$COMMON_LIB_ROOT/utilities/config_helper.h \
    $$COMMON_LIB_ROOT/utilities/console_utilities.h \
    $$COMMON_LIB_ROOT/utilities/random_utils.h

SOURCES += \
    $$COMMON_LIB_ROOT/algorithm/binary_coded_decimals_converter.cpp \
    $$COMMON_LIB_ROOT/algorithm/crc16.cpp \
    $$COMMON_LIB_ROOT/algorithm/crc8.cpp \
    $$COMMON_LIB_ROOT/cmd/abstract_command.cpp \
    $$COMMON_LIB_ROOT/cmd/compass_lcc5000_command.cpp \
    $$COMMON_LIB_ROOT/cmd/mshpr_command.cpp \
    $$COMMON_LIB_ROOT/cmd/ubpch_command.cpp \
    $$COMMON_LIB_ROOT/cmd/um2_13_command.cpp \
    $$COMMON_LIB_ROOT/core/control/control_service.cpp \
    $$COMMON_LIB_ROOT/core/event/emergency_stop_event.cpp \
    $$COMMON_LIB_ROOT/core/event/failure_event.cpp \
    $$COMMON_LIB_ROOT/core/event/init_completed_event.cpp \
    $$COMMON_LIB_ROOT/core/event/reset_event.cpp \
    $$COMMON_LIB_ROOT/core/event/shutdown_event.cpp \
    $$COMMON_LIB_ROOT/core/event/start_auto_tracking_event.cpp \
    $$COMMON_LIB_ROOT/core/event/start_extreme_tracking_event.cpp \
    $$COMMON_LIB_ROOT/core/event/start_moving_event.cpp \
    $$COMMON_LIB_ROOT/core/event/start_moving_to_point_event.cpp \
    $$COMMON_LIB_ROOT/core/event/start_target_designation_tracking_event.cpp \
    $$COMMON_LIB_ROOT/core/event/stop_event.cpp \
    $$COMMON_LIB_ROOT/core/queue/priority_event_queue.cpp \
    $$COMMON_LIB_ROOT/core/axis.cpp \
    $$COMMON_LIB_ROOT/core/driver_controller.cpp \
    $$COMMON_LIB_ROOT/core/state_machine/state/auto_tracking_state.cpp \
    $$COMMON_LIB_ROOT/core/state_machine/state/error_state.cpp \
    $$COMMON_LIB_ROOT/core/state_machine/state/extreme_tracking_state.cpp \
    $$COMMON_LIB_ROOT/core/state_machine/state/idle_state.cpp \
    $$COMMON_LIB_ROOT/core/state_machine/state/initializing_state.cpp \
    $$COMMON_LIB_ROOT/core/state_machine/state/moving_state.cpp \
    $$COMMON_LIB_ROOT/core/state_machine/state/moving_to_point_state.cpp \
    $$COMMON_LIB_ROOT/core/state_machine/state/operational_state.cpp \
    $$COMMON_LIB_ROOT/core/state_machine/state/target_designation_tracking_state.cpp \
    $$COMMON_LIB_ROOT/core/state_machine/state_machine.cpp \
    $$COMMON_LIB_ROOT/core/state_machine_controller.cpp \
    $$COMMON_LIB_ROOT/devices/bks_device.cpp \
    $$COMMON_LIB_ROOT/devices/compass_lcc5000_device.cpp \
    $$COMMON_LIB_ROOT/devices/dvt_device.cpp \
    $$COMMON_LIB_ROOT/devices/lir919d_device.cpp \
    $$COMMON_LIB_ROOT/devices/mshpr_device.cpp \
    $$COMMON_LIB_ROOT/devices/pvt_device.cpp \
    $$COMMON_LIB_ROOT/devices/ubpch_device.cpp \
    $$COMMON_LIB_ROOT/devices/um2_13_device.cpp \
    $$COMMON_LIB_ROOT/drivers/vesper_driver.cpp \
    $$COMMON_LIB_ROOT/modbus/abstract_modbus_device.cpp \
    $$COMMON_LIB_ROOT/modbus/modbus_protocol_factory.cpp \
    $$COMMON_LIB_ROOT/modbus/modbus_rtu.cpp \
    $$COMMON_LIB_ROOT/modbus/modbus_tcp.cpp \
    $$COMMON_LIB_ROOT/modbus/read_coils.cpp \
    $$COMMON_LIB_ROOT/modbus/read_descrete_inputs.cpp \
    $$COMMON_LIB_ROOT/modbus/read_holding_registers.cpp \
    $$COMMON_LIB_ROOT/modbus/read_input_registers.cpp \
    $$COMMON_LIB_ROOT/modbus/write_multiple_coils.cpp \
    $$COMMON_LIB_ROOT/modbus/write_multiple_registers.cpp \
    $$COMMON_LIB_ROOT/modbus/write_single_coil.cpp \
    $$COMMON_LIB_ROOT/modbus/write_single_register.cpp \
    $$COMMON_LIB_ROOT/network_transport/abstract_network_transport.cpp \
    $$COMMON_LIB_ROOT/network_transport/abstract_network_transport_factory.cpp \
    $$COMMON_LIB_ROOT/network_transport/serial_transport.cpp \
    $$COMMON_LIB_ROOT/network_transport/tcp_transport.cpp \
    $$COMMON_LIB_ROOT/network_transport/udp_transport.cpp \
    $$COMMON_LIB_ROOT/parsers/compass_lcc5000_parser.cpp \
    $$COMMON_LIB_ROOT/parsers/mshpr_parser.cpp \
    $$COMMON_LIB_ROOT/parsers/ubpch_parser.cpp \
    $$COMMON_LIB_ROOT/requesters/serial_circular_requester.cpp \
    $$COMMON_LIB_ROOT/telemetry/tcp_telemetry_server.cpp \
    $$COMMON_LIB_ROOT/uacs_network_transport/myabstractconnect.cpp \
    $$COMMON_LIB_ROOT/uacs_network_transport/serialconnect.cpp \
    $$COMMON_LIB_ROOT/uacs_network_transport/tcpconnect.cpp \
    $$COMMON_LIB_ROOT/utilities/config_helper.cpp \
    $$COMMON_LIB_ROOT/utilities/console_utilities.cpp


DEFINES += COMMON_LIBRARY

DISTFILES += \
    common_lib.pri
