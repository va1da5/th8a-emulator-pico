#ifdef __cplusplus
extern "C"
{
#endif

#define CFG_TUSB_MCU OPT_MCU_RP2040
#define CFG_TUSB_RHPORT0_MODE (OPT_MODE_HOST | OPT_MODE_DEFAULT_SPEED)
#define CFG_TUSB_MEM_ALIGN TU_ATTR_ALIGNED(4)
#define CFG_TUSB_DEBUG 0 // 1,2

#if CFG_TUSB_MCU == OPT_MCU_RP2040
// change to 1 if using pico-pio-usb as host controller for raspberry rp2040
#define CFG_TUH_RPI_PIO_USB 0
#define BOARD_TUH_RHPORT CFG_TUH_RPI_PIO_USB
#endif

// RHPort number used for host can be defined by board.mk, default to port 0
#ifndef BOARD_TUH_RHPORT
#define BOARD_TUH_RHPORT 0
#endif

// RHPort max operational speed can defined by board.mk
#ifndef BOARD_TUH_MAX_SPEED
#define BOARD_TUH_MAX_SPEED OPT_MODE_DEFAULT_SPEED
#endif

// Enable Host stack
#define CFG_TUH_ENABLED 1

// Default is max speed that hardware controller could support with on-chip PHY
#define CFG_TUH_MAX_SPEED BOARD_TUH_MAX_SPEED

#ifndef CFG_TUSB_MEM_SECTION
#define CFG_TUSB_MEM_SECTION
#endif

#ifndef CFG_TUSB_MEM_ALIGN
#define CFG_TUSB_MEM_ALIGN __attribute__((aligned(4)))
#endif

#define CFG_TUH_ENUMERATION_BUFSIZE 256
#define CFG_TUH_HUB 1

// max device support (excluding hub device)
// 1 hub typically has 4 ports
#define CFG_TUH_DEVICE_MAX (CFG_TUH_HUB ? 4 : 1)

#define CFG_TUH_HID 1
#define CFG_TUH_HID_EPIN_BUFSIZE 64
#define CFG_TUH_HID_EPOUT_BUFSIZE 64

#ifdef __cplusplus
}
#endif