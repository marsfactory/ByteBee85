# ByteBee85 | Circuito Grafico Workshop / Circuit Workshop Circuit
![bytebee85](https://raw.githubusercontent.com/marsfactory/ByteBee85/main/diycad/bytebee85.png)

### Fabrica Marciana & Mark Dusjagr, Buenos Aires, 2022. 
Minimalist low-fi synthesizer on the ATtiny85 running acid bytebeats
``The 8bit mixtape Neo fork`` 

# DIY CAD

### Esquema completo con programador / Complete schematic with programmer
![schematic](https://raw.githubusercontent.com/marsfactory/ByteBee85/main/diycad/esquema.png)

### Esquema simple sin programador / Simple schematic without programmer
![schematic-without-programmer](https://raw.githubusercontent.com/marsfactory/ByteBee85/main/diycad/esquema-sin-programmer.png)

### Componentes / Components
![components](https://raw.githubusercontent.com/marsfactory/ByteBee85/main/diycad/componenetes.png)

### Diseño de PCB / PCB Design
![pcb-design-process](https://raw.githubusercontent.com/marsfactory/ByteBee85/main/diycad/pcb-process.png)

### Diseño para imprimir / Layout to print
![pcb-design-process](https://raw.githubusercontent.com/marsfactory/ByteBee85/main/diycad/template-components.png)


### Si no quieres usar el método DIYCAD para realizar tu PCB, puedes utilizar esta placa para producción casera. / If you don't want to use the DIYCAD method to make your PCB, you can use this board for homemade production.

#### PCB
![pcb](https://raw.githubusercontent.com/marsfactory/ByteBee85/main/hardware/production/pcb.png)

#### LAYOUT
![layout](https://raw.githubusercontent.com/marsfactory/ByteBee85/main/hardware/production/layout.png)

#### SCHEMATIC
![layout](https://raw.githubusercontent.com/marsfactory/ByteBee85/main/hardware/images/schematic.jpg)

## Configuración / Configuration

### Librerías / Libraries

- Agregar las librerías en caso de ser necesario. (TODO...) / Add the libraries if necessary. (TODO...)

### Programación a través de audio / Programming via audio

#### Si el attiny no tiene su bootloader es necesario flashearlo para utilizar el programador a través de audio: / If the attiny doesn't have its bootloader, it needs to be flashed to use the audio programmer:

- Este proyecto utiliza la board del 8bit mixtape neo / This project uses the 8bit mixtape neo board
Se puede agregar al IDE Arduino a través del gestor de tarjetas, agregando la línea: / It can be added to the Arduino IDE through the board manager, by adding the line:
`http://8bitmixtape.github.io/package_8bitmixtape_index.json`

- En el 2.0 hay que agregar las boards manualmente. / In 2.0 the boards must be added manually.

- En Windows:
``TODO...``

- En Linux:
Poner Arduino como Programmer as ISP, subir sketch de ejemplo, realizar conexiones, tener seleccionada la placa Arduino Uno, ejecutar la terminal / Set Arduino as Programmer as ISP, upload example sketch, make connections, select the Arduino Uno board, run the terminal
``sudo apt-install avrdude``  

``avrdude -P /dev/ttyACM0 -b 19200 -c avrisp -p t85 -U efuse:w:0xfe:m -U hfuse:w:0xdd:m -U lfuse:w:0xe1:m``

``avrdude -v -p attiny85 -c avrisp -P /dev/ttyACM0 -b 19200 -U flash:w:/home/arsat/.arduino15/packages/8BitMixtape/hardware/avr/0.0.28/bootloaders/tinyaudioboot/AudioBootAttiny85_AudioPB3_LEDPB1.hex:i`` 

NOTA: para subir el código por audio, esperar a que el LED parpadee (2 segundos aprox) y darle upload con un volumen al 70 u 80 %. / NOTE: to upload the code via audio, wait for the LED to blink (approximately 2 seconds) and hit upload with a volume of 70 to 80%.

### Programación estándar para el attiny85 / Standard programming for the attiny85

TODO...