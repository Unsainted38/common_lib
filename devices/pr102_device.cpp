#include "pr102_device.h"
#include "utilities/config_helper.h"
#include "read_holding_registers.h"
#include "write_multiple_registers.h"

Pr102Device::Pr102Device(std::shared_ptr<SerialCircularRequester> requester, QString config_path, QString section, QObject *parent)
    : ModbusDevice{std::move(requester), config_path, section, parent}
{
    QSettings settings(config_path, QSettings::IniFormat);
    settings.beginGroup(section);
    auto registers = ConfigHelper::loadConfig<Registers>(
        settings,
        field("GetRegisters", &Registers::get_registers_),
        field("GetCount", &Registers::get_count_),
        field("SetRegisters", &Registers::set_registers_),
        field("SetCount", &Registers::set_count_));
    settings.endGroup();
    getRegisters_ = std::make_unique<ReadHoldingRegisters>(registers.get_registers_, registers.get_count_, protocol_);
    setRegisters_ = std::make_unique<WriteMultipleRegisters>(registers.set_registers_, registers.set_count_, protocol_);

    addCircularCommand(getRegisters_.get());
    addCircularCommand(setRegisters_.get());
}

QVector<quint16> Pr102Device::getRegisters()
{
    return getRegisters_->getValue().value<QVector<quint16>>();
}

void Pr102Device::setRegisters(QVector<quint16> registers)
{
    setRegisters_->setValue(QVariant::fromValue(registers));
}
