#include "mainwindow.h"
#include <QApplication>

/*! \mainpage Cellulut Documentation
 *
 * \section LO21
 *
 * TD5 - GROUPE 2 <br>
 * L'application <b>Cellulut</b> est destinée à simuler des automates cellulaires. <br>
 * Elle a été conçue et développée dans le cadre des projets de l'UV LO21 par :<br>
 * Aurélie Law-Yen, Damien Willett, Théo Brochard, Thomas Bossuat et Yanan Fu.<br>
 * Vous trouverez ici la documentation de ce projet.
 *
 */

int main(int argc, char* argv[]) {

    // Création de l'application
    QApplication app(argc, argv);
    app.setApplicationName("Cellulut");

    // Récupération de l'automate factory et du voisinage factory
    AutomateFactory& af = AutomateFactory::getFactory();
    VoisinageFactory& vf = VoisinageFactory::getFactory();

    /* ICI AJOUTER LES CLASSES */

    af.addClass<AutomateGameLife>("AutomateGameLife");
    af.addClass<AutomateGriffeath>("AutomateGriffeath");
    af.addClass<AutomateBriansBrain>("AutomateBriansBrain");
    af.addClass<AutomateLangtonLoop>("AutomateLangtonLoop");
    af.addClass<AutomateWireWorld>("AutomateWireWorld");

    /* ICI AJOUTER LES VOISINAGES */

    vf.addClass<VoisinageMoore>("VoisinageMoore");
    vf.addClass<VoisinageVonNeuman>("VoisinageVonNeuman");
    vf.addClass<VoisinageMooreGeneral>("VoisinageMooreGeneral");
    vf.addClass<VoisinageVonNeumanGeneral>("VoisinageVonNeumanGeneral");
    vf.addClass<VoisinageArbitraire>("VoisinageArbitraire");

    // Création de la fenetre principale et affichage de celle-ci
    MainWindow w;
    w.show();
    return app.exec();

}
