#include "osapi.h"

namespace osapi {

unsigned int getSystemTime() {
    return xTaskGetTickCount();
}

} // namespace osapi

