#include "osapi.h"

namespace osapi {

unsigned int getSystemTime() {
    return osKernelGetTickCount();
}

} // namespace osapi

