#ifndef CARETAKER_H
#define CARETAKER_H
#include <string>
#include <iostream>
#include <QCoreApplication>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDomDocument>
#include <QDomElement>
#include <QDomText>
#include <QInputDialog>
#include <QStringList>
#include "reseau.h"

using namespace std;

/*!
* \class CareTaker
* \brief Classe permettant de sauvegarder et charger des configurations dans le cadre de la fonctionnalité de retour à des configurations précédentes ou à la demande de l'utilisateur..
*/
class CareTaker : public QWidget {

private:

    unsigned int nbBackups;
    unsigned int emptyPrevious = 0;
    unsigned int nbprevious = 0;
    Reseau::Memento ** tab;
    // newClickPrevious = 1 avant le premier retour précédent d'une série de retours
    // newClickPrevious = 0 quand la simulation n'a pas avancé d'un pas depuis le dernier retour en arrière
    unsigned int newClickPrevious = 1;
    CareTaker(const CareTaker& C) = delete;
    ostream& operator=(const CareTaker& C) = delete;

public:

    /*!
    * \brief Constructeur de la classe CareTaker.
    * \param nbsBackups Nombre maximum de configurations qui pourront être mémorisées et chargées de nouveau avec la fonctionnalité d'accès aux dernières configurations générées.
    */
    CareTaker(unsigned int nbsBackups) : nbBackups(nbsBackups), tab(new Reseau::Memento*[nbsBackups+1]) {};

    /*!
    * \brief Destructeur de la classe CareTaker.
    */
    ~CareTaker(){
        for(unsigned int i = 0; i < nbprevious; i++)
            delete tab[i];
        delete[] tab;
    }

    /*!
    * \brief Accesseur du nombre maximum de configurations qui pourront être mémorisées et chargées de nouveau avec la fonctionnalité d'accès aux dernières configurations générées.
    */
    unsigned int getNbBackups() const { return nbBackups; }

    /*!
    * \brief Mise à jour du nombre maximum de configurations qui pourront être mémorisées et chargées de nouveau avec la fonctionnalité d'accès aux dernières configurations générées  & modification du tableau .
    * \param n Nouveau nbBackups
    */
    void setNbBackups(unsigned int n);

    // Sauvegarder une configuration manuellement

    /*!
    * \brief Sauvegarder manuellement une configuration.
    * \param backup Référence sur la configuration que l'on souhaite enregistrer.
    * \param title Titre de la configuration.
    * \param modele Nom du modèle associé à la configuration.
    */
    void addMemento(const Reseau::Memento& backup, string title, string modele);

    /*!
    * \brief Enregistre au format XML les informations relatives à une sauvegarde manuelle d'une configuration.
    * \param backup Référence sur la configuration que l'on souhaite enregistrer.
    * \param title Titre de la configuration.
    * \param modele Nom du modèle associé à la configuration.
    */
    void saveXMLFile(const Reseau::Memento& memento, string title, string modele);

    /*!
    * \brief Ajoute à l'arborescence du fichier XML les informations relatives à une sauvegarde manuelle d'une configuration.
    * \param xml Référence sur l'objet représentant le document XML sous forme d'arborescence.
    * \param title Titre de la configuration.
    * \param configuration Référence sur l'élément que l'on souhaite ajouter à l'arborescence du fichier XML.
    * \param memento Référence sur l'objet représentant la configuration à enregistrer pour avoir accès aux indices des états.
    */
    void saveXMLConfiguration(QDomDocument& xml, const string title, QDomElement& configuration, const Reseau::Memento& memento);

    // Sauvergarder la configuration actuelle à chaque pas de simulation

    /*!
    * \brief Accesseur de tab : tableau des nbBackups configurations précédentes.
    */
    Reseau::Memento** gettabPrevious() const { return tab; }

    /*!
    * \brief Accesseur de nbprevious.
    */
    unsigned int getnbprevious() const { return nbprevious; }

    /*!
    * \brief Retourne un tableau d'entiers avec les indices des états.
    */
    Reseau::Memento* depilerprevious();

    /*!
    * \brief Supprime juste le dernier memento enregistré.
    */
    void removelastone();

    /*!
    * \brief Sauvegarde une configuration à chaque pas de simulation.
    * \param backup Référence sur la configuration que l'on souhaite enregistrer.
    */
    void addMementoPrevious(Reseau::Memento *backup);

    /*!
    * \brief Vide le fichier enregistrant les dernières configurations de la simulation.
    */
    void emptyPreviousConfigurations();

    /*!
    * \brief Mise à jour de la variable permettant de savoir si le fichier contenant l'historique est vide.
    * \param n Vaut 1 avant le premier retour précédent d'une série de retours en arrière et vaut 0 quand la simulation n'a pas avancé d'un pas depuis le dernier retour en arrière.
    */
    void setNewClickPrevious(unsigned int n) { newClickPrevious = n; }

    /*!
    * \brief Accesseur permettant de savoir si le dernier clic sur le bouton configuration précédente a été fait juste après avoir déjà cliqué dessus.
    */
    unsigned int getNewClickPrevious() const { return newClickPrevious; }

};

#endif // CARETAKER_H
