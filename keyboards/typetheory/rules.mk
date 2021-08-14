MCU = atmega32u4
BOOTLOADER = caterina
F_CPU = 8000000

SPLIT_KEYBOARD   = yes
LTO_ENABLE       = yes

BOOTMAGIC_ENABLE = yes
MOUSEKEY_ENABLE  = yes
EXTRAKEY_ENABLE  = yes
CONSOLE_ENABLE   = yes
COMMAND_ENABLE   = no
NKRO_ENABLE      = no    # not supported with bluetooth
UNICODE_ENABLE   = no
BLUETOOTH         = AdafruitBLE
