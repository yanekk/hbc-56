#include "devices/device.h"

HBC56Device createDualLcdDevice() {
    HBC56Device device = createDevice("Dual Graphics LCD");
    
    return device;
}