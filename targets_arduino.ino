#include <Arduino.h>
#include <MicroOscSlip.h>
#include <Chrono.h>
#include <Bounce2.h>
Bounce2::Button bouton = Bounce2::Button();

MicroOscSlip<128> monOsc(&Serial);

Chrono chronoPot;

void traiterMessageOsc(MicroOscMessage &message)
{
    // SI L’ADRESSE DU MESSAGE EST "/lumiere" ET QUE LE MESSAGE CONTIENT UN ARGUMENT ENTIER ("i") :
    if (message.checkOscAddressAndTypeTags("/ok", "i"))
    {

        int valeur = message.nextAsInt(); // RÉCUPÉRER L’ENTIER ("i")

        digitalWrite(6, valeur); // ASSIGNER LA VALEUR A LA DEL
    }
}

void setup()
{
    pinMode(6, OUTPUT);
    Serial.begin(115200);
    bouton.attach(9, INPUT_PULLUP);
    bouton.setPressedState(LOW);
}

void loop()
{
    monOsc.onOscMessageReceived(traiterMessageOsc); // ACQUISITION DES MESSAGES

    if (chronoPot.hasPassed(22))
    {                        // SI LE CHRONO DÉPASSE 20 MILLISECONDES
        chronoPot.restart(); // REPARTIR LE CHRONO

        // LE 3 CI-BAS CORRESPOND À LA BROCHE ANALOGIQUE A3
        int valeur = analogRead(1); // LECTURE DE LA TENSION DE LA BROCHE A3 ENTRE 0 ET 1023

        monOsc.sendInt("/rot", valeur); // ENVOYER LA VALEUR
    }

    bouton.update();
    if (bouton.pressed())
    { // Bouton vient d’être appuyé
        monOsc.sendInt("/tir", 1);
    }
}