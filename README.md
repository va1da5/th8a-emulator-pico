# Generic USB Shifter to Thrustmaster TH8A Emulator


### UDEV Rule

Create udev rule for Pico UART device

`cat > /etc/udev/rules.d/95-pico.rules`

```
# Raspberry Pi Pico
SUBSYSTEM=="tty", ATTRS{idVendor}=="2e8a", ATTRS{idProduct}=="000c", GROUP="dialout", MODE="0666", SYMLINK+="pico"

```

### Monitor UART

```bash
minicom -b 115200 -8 -D /dev/pico
# or 
minicom -b 115200 -8 -D /dev/ttyACM0
```

### Connection

- VDD -> RED
- VSS -> GND
- I2C-SCL -> WHITE
- I2C-SDA -> GREEN

## Connection Diagram

![connections](./images/connections.png)

## Raspberry Pi Pico Pin-outs

Original Raspberry Pi Pico

![Original Raspberry Pi Pico](./images/Raspberry-Pi-Pico-Pinout.jpg)

Pink Chinese Clone

![Pink Chinese Clone](./images/pink-pico-pinout.png)


## Flash Binaries

```bash
# build binary manually
make build

# flash binary directly using openocd
make flash

# build and flash
make apply
```


## Picoprobe Wiring

![wiring diagram](./images/picoprobe-conncetion.png)


## USB Shifter Analysis

```bash
lsusb
# ...
# Bus 001 Device 033: ID 8587:5757 FreeJoy JACK-SQ
# ...

# Find out more details about specific USB device.
lsusb -d 8587:5757 -v
```

Wireshark filter for USB Device 33 on Bus 1  `usb.src == "1.33.1" || usb.dst == "1.33.1"`. 


## Notes

Queue is being filled using interrupts. Later, pooling task reads from the queue and acts on it.

IRQ calls [`static void __tusb_irq_path_func(hcd_rp2040_irq)(void)`](/pico/pico-sdk/lib/tinyusb/src/portable/raspberrypi/rp2040/hcd_rp2040.c) function.

```c
// USB controller memory location
#define USBCTRL_REGS_BASE _u(0x50110000)

```

The USB shifter is missing a 1.5K pull-up resistor on DP (D+) line that would let MCU know its supported speed. 


## Notes

TSS Sparco handbrake I2C data [@forum](https://www.isrtv.com/forums/topic/24532-gearbox-connector-on-base/?do=findComment&comment=232234)

```
02 0C 02 00 FF 80 81 80 81 00 00 00 00 00

02 0C 02 00 8A 80 4D 80 4D 00 00 00 00 00

02 0C 02 00 82 80 4A 80 4A 00 00 00 00 00

02 0C 02 01 17 7F 20 7F 20 00 00 00 00 00

02 0C 02 01 0F 7F 1D 7F 1D 00 00 00 00 00
```

> Above TSS handbrake's I2C data mean about 0-100% brake force. Of course you need to send out every row maybe 100 times. Because of 250ms. Elsewhere you won't see anything.

```
Handbreak min -> max

0  02 02 02 02 02 // TSS handbrake
1  0C 0C 0C 0C 0C // Unknown
2  02 02 02 02 02 // ?? Device ID
3  00 00 00 01 01 // Unknown
4  FF 8A 82 17 0F // Unknown
5  80 80 80 7F 7F // Unknown ◄─┐
6  81 4D 4A 20 1D // Unknown ◄─│─┐
7  80 80 80 7F 7F // Unknown ◄─┘ │
8  81 4D 4A 20 1D // Unknown ◄───┘
9  00 00 00 00 00 // Unknown
A  00 00 00 00 00 // Unknown
B  00 00 00 00 00 // Unknown
C  00 00 00 00 00 // Unknown
D  00 00 00 00 00 // Unknown
```

## References

- [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk)
- [Raspberry Pi Pico and RP2040 - C/C++ Part 1: Blink and VS Code](https://www.digikey.lt/en/maker/projects/raspberry-pi-pico-and-rp2040-cc-part-1-blink-and-vs-code/7102fb8bca95452e9df6150f39ae8422)
- [Raspberry Pi Pico and RP2040 - C/C++ Part 2: Debugging with VS Code](https://www.digikey.be/en/maker/projects/raspberry-pi-pico-and-rp2040-cc-part-2-debugging-with-vs-code/470abc7efb07432b82c95f6f67f184c0)
- [Raspberry Pi Pico and Pico W](https://www.raspberrypi.com/documentation/microcontrollers/raspberry-pi-pico.html)
- [RP2040 Datasheet](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)
- [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk)
- [Picoprobe](https://github.com/raspberrypi/picoprobe)
- [Getting started with Raspberry Pi Pico](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)
- [Picoprobe: Using the Raspberry Pi Pico as Debug Probe](https://mcuoneclipse.com/2022/09/17/picoprobe-using-the-raspberry-pi-pico-as-debug-probe/)

- [USB Shifter to Thrustmaster Wheelbase](https://github.com/azzajess/USB-Shifter-to-Thrustmaster-Wheelbase)
- [Awesome Arduino](https://github.com/Lembed/Awesome-arduino)
- [RPi Pico I2C Address Limitation](https://raspberrypi.github.io/pico-sdk-doxygen/group__hardware__i2c.html#i2c_example)
- [GoLang Compiler Directives](https://pkg.go.dev/cmd/compile#hdr-Compiler_Directives)
- [[RP2040] First byte of X52 Pro HID report is zero, unless CFG_TUSB_DEBUG >= 2](https://github.com/hathach/tinyusb/issues/1883)
- [capturing usb on linux with wireshark](https://www.youtube.com/watch?app=desktop&v=bhZqA1By314)
- [picoprobe-cmsis-v1.0.1](https://github.com/raspberrypi/picoprobe/releases/tag/picoprobe-cmsis-v1.0.1)
- [LearnEmbeddedSystems/rp2040-project-template](https://github.com/LearnEmbeddedSystems/rp2040-project-template/tree/5f169a12eb940ed6a86003672052db57d2a82abc)
- [USB in a NutShell](https://www.beyondlogic.org/usbnutshell/usb2.shtml)
- [STM32 CLONES: THE GOOD, THE BAD AND THE UGLY](https://hackaday.com/2020/10/22/stm32-clones-the-good-the-bad-and-the-ugly/)
- [STM32F103 USB circuit](https://community.st.com/s/question/0D50X00009XkY1uSAF/stm32f103-usb-circuit)