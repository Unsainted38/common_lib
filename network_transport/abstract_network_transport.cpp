#include "abstract_network_transport.h"

AbstractNetworkTransport::AbstractNetworkTransport(QString configPath, QString section, QObject *parent)
    : QObject{parent},
      m_configPath(configPath),
      m_section(section) {
}

