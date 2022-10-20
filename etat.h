#ifndef ETAT_H
#define ETAT_H
#include <string>
#include <QColor>

/*!
* \class Etat
* \brief Objet Etat repésentant l'état des cellules d'un automate, défini par un indice, un label et une couleur
*/
class Etat {

    unsigned int indice;
    std::string label;
    QColor couleur;

public :

    /*!
    * \brief Constructeur de la classe Etat.
    * \param i Indice de l'état.
    * \param lab Label de l'état.
    * \param c Couleur de l'état.
    */
    Etat(unsigned int i, std::string lab, QColor c) : indice(i), label(lab), couleur(c) {}

    /*!
    * \brief Retourne l'indice de l'état.
    */
    unsigned int getIndice() const { return indice; }

    /*!
    * \brief Retourne le label de l'état.
    */
    std::string getLabel() const { return label; }

    /*!
    * \brief Retourne la couleur de l'état.
    */
    QColor getCouleur() const { return couleur; }

    /*!
    * \brief Modifie la couleur de l'état.
    * \param color Couleur à assigner à l'état.
    */
    void setCouleur(QColor color) { couleur = color; }

    /*!
    * \brief Modifie le label de l'état.
    * \param str Label à assigner à l'état.
    */
    void setLabel(std::string str) { label = str; }

};

/*!
* \class EtatManager
* \brief Classe gérant l'ensemble des états disponibles, possédant ainsi un tableau de tous les états actuellement disponibles (maximum 8). Classe singleton.
*/
class EtatManager {

    Etat** etats;
    unsigned int nbEtats;
    unsigned int nbMaxEtats;
    static EtatManager* instance;
    EtatManager() : etats(nullptr), nbEtats(0), nbMaxEtats(8) {}
    ~EtatManager();
    EtatManager(const EtatManager& a) = delete;
    EtatManager& operator=(const EtatManager& a) = delete;

public:

    /*!
    * \brief Retourne l'instance de la classe EtatManager.
    */
    static EtatManager& getManager();

    /*!
    * \brief Libère l'instance de l'EtatManager.
    */
    static void libereManager();

    /*!
    * \brief Construit un nouvel Etat.
    * \param str Label de l'état.
    * \param color Couleur de l'état.
    */
    void newEtat(const std::string str, QColor color);

    /*!
    * \brief Retourne un pointeur sur l'état se trouvant à l'indice i du tableau d'états de l'EtatManager.
    * \param i Indice de l'état que l'on souhaite récupérer.
    */
    Etat* getEtat(unsigned int i) { return etats[i]; }

    /*!
    * \brief Retourne le nombre d'états stockés actuellement dans le tableau d'états de l'EtatManager.
    */
    unsigned int getNbEtats() const { return nbEtats; }

    /*!
    * \brief Permet de choisir le nombre maximal d'états possibles.
    * \param nb Nombre maximal d'états possibles.
    */
    void setNbMaxEtats(unsigned int nb);

};

#endif // ETAT_H
