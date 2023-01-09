#ifndef _HBC56_CF_DEVICE_H_
#define _HBC56_CF_DEVICE_H_

#include "device.h"
#include "file.h"

#ifdef __cplusplus
extern "C" {
#endif

HBC56Device createCompactFlashDevice(uint16_t baseAddr, File *imageFile);

#define CF_DATA  0x00 // Data (R/W)
#define CF_ERR   0x01 // Error register (R)
#define CF_FEAT  0x01 // Features (W)
#define CF_SECCO 0x02 // Sector count (R/W)
#define CF_LBA0  0x03 // LBA bits 0-7 (R/W, LBA mode)
#define CF_LBA1  0x04 // LBA bits 8-15 (R/W, LBA mode)
#define CF_LBA2  0x05 // LBA bits 16-23 (R/W, LBA mode)
#define CF_LBA3  0x06 // LBA bits 24-27 (R/W, LBA mode)
#define CF_STAT  0x07 // Status (R)
#define CF_CMD   0x07 // Command (W)

#define CF_Command_ReadSectors 0x20
#ifdef __cplusplus
}
#endif


#endif