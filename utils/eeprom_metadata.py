#!/usr/bin/python

# By Michael Bartlett, https://github.com/bartlettmic
"""
USAGE: Provide a compiled .bin file (can be generated
with something like `avr-objcopy -I ihex $HEX_FILE -O binary
$BIN_FILE`) and this will prepend the necessary miniboot-header
bytes to a resulting .eeprom file which can be streamed directly
into EEPROM to operate with miniboot. This will also nicely
print out the generated data header data in human readable
format.
"""

from time import time
from binascii import crc32, hexlify, unhexlify
from sys import argv
from os import stat

if len(argv) < 2:
    print("No binary file provided, dying...")
    exit(1)
    
binary_file = argv[1]

(mode, ino, dev, nlink, uid, gid, size, atime, mtime, ctime) = stat(binary_file)
    
# Header protocol:
#  0-9   = miniboot header (ABminiboot)
#  10-19 = sketch name (provided in arguments)
#  20-23 = timestamp of sketch creation (creation time epoch)
#  24-27 = timestamp of eeprom write (current timestamp epoch)
#  28-31 = CRC32 (Checksum hex value)
#  32-33 = Length (size of binary in Hex)

with open(binary_file, 'rb') as binary:
    payload = binary.read()
    
    payload_size = len(payload)
    payload_size_hex = hexlify(payload_size.to_bytes(2,'big'))
    
    crc = crc32(payload)
    crc_hex = hexlify(crc.to_bytes(4, 'big'))
    
    header_string = "ABminiboot"
    header_hex = hexlify(bytes(header_string, 'utf-8'))
    
    app_name = binary_file.ljust(10, ' ')[:10]
    # app_name = "AUTOGEN".ljust(10, ' ')
    app_hex = hexlify(bytes(app_name, 'utf-8'))

    ctime_hex = hexlify(ctime.to_bytes(4,'big'))
    
    current_epoch = int(time())
    epoch_hex = hexlify(current_epoch.to_bytes(4,'big'))

    eeprom_metadata = header_hex + app_hex + ctime_hex + epoch_hex + crc_hex + payload_size_hex
    # print("EEPROM header:", eeprom_metadata)
    
    eeprom_metadata = unhexlify(eeprom_metadata)
    
    eeprom_payload = eeprom_metadata + payload
    
    # for i in range(len(eeprom_metadata)):
    #     print(i,":",hex(eeprom_metadata[i]))
        
    print("Miniboot header:   ", header_string)
    print("Application name:  ", app_name)
    print("Created timestamp: ", ctime)
    print("Current timestamp: ", current_epoch)
    print("CRC32:             ", crc)
    print("Original binary size:", payload_size)
    print("EEPROM payload size: ", len(eeprom_payload))
    
    output_file = binary_file.replace('.bin','.eeprom')
    with open(output_file, 'w+b') as firmware:
        firmware.write(eeprom_payload)
        print(f"\n-> \"{output_file}\"")
    