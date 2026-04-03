#include "crc8.h"


quint8 BytesSumCrc(const QByteArray bytes) {
    quint8 checksum = 0;

    for(auto byte : bytes) {
        checksum += static_cast<quint8>(byte);
    }

    return checksum;
}
