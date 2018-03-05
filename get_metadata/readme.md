

GET METADATA
by Ranjit Bhinge


A nodeJS tool to automatically generate metadata and create an Arduino sketch to help you write the EEPROM contents while using miniboot


To use this tool:
1. Install NodeJS and NPM from https://nodejs.org/en/download/
2. Check to ensure that NodeJS and NPM are installed. Follow http://blog.teamtreehouse.com/install-node-js-npm-windows
3. navigate to the miniboot/get_metadata/ directory from your terminal
4. Run > npm install
5. This installs dependencies
6. To run the tool, type the command > node get_metadata
7. When it asks for the .bin file path, provide the file path
8. When it asks for the sketch name, provide a name under 10 characters
9. It then processes the chosen file (if it exists) and spews out the metadata
10. It also generates an Arduino sketch that you can run with the I2C EEPROM connected to Arduino like this: 
VCC-5v, GND-GND, SCL-A5 & SDA-A4
11. The generated sketch allows micocontrollers to write the new Sketch to the EEPROM and then read from that EEPROM and flash themselves using Miniboot