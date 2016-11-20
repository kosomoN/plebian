#include "components.h"

#include "transform.h"

const size_t ComponentIDToFamily[1] = {
    Transform::Family()
};

uint8_t Transform::NetworkID() { return 0; }
