load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("@avr_tools//tools/avr:hex.bzl", "hex")

BOOTLOADER_START_ADDRESS = "0x7800"

EEPROM_CONFIGURATION_START_BYTE = "0x03F6"

config_setting(
    name = "avr",
    values = {
        "cpu": "avr",
    },
)

genrule(
    name = "gen_bootloader_h",
    srcs = ["src/miniboot.cpp"],
    outs = ["bootloader.h"],
    cmd = "printf \"%s\n%s %s\n%s %s\" " +
          "\"#pragma once\" " +
          "\"#define BOOTLOADER_START_ADDRESS \" \"" + BOOTLOADER_START_ADDRESS + "\" " +
          "\"#define EEPROM_CONFIGURATION_START_BYTE \" \"" + EEPROM_CONFIGURATION_START_BYTE +
          "\" >$@",
)

cc_library(
    name = "bootloader_h",
    srcs = [":gen_bootloader_h"],
)

cc_binary(
    name = "miniboot",
    srcs = glob([
        "src/*.cpp",
        "src/*.c",
        "src/*.h",
        "src/Drivers/CRC/*.h",
    ]),
    copts = select({
        ":avr": [
            "-mmcu=$(MCU)",
            "-Wl,--section-start=.text=" + BOOTLOADER_START_ADDRESS,
            "-Os",
            "-std=gnu++14",
            "-fdiagnostics-color",
        ],
        "//conditions:default": [],
    }),
    includes = [
        "src",
    ],
    linkopts = select({
        ":avr": [
            "-mmcu=$(MCU)",
            "-std=gnu++14",
        ],
        "//conditions:default": [],
    }),
    deps = [
        ":bootloader_h",
        "@system_includes//:arduino",
    ],
)

hex(
    name = "miniboot_hex",
    src = ":miniboot_sources",
)

#   listing(
#       name = "arduino_cli_mcu_listing",
#       src = ":arduino_cli_mcu",
#   )
