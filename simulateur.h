#ifndef SIMULATEUR_H
#define SIMULATEUR_H
#include "factory.h"

/*!
* \class Simulateur
* \brief Simulateur réalisant la gestion des transitions de l'automate.
*/
class Simulateur : public QObject {

    Q_OBJECT

    Reseau* grille = nullptr;
    Automate* automate = nullptr;
    unsigned int nbPas = 0;
    QTimer* timer = nullptr;

    /*!
    * \brief Suppression de l'opérateur d'affectation afin d'éviter des duplications.
    */
    Simulateur& operator=(const Simulateur& s) = delete;

    /*!
    * \brief Suppression du constructeur par recopie afin d'éviter des duplications.
    */
    Simulateur(const Simulateur&) = delete;

public:

    /*!
    * \brief Constructeur de la classe Simulateur.
    * \param a Pointeur constant sur un automate (ou classe héritée).
    */
    Simulateur(Automate* a, QObject * parent = nullptr);

    /*!
    * \brief Constructeur de la classe Simulateur sans argument.
    */
    Simulateur() = default;

    /*!
    * \brief Destructeur de la classe Simulateur. Désaloue l'automate du simulateur ainsi que le timer.
    */
    ~Simulateur() {
        delete automate;
        delete timer;
    }

    /*!
    * \brief Accesseur permettant d'assigner (ou modifier) la grille du simulateur.
    * \param r Pointeur constant sur un réseau.
    */
    void setGrille(Reseau* r) { grille = r; }

    /*!
    * \brief Accesseur permettant d'assigner (ou modifier) l'automate du simulateur.
    * \param a Pointeur constant sur un automate.
    */
    void setAutomate(Automate* a) { automate = a; }

    /*!
    * \brief Accesseur permettant d'obtenir le pointeur sur l'automate du simulateur.
    */
    Automate* getAutomate() const { return automate; }

    /*!
    * \brief Accesseur permettant d'obtenir le pointeur sur le réseau.
    */
    const Reseau* getReseau() const { return grille; }

    /*!
    * \brief Accesseur permettant d'obtenir le pointeur sur le timer.
    */
    const QTimer* getTimer() const { return timer; }

    /*!
    * \brief Effectue des transitions sur le réseau pointé par le simulateur grâce à l'automate pointé par le simulateur. Cela de manière automatique après un pas de temps confirgurable.
    */
    void simulation();

    /*!
    * \brief Arrête la simulation en arrêtant le timer.
    */
    void arret();

    /*!
    * \brief Revient en arrière d'une transition si cela est possible.
    */
    void setSpeed(int speed);

public slots:

    /*!
    * \brief Effectue une unique transition sur le réseau pointé par le simulateur grâce à l'automate pointé par le simulateur.
    */
    void suivant();

};

#endif // SIMULATEUR_H
