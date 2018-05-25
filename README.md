# CAS-Embedded-System-2018 Object-Tracking
Im Modul Embedded Systems soll eine Projektarbeit durchgeführt werden, um das gelernte Wissen direkt anzuwenden.

## Descrption:
Es soll mit einem Raspberry Pi und der Pi Kamera ein Tischtennisball erkannt werden. Dieser wird mittels zwei Servos(Pan,Tilt) getrackt. Befindet sich der Ball in einem definierten Bildausschnitt, soll dieser mit einem Luftstoss oder ähnlichem abgelenkt werden.

## Setup:

### Raspberry Pi
Anleitung zum Setup des PI's: https://www.raspberrypi.org/documentation/setup/

### Raspberry Kamera
Neuste Firmware für den Raspberry laden:
```console
sudo apt-get update
sudo apt-get upgrade
```
Kamera aktivieren: 
```console
sudo raspi-config
```
Die Kamera Option muss über die Pfeiltasten und Cursor auf "Enable" gesetzt werden.

### Eclipse and OpenCV
Anleitung zur Installation von Eclipse und OpenCV: https://docs.opencv.org/3.1.0/d7/d16/tutorial_linux_eclipse.html
