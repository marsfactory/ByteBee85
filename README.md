# ByteBee85
Minimalist low-fi synthesizer on the ATtiny85 running acid bytebeats
``The 8bit mixtape Neo fork``


- Este proyecto utiliza la board del 8bit mixtape neo 
Se puede agregar al IDE Arduino a traves del gestor de tarjetas, agregando la linea: 
`http://8bitmixtape.github.io/package_8bitmixtape_index.json`

- Si el attiny no tiene su bootloader es necesario flasharlo, 
En windows: 
``TODO...``

- En linux:
Poner Arduino como Programmer as ISP , subir skectch de ejemplo, realizar conexiones, tener seleccionado la placa Arduino Uno, ejecutar la terminal 
``sudo apt-install avrdude`` (o algo por el estilo) 
``avrdude -P /dev/ttyACM0 -b 19200 -c avrisp -p t85 -U efuse:w:0xfe:m -U hfuse:w:0xdd:m -U lfuse:w:0xe1:m``
``avrdude -v -p attiny85 -c avrisp -P /dev/ttyACM0 -b 19200 -U flash:w:/home/arsat/.arduino15/packages/8BitMixtape/hardware/avr/0.0.28/bootloaders/tinyaudioboot/AudioBootAttiny85_AudioPB3_LEDPB1.hex:i``

- Agregar las librerias en caso de ser necesario. 
Este proyecto es una adaptacion para funcionar con los comonenetes minimos posibles, un pote y un boton, para realizar byebeat en un attiny85. 
