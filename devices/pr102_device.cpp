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
    getRegistersCmd_ = std::make_unique<ReadHoldingRegisters>(registers.get_registers_, registers.get_count_, protocol_);
    getRegistersCmd_->set_callback([&]() {
        update_measurement();
    });
    setRegistersCmd_ = std::make_unique<WriteMultipleRegisters>(registers.set_registers_, registers.set_count_, protocol_);
    QVector<quint16> regs({0});
    setRegistersCmd_->setValue(QVariant::fromValue(regs));

    addCircularCommand(getRegistersCmd_.get());
    addCircularCommand(setRegistersCmd_.get());

    timer_ = std::make_unique<QTimer>();
    timer_->start(3000);
    connect(timer_.get(), &QTimer::timeout, this, &Pr102Device::invalidate_measurement);
}

QVector<quint16> Pr102Device::getRegisters()
{
    return getRegistersCmd_->getValue().value<QVector<quint16>>();
}

void Pr102Device::setRegisters(QVector<quint16> registers)
{
    setRegistersCmd_->setValue(QVariant::fromValue(registers));
}

bool Pr102Device::is_online() const
{
    return measurement_.value.online;
}

Measurement<Pr102State> Pr102Device::measurement() const
{
    return measurement_;
}

void Pr102Device::update_measurement()
{
    timer_->start(3000);
    measurement_.value.online = true;
    measurement_.valid = true;
    measurement_.timestamp = std::chrono::steady_clock::now();
    measurement_.value.registers_data = getRegisters();

}

void Pr102Device::invalidate_measurement()
{
    measurement_.value.online = false;
    measurement_.valid = false;
    measurement_.value.registers_data = getRegisters();
}

