load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@avr_tools//tools/avr:hex.bzl", "hex")

config_setting(
    name = "avr",
    values = {
        "cpu": "avr",
    },
)

BOOTLOADER_START_ADDRESS = "0x7800"

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
