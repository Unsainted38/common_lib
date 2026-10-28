#include "lir919d_device.h"
#include <utilities/bit_utils.h>
#include "modbus/read_input_registers.h"

void Lir919dDevice::onTimer()
{
    QVector<quint16> regs = descrete_cmd_->getValue().value<QVector<quint16>>();
    if (regs.size() < 3) {
        measurement_.value.online = false;
        return;
    }
    measurement_.value.online = true;
    measurement_.value.encoder_descrete = BitUtils::makeQuint32(regs[1], regs[2]);
    measurement_.timestamp = std::chrono::steady_clock::now();
    measurement_.valid = true;
}

Lir919dDevice::Lir919dDevice(std::shared_ptr<SerialCircularRequester> requester, QString configPath, QString section, QObject *parent)
    : ModbusDevice{std::move(requester), configPath, section, parent}
{
    QSettings settings(configPath, QSettings::IniFormat);

    settings.beginGroup(section);
    measurement_.value.turn_count_bits = settings.value("TurnCountBits", 12).value<quint8>();
    measurement_.value.bits_per_turn = settings.value("BitsPerTurn", 18).value<quint8>();
    settings.endGroup();

    descrete_cmd_ = new ReadInputRegisters(0x10, 3, protocol_);
    addCircularCommand(descrete_cmd_);

    m_timer_ = new QTimer(this);
    m_timer_->start(100);
    connect(m_timer_, SIGNAL(timeout()), this, SLOT(onTimer()));
}

Measurement<Lir919dState> Lir919dDevice::measurement() const
{
    return measurement_;
}

quint32 Lir919dDevice::descrete() const
{
    return measurement_.value.encoder_descrete;
}

bool Lir919dDevice::is_online() const
{
    return measurement_.value.online;
}

unsigned Lir919dDevice::turn_count_bits() const
{
    return measurement_.value.turn_count_bits;
}

unsigned Lir919dDevice::bits_per_turn() const
{
    return measurement_.value.bits_per_turn;
}
