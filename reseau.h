#ifndef RESEAU_H
#define RESEAU_H
#include <QtWidgets>
#include "etat.h"

using namespace std;

/*!
* \class Reseau
* \brief Classe permettant de gérer la grille où se déroule la simulation.
*/
class Reseau : public QWidget {

    Q_OBJECT

    QTableWidget* gridTableWidget = nullptr;
    unsigned int width;
    unsigned int height;
    Etat*** cells;
    unsigned int heightInit;
    Etat*** cellsInit;
    unsigned int nbPasToInit;

public:

    /*!
    * \brief Constructeur de la classe Reseau.
    * \param w Largeur de la grille.
    * \param h Haueur de la grille.
    */
    Reseau(unsigned int w, unsigned int h);

    /*!
    * \brief Destructeur de la classe Reseau.
    */
    ~Reseau();

    /*!
    * \brief Retourne la largeur de la grille.
    */
    unsigned int getWidth() const { return width; }

    /*!
    * \brief Retourne la hauteur de la grille.
    */
    unsigned int getHeight() const { return height; }

    /*!
    * \brief Génère le widget représentant la grille.
    */
    void drawGrid(QBoxLayout* parent);

    /*!
    * \brief Change la largeur de la grille.
    * \param w Nouvelle largeur de la grille.
    */
    void setWidth(unsigned int w);

    /*!
    * \brief Change la hauteur de la grille.
    * \param h Nouvelle hauteur de la grille.
    */
    void setHeight(unsigned int h);

    /*!
    * \brief Retourne le widget représentant la grille.
    */
    QTableWidget* getGridTable() { return gridTableWidget; }

    /*!
    * \brief Accesseur permettant d'obtenir le pointeur sur l'état d'une cellule.
    * \param i Coordonnée en abscisse de la cellule dont on souhaite récupérer le pointeur d'état.
    * \param j Coordonnée en ordonnée de la cellule dont on souhaite récupérer le pointeur d'état.
    */
    Etat* getEtat(int i, int j) const { return cells[i][j]; }

    /*!
    * \brief Accesseur permettant d'obtenir le pointeur sur l'état d'une cellule de la configuration initiale.
    * \param i Coordonnée en abscisse de la cellule dont on souhaite récupérer le pointeur d'état.
    * \param j Coordonnée en ordonnée de la cellule dont on souhaite récupérer le pointeur d'état.
    */
    Etat* getEtatInit(int i, int j) const { return cellsInit[i][j]; }

    /*!
    * \brief Change l'état d'une cellule en fonction de ses coordonnées.
    * \param i Coordonnée en abscisse de la cellule dont on souhaite change l'état.
    * \param j Coordonnée en ordonnée de la cellule dont on souhaite change l'état.
    */
    void setCellule(int i, int j, Etat* etat) { cells[i][j] = etat; }

    /*!
    * \brief Change l'état d'une cellule de la configuration initiale en fonction de ses coordonnées.
    * \param i Coordonnée en abscisse de la cellule dont on souhaite change l'état.
    * \param j Coordonnée en ordonnée de la cellule dont on souhaite change l'état.
    */
    void setCelluleInit(int i, int j, Etat* etat) { cellsInit[i][j] = etat; }

    /*!
    * \brief Met à jour la grille en récupérant la couleur de chaque cellule du réseau.
    * \param saveSignal prend la valeur 0 lorsque l'on actualise la grille suite au "bouton précédent", 1 sinon.
    */
    void actualiserGrille(unsigned int saveSignal);

    /*!
    * \brief Génère une grille aléatoire.
    */
    void generateRandomGrid();

    /*!
    * \brief Définit de la configuration initiale pour la simulation.
    */
    void setInit();

    /*!
    * \brief Retour à la configuration initiale au cours de la simulation.
    */
    void resetToInit();

    /*!
    * \brief Retourne le nombre de pas effectués depuis la configuration initiale.
    */
    unsigned int getNbPasToInit() { return nbPasToInit; }

    /*!
    * \brief Incrémente le nombre de pas effectués depuis la configuration initiale.
    */
    void incrementNbPasToInit() { nbPasToInit += 1; }

    /*!
    * \brief Décrémente le nombre de pas effectués depuis la configuration initiale.
    */
    void decrementNbPasToInit() {
        if (nbPasToInit > 0)
            nbPasToInit -= 1;
    }

    /*!
    * \brief Réinitialise le nombre de pas effectués depuis la configuration initiale.
    */
    void resetNbPasToInit() { nbPasToInit = 0; }

    /*!
    * \brief Affiche la période de l'instance (le nombre de pas d’horloge nécessaire pour revenir à la configuration initiale).
    */
    void printInstance();

    /*!
    * \class Memento
    * \brief Classe disposant des fonctions permettant d'enregistrer des configurations à des moments donnés.
    */
    class Memento {

        unsigned int width;
        unsigned int height;
        Etat*** cells;

    public:

        /*!
        * \brief Constructeur de la classe Memento.
        * \param l Largeur de la grille.
        * \param h Haueur de la grille.
        * \param e Pointeur sur un tableau de pointeurs d'états.
        */
        Memento(unsigned int l, unsigned int h,Etat*** e);

        /*!
        * \brief Destructeur de la classe Memento.
        */
        ~Memento();

        /*!
        * \brief Constructeur par recopie de la classe Memento.
        */
        Memento(const Memento& a);

        /*!
        * \brief Opérateur d'affectation de la classe Memento.
        */
        Memento operator=(const Memento& a);

        /*!
        * \brief Accesseur permettant d'obtenir la largeur de la grille.
        */
        unsigned int getWidth() const { return width; }

        /*!
        * \brief Accesseur permettant d'obtenir la hauteur de la grille.
        */
        unsigned int getHeight() const { return height; }

        /*!
        * \brief Accesseur permettant d'obtenir le pointeur d'un tableau de pointeurs d'états.
        */
        Etat*** getEtat() { return cells; }

        /*!
        * \brief Accesseur permettant d'obtenir le pointeur sur l'état d'une cellule.
        * \param i Coordonnée en abscisse de la cellule dont on souhaite récupérer le pointeur d'état.
        * \param j Coordonnée en ordonnée de la cellule dont on souhaite récupérer le pointeur d'état.
        */
        Etat* getEtat(int i, int j) const { return cells[i][j]; }

    };

    /*!
    * \brief Construit un objet de la classe Memento et le retourne, dans le cadre d'une sauvegarde de configuration.
    */
    Memento save();

    /*!
    * \brief Restaure le réseau à partir d'une configuration qui avait été sauvegardée.
    * \param backup Objet Memento dont on veut récupérer les caractéristiques.
    */
    void restore(Memento backup);

private slots:

    /*!
    * \brief Changer manuellement l'état d'une cellule.
    */
    void cellActivation(const QModelIndex& index);

signals:

    /*!
    * \brief Signal émis dans actualiserGrille(unsigned int saveSignal) si la grille n'est pas actualisée dans le cadre d'un clic sur le bouton pour revenir à un état précédent.
    */
    void signalForSaveSimuConfiguration();

};

#endif // RESEAU_H
