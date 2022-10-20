#include <QTimer>
#include "simulateur.h"
#include "mainwindow.h"

// Création du simulateur, on crée un timer qui est de base à 1 seconde
Simulateur::Simulateur(Automate* a, QObject* parent) : QObject(parent), automate(a) {

    timer = new QTimer;
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(suivant()));

}

// On lance la simulation en lançant le timer
void Simulateur::simulation() {

    timer->start();

}

// On applique une transition
void Simulateur::suivant() {

    automate->appliquerTransition(*grille);
    grille->actualiserGrille(1);
    grille->incrementNbPasToInit();

    // Comparaison pour savoir si on est revenu avec la configuration actuelle à la configuration initiale
    bool configInit = true;

    for (unsigned int i=0; i<grille->getHeight(); i++)
        for (unsigned int j=0; j<grille->getWidth(); j++)
            if (grille->getEtat(i,j) != grille->getEtatInit(i,j))
                configInit = false;

    if (configInit == true) {
        grille->printInstance();
        arret();
    }

}

// On arrête la simulation en arrêtant le timer
void Simulateur::arret() {

    timer->stop();

}

// On met à jour la vitesse de la simulation donc le temps du timer
void Simulateur::setSpeed(int speed) {

    int temps = 1000/speed; // vitesse = nb transi par seconde donc en ms : 1000/vitesse
    timer->setInterval(temps);

}
