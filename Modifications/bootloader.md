
https://www.engbedded.com/fusecalc/

```
avrdude -c jtag2isp -p atmega2560 -i 125000 -e -U lock:w:0x3f:m -U lfuse:w:0xcf:m -U hfuse:w:0xd0:m -U efuse:w:0xff:m
avrdude -c jtag2isp -p atmega2560 -i 125000 -e -U lock:w:0x3f:m -U efuse:w:0xff:m -U hfuse:w:0xd0:m -U lfuse:w:0xcf:m

ric.bootloader.unlock_bits=0x3F
ric.bootloader.lock_bits=0x0F

```

```
avrdude -c jtag2isp -p atmega2560 -i 125000 -e -U lock:w:0x3f:m -U lfuse:w:0xcf:m -U hfuse:w:0xd0:m -U efuse:w:0xff:m -U boot:w:./bootloader/ric3d__atmega2560.hex -U lock:w:0x0f:m
```
