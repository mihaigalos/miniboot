wsl make
pause
avrdude -c usbtiny -p m328p -U flash:w:miniboot.hex
pause