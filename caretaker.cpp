#include "caretaker.h"
#include <iostream>
#include <QInputDialog>

void CareTaker::addMemento(const Reseau::Memento& backup, string title, string modele) {

    CareTaker::saveXMLFile(backup, title, modele);

}

void CareTaker::addMementoPrevious(Reseau::Memento *backup) {

    // On vient supprimer l'état le plus ancien
    if (nbprevious > nbBackups){
        delete tab[0];
        nbprevious--;
        // On déplace tout d'une case vers la gauche
        for(unsigned int i = 0; i < (nbprevious); i++)
            tab[i] =tab[i+1];
    }
    tab[nbprevious++] = backup;

}

// Une configuration est associée à 1 modèle particulier, donc enregistrer les infos liées au modèle (voisinage, règles de transition, ensemble d'états)
// Si le modèle courant est nouveau -> enregistrer au préalable ce nouveau modèle
void CareTaker::saveXMLFile(const Reseau::Memento& memento, string title, string modele) {

    /* Il faut que le fichier "configurations.xml" soit composé au minimum de :
    <?xml version='1.0' encoding='UTF-8'?>
    <root>
    </root>
    */

    QDomDocument xml;
    xml.appendChild(xml.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"") );
    QDomElement tagRoot = xml.createElement("root");

    QDomElement configuration = xml.createElement("Configuration");
    configuration.setAttribute("modele", QString::fromStdString(modele));

    saveXMLConfiguration(xml, title, configuration, memento);

    tagRoot.appendChild(configuration);
    xml.appendChild(tagRoot);

    QString fileName = QFileDialog::getSaveFileName(this, tr("Sauvegarder configuration"),
                               "../projet_lo21/configurations/"+QString::fromStdString(title)+".xml",
                               tr("Nom de la configuration(*.xml)"));

    if(fileName.isEmpty()) return;

    QFile file(fileName);

    if( !file.open( QIODevice::WriteOnly | QIODevice::Text))
    qDebug( "Erreur pour ouvrir le fichier en écriture." );
    QTextStream stream(&file);
    stream << xml.toString();

    file.close();

}

void CareTaker::saveXMLConfiguration(QDomDocument& xml, const string title, QDomElement& configuration, const Reseau::Memento& memento) {

    QDomElement tagTitle = xml.createElement("title");
    QDomText textTitle = xml.createTextNode(QString::fromStdString(title));
    tagTitle.appendChild(textTitle);
    configuration.appendChild(tagTitle);

    QDomElement tagWidth = xml.createElement("width");
    QDomText textWidth = xml.createTextNode(QString::number(memento.getWidth()));
    tagWidth.appendChild(textWidth);
    configuration.appendChild(tagWidth);

    QDomElement tagHeight = xml.createElement("height");
    QDomText textHeight = xml.createTextNode(QString::number(memento.getHeight()));
    tagHeight.appendChild(textHeight);
    configuration.appendChild(tagHeight);

    // Sauvegarde les indices des états de chaque case
    for (unsigned int i = 0; i<memento.getHeight(); i++){
        QDomElement tagLine = xml.createElement("line");
        for (unsigned int j = 0; j<memento.getWidth(); j++){
            QDomElement tagColumn = xml.createElement("column");
            QDomText textColumn = xml.createTextNode(QString::number(memento.getEtat(i, j)->getIndice()));
            tagColumn.appendChild(textColumn);
            tagLine.appendChild(tagColumn);
        }
        configuration.appendChild(tagLine);
    }

    xml.appendChild(configuration);

}

void CareTaker::emptyPreviousConfigurations() {

    for(unsigned int i = 0; i < (nbprevious); i++)
        delete tab[i];
    nbprevious=0;

}

Reseau::Memento* CareTaker::depilerprevious() {

    // On récupère la dernière configuration enregistrée et on diminue le nombre de configurations précédentes
    Reseau::Memento * tab_to_return = tab[nbprevious-1];
    nbprevious--;
    return tab_to_return;

}

void CareTaker::removelastone() {

    delete tab[nbprevious-1];
    nbprevious--;

}

void CareTaker::setNbBackups(unsigned int n) {

    Reseau::Memento ** tmp = new Reseau::Memento*[n+1];

    if (n<(nbprevious)) {
        // On vient recopier les anciens états dans le nouveau tab
        for(unsigned int i = 0; i <=n; i++)
            if (tab[i])
                tmp[i]=tab[nbprevious-1-n+i];
        delete[] tab;
        nbprevious=std::min(n+1,nbprevious);
    } else {
        for(unsigned int i = 0; i <=n; i++)
            if (tab[i])
                tmp[i]=tab[i];
        delete[] tab;
    }
    nbBackups=n;
    tab = tmp;

}
