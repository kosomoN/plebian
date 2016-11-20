#ifndef NETWORK_DEFINES_H_
#define NETWORK_DEFINES_H_

#include <MessageIdentifiers.h>

#define TICK_LENGTH_MS (1000.0f / 20.0f)

enum PacketId {
    ID_START_TIME = ID_USER_PACKET_ENUM
};

#endif // NETWORK_DEFINES_H_
