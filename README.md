# ByteBee85
Minimalist low-fi synthesizer on the ATtiny85 running acid bytebeats
``The 8bit mixtape Neo fork``

Este proyecto es una adaptacion para funcionar con los comonenetes minimos posibles, un pote y un boton, para realizar byebeat en un attiny85. 


# DIY CAD



### Esquema completa con programador 
[!schematic](diycad/esquema.png)

### Esquema simple sin programador
[!schematic-without-programmer](diycad/esquema-sin-programmer.png)

### Componentes
[!componenets](diycad/componenetes.png)

### Pcb Design
[!pcb-design-process](diycad/pcb-process.png)

### Layout to print
[!pcb-design-process](diycad/template-components.png)


## Configuracion

### Librerias

- Agregar las librerias en caso de ser necesario. (TODO...)

### Programacion a traves de audio

#### Si el attiny no tiene su bootloader es necesario flasharlo para utilizar el programador a traves de audio:

- Este proyecto utiliza la board del 8bit mixtape neo 
Se puede agregar al IDE Arduino a traves del gestor de tarjetas, agregando la linea: 
`http://8bitmixtape.github.io/package_8bitmixtape_index.json`

- En el 2.0 hay que agregar las boards manualmente. 

- En windows: 
``TODO...``

- En linux:
Poner Arduino como Programmer as ISP , subir skectch de ejemplo, realizar conexiones, tener seleccionado la placa Arduino Uno, ejecutar la terminal 
``sudo apt-install avrdude``  


``avrdude -P /dev/ttyACM0 -b 19200 -c avrisp -p t85 -U efuse:w:0xfe:m -U hfuse:w:0xdd:m -U lfuse:w:0xe1:m``

``avrdude -v -p attiny85 -c avrisp -P /dev/ttyACM0 -b 19200 -U flash:w:/home/arsat/.arduino15/packages/8BitMixtape/hardware/avr/0.0.28/bootloaders/tinyaudioboot/AudioBootAttiny85_AudioPB3_LEDPB1.hex:i`` 


NOTA: para subir el codigo por audio, esperar a que el led parpadea (2segundos aprox) y darle upload con un volumen al 70 u 80 %. 

### Programacion estandar para el attiny85