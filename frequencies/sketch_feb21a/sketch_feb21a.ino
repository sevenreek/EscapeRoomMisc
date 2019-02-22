// TEST DE LECTURE DU NO DE SERIE D'UN DS2401
// https://pdfserv.maximintegrated.com/en/ds/DS2401.pdf

// J'AI UN FORMAT TO92 --> Pin 1 = GND, PIN 2 = DATA, PIN3 = Non Connectée

// On Connecte la pin 7 de l'arduino à PIN 2
// On connecte aussi PIN 2 à 5V via une Résistance de 4.7kΩ
// On connecte pin1 vers GND

const byte oneWirePin = 7;

#include <OneWire.h>
OneWire DS2401(oneWirePin);

void printHex(byte b, const char separateur = '\n')
{
  if (b <= 0xF) Serial.print(0);
  Serial.print(b, HEX);
  if (separateur) Serial.write(separateur);
}


void setup()
{
  Serial.begin (9600);
  Serial.println(F("TEST DU DALLAS DS2401"));

  if (DS2401.reset()) {
    byte data[8];   // les 64 bits (8 octets) du DS2401: 8 bits de famille, 48bits de N° de série et 8 bits de CRC

    // extrait du manuel:
    // This command allows the bus master to read the DS2401's 8-bit family code,
    // unique 48-bit serial number, and 8-bit CRC. This command can only be used
    // if there is a single DS2401 on the bus. If more than one slave is present on
    // the bus, a data collision will occur when all slaves try to transmit at the
    // same time (open drain will produce a wired-AND result). The DS2401 Read ROM
    // function will occur with a command byte of either 33h or 0Fh in order to ensure
    // compatibility with the DS2400, which will only respond to a 0Fh command word with
    // its 64-bit ROM data.

    DS2401.write(0x33);  //on envoie la commande de lecture "Read ROM [33h]"

    // on lit la réponse qui consiste en 8 octets

    data[0] = DS2401.read(); // le premier est le N° de famille
    Serial.print(F("Code famille: 0x"));
    printHex(data[0]);

    Serial.print(F("No de serie: 0x"));
    for (byte i = 1; i <= 6; i++) // les 6 suivants sont le N° de série
    {
      data[i] = DS2401.read(); // on lit les 6 octets
      printHex(data[i], (i < 6) ? '\0' : '\n');
    }

    data[7] = DS2401.read(); //on lit le dernier octect qui est le CRC

    // on vérifie que le CRC de la puce est bien le m$eme que celui calculé
    if (data[7] == OneWire::crc8(data, 7)) {
      Serial.print(F("CRC du DS2401 correct = 0x"));
      printHex (data[7]);
    } else {
      Serial.println(F("Erreur de CRC"));
    }
  }
}

void loop() {}
