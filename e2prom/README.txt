Export the hex file to bin before continuing. 
Used hex2bin v2.5, Copyright (C) 2017 Jacques Pelletier & contributors. 

Br@y's Terminal is broken when using the send file feature.
Use YAT Terminal to send file via UART instead.
YAT tries to parse the \r\n sequence when sending, which leads to an error being shown and the transmission is interrupted, the file is not fully sent.
To overcome this, go to Terminal->Settings->select Terminal Type: binary. Selecte Binary Settings... and uncheck everythig.

You should now be able to send the file in full via UART. 
