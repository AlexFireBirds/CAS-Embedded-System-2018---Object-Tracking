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
Danach wird ein Reboot gefordert.

Prüfen ob die Kamera funktioniert:
```console
raspistill -o cam.jpg
```
Bild wird unter /home/pi gespeichert

### Eclipse and OpenCV
Eclipse mit folgenden Befehlen installieren:
```console
sudo apt-get install eclipse
sudo apt-get install eclipse-cdt g++
```
Anleitung zur Konfiguration von Eclipse und OpenCV: https://docs.opencv.org/3.1.0/d7/d16/tutorial_linux_eclipse.html

Damit die Kamera in OpenCV verwendet werden kann, muss das LKM (LoadableKernelModule) noch dem Linux Kernel hinzugefügt werden.
Um es bei jedem Neustart automatisch zu starten, muss folgende Zeile dem modules.conf File hinzugefügt werden:
/etc/modules-load.d/modules.conf
```console
bcm2835-v4l2
```

Alternativ kann es via Terminal manuell hinzugefügt werden (muss beim nächsten Reboot wiederholt werden):
```console
sudo modprobe bcm2835-v4l2
```
### PWM Output setup
Unter **/boot/config.txt** die Zeile **dtoverlay=pwm-2chan** einfügen. Danach speichern und neustarten.
Nun muss der Pfad **/sys/class/pwm/pwmchip0** existieren. pwm0 ist auf dem GPIO18 und pwm1 auf dem GPIO19.
Rechte setzen, damit auf die PWM HW zugegriffen werden kann. Unter **/etc/udev/rules.d/99-com.rules** folgende Zeilen hinzufügen:
```console
SUBSYSTEM=="pwm*", PROGRAM="/bin/sh -c '\
        chown -R root:gpio /sys/class/pwm && chmod -R 770 /sys/class/pwm;\
        chown -R root:gpio /sys/devices/platform/soc/*.pwm/pwm/pwmchip* && chmod -R 770 /sys/devices/platform/soc/*.pwm/pwm/pwmchip*\
'"
``` 
Danach in Eclipse unter ***Properties->C/C++ Build->Settings->Build Steps->Post-build steps* die folgende Zeile einfügen:
```console
sudo chown root <Projektname>; sudo chmod +s <Projektname>;
```

### Git GUI Client
```console
sudo apt-get install git-cola
```