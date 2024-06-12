# ByteBee85 | Circuito Grafico Workshop / Circuit Workshop Circuit
![bytebee85](https://raw.githubusercontent.com/marsfactory/ByteBee85/main/diycad/bytebee85.png)

### Fabrica Marciana & Marc Dusjagr, Buenos Aires, 2024. 
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

### Programación a través de audio / Programming via audio

- Este proyecto utiliza la board del 8bit mixtape neo. Se puede agregar al IDE Arduino a través del gestor de tarjetas, agregando la línea en sus preferencias de hardware adicional: 
`http://8bitmixtape.github.io/package_8bitmixtape_index.json`
 
 Instrucciones aqui: 
 https://github.com/8BitMixtape/8Bit-Mixtape-NEO/wiki/4_3-IDE-integration

- Para subir el código por audio, esperar a que el LED parpadee (2 segundos aprox) y darle UPLOAD desde el IDE con un volumen al 70 u 80 %.

- `NOTA`: En el IDE 2.0 hay que agregar las boards manualmente.


#### Si el attiny no tiene su bootloader es necesario flashearlo para utilizar el programador a través de audio: / If the attiny doesn't have its bootloader, it needs to be flashed to use the audio programmer:

- En Windows:
From Arduino IDE
Arduino ISP Programmer 
Tools > Burn Bootloader
Select 8BitMixtapeNeo Board

- En Linux:
Poner Arduino como Programmer as ISP, subir sketch de ejemplo, realizar conexiones, tener seleccionada la placa Arduino Uno, ejecutar la terminal / Set Arduino as Programmer as ISP, upload example sketch, make connections, select the Arduino Uno board, run the terminal
``sudo apt-install avrdude``  

``avrdude -P /dev/ttyACM0 -b 19200 -c avrisp -p t85 -U efuse:w:0xfe:m -U hfuse:w:0xdd:m -U lfuse:w:0xe1:m``

``avrdude -v -p attiny85 -c avrisp -P /dev/ttyACM0 -b 19200 -U flash:w:/home/arsat/.arduino15/packages/8BitMixtape/hardware/avr/0.0.28/bootloaders/tinyaudioboot/AudioBootAttiny85_AudioPB3_LEDPB1.hex:i`` 

### Programación estándar para el attiny85 / Standard programming for the attiny85

- Pueden checar en este tutorial para quemar el Attiny85 con un bootloader clasico y subir el codigo utilizando un Arduino. 
https://laboratoriodejuguete.com/category/attiny85/

