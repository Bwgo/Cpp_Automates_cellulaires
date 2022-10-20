#ifndef AUTOMATE_H
#define AUTOMATE_H
#include <string>
#include <QDomDocument>
#include <unordered_map>
#include "reseau.h"
#include "exception.h"

/*!
* \class Voisinage
* \brief Classe permettant de définir l'ensemble des cellules voisines à prendre en compte dans le calcul de l'état suivant d'une cellule
*/
class Voisinage {

    unsigned int nbVoisin;

public:

    /*!
    * \brief Constructeur de la classe Voisinage sans argument.
    */
    Voisinage() = default;

    /*!
    * \brief Destructeur de la classe Voisinage. La méthode est virtuelle.
    */
    virtual ~Voisinage() = default;

    /*!
    * \brief Remplit la liste des voisins d'une cellule d'un réseau donné. La méthode est virtuelle pure car chacun des voisinages agira différemment et la classe est abstraite.
    * \param r Réseau sur lequel on travaille.
    * \param i Numéro de la ligne où se trouve la cellule dont on souhaite les voisins.
    * \param j Numéro de la colonne où se trouve la cellule dont on souhaite les voisins.
    * \param listeVoisin Tableau de voisins que la méthode doit remplir.
    */
    virtual void listeVoisin(Reseau& r, unsigned int i, unsigned int j, Etat** listeVoisin) const = 0;

    /*!
    * \brief Retourne une copie dynamique du voisinage. La méthode est virtuelle pure car chacun des voisinages agira différemment et la classe est abstraite.
    */
    virtual Voisinage* getCopyDynamic() const = 0;

    /*!
    * \brief Renvoie le nombre de voisins dont le voisinage s'occupe.
    */
    virtual unsigned int getNbVoisin() const { return nbVoisin; }

    /*!
    * \brief Demande à l'utilisateur des paramètres pour le voisinage et qui modifie ces paramètres suite aux réponses de l'utilisateur. Retourne true si cela a modifié quelque chose, false sinon.
    */
    virtual bool setParameters() { return false; }

    /*!
    * \brief Retourne les paramètres du voisinage.
    */
    virtual unordered_map<string, int> getParameters() const = 0;

    /*!
    * \brief Récupère des paramètres pour le voisinage via un noeud XML (<voisinage...>xxx</voisinage>) et qui modifie ces paramètres suite aux informations dans celui ci. Retourne true si cela a modifié quelque chose, false sinon.
    */
    virtual bool setParametersXML(QDomNode&) { return false; }

};

/*!
* \class VoisinageMoore
* \brief Classe héritée de Voisinage permettant de gérer le voisinage de Moore.
*/

class VoisinageMoore : public Voisinage {

    unsigned int nbVoisin = 8;

public:

    /*!
    * \brief Destructeur de la classe VoisinageMoore.
    */
    ~VoisinageMoore() = default;

    /*!
    * \brief Remplit la liste des voisins d'une cellule d'un réseau donné.
    * \param r Réseau sur lequel on travaille.
    * \param i Numéro de la ligne où se trouve la cellule dont on souhaite les voisins.
    * \param j Numéro de la colonne où se trouve la cellule dont on souhaite les voisins.
    * \param listeVoisin Tableau de voisins que la méthode doit remplir.
    */
    void listeVoisin(Reseau& r, unsigned int i, unsigned int j, Etat** listeVoisin) const override;

    /*!
    * \brief Renvoie le nombre de voisins dont le voisinage s'occupe.
    */
    unsigned int getNbVoisin() const override { return nbVoisin; }

    /*!
    * \brief Retourne une copie dynamique du voisinage.
    */
    VoisinageMoore* getCopyDynamic() const override { return new VoisinageMoore(*this); }

    /*!
    * \brief Retourne les paramètres du voisinage.
    */
    unordered_map<string, int> getParameters() const override {
        unordered_map<string, int> parameters;
        return parameters;
    }

};

/*!
* \class VoisinageVonNeuman
* \brief Classe héritée de Voisinage permettant de gérer le voisinage de VonNeuman.
*/
class VoisinageVonNeuman : public Voisinage {

    unsigned int nbVoisin = 4;

public:

    /*!
    * \brief Destructeur de la classe VoisinageVonNeuman.
    */
    ~VoisinageVonNeuman() = default;

    /*!
    * \brief Remplit la liste des voisins d'une cellule d'un réseau donné.
    * \param r Réseau sur lequel on travaille.
    * \param i Numéro de la ligne où se trouve la cellule dont on souhaite les voisins.
    * \param j Numéro de la colonne où se trouve la cellule dont on souhaite les voisins.
    * \param listeVoisin Tableau de voisins que la méthode doit remplir.
    */
    void listeVoisin(Reseau& r, unsigned int i, unsigned int j, Etat** listeVoisin) const override;

    /*!
    * \brief Renvoie le nombre de voisins dont le voisinage s'occupe.
    */
    unsigned int getNbVoisin() const override { return nbVoisin; }

    /*!
    * \brief Retourne une copie dynamique du voisinage.
    */
    VoisinageVonNeuman* getCopyDynamic() const override { return new VoisinageVonNeuman(*this); }

    /*!
    * \brief Retourne les paramètres du voisinage.
    */
    unordered_map<string, int> getParameters() const override {
        unordered_map<string, int> parameters;
        return parameters;
    }

};


/*!
* \class VoisinageMooreGeneral
* \brief Classe héritée de Voisinage permettant de gérer le voisinage de Moore généralisé.
*/
class VoisinageMooreGeneral : public Voisinage {

    unsigned int radius = 2;

public:

    /*!
    * \brief Destructeur de la classe VoisinageMooreGeneral.
    */
    ~VoisinageMooreGeneral() = default;

    /*!
    * \brief Remplit la liste des voisins d'une cellule d'un réseau donné.
    * \param r Réseau sur lequel on travaille.
    * \param i Numéro de la ligne où se trouve la cellule dont on souhaite les voisins.
    * \param j Numéro de la colonne où se trouve la cellule dont on souhaite les voisins.
    * \param listeVoisin Tableau de voisins que la méthode doit remplir.
    */
    void listeVoisin(Reseau& r, unsigned int i, unsigned int j, Etat** listeVoisin) const override;

    /*!
    * \brief Renvoie le nombre de voisins dont le voisinage s'occupe.
    */
    unsigned int getNbVoisin() const override { return pow(2*radius+1,2)-1; };

    /*!
    * \brief Définit le rayon.
    * \param r Rayon
    */
    void setRadius(unsigned int r) { radius = r; }

    /*!
    * \brief Retourne une copie dynamique du voisinage.
    */
    VoisinageMooreGeneral* getCopyDynamic() const override { return new VoisinageMooreGeneral(*this); }

    /*!
    * \brief Demande à l'utilisateur des paramètres pour le voisinage et qui modifie ces paramètres suite aux réponses de l'utilisateur. Retourne true si cela a modifié quelque chose, false sinon.
    */
    bool setParameters() override;

    /*!
    * \brief Récupère des paramètres pour le voisinage via un noeud XML (<voisinage...>xxx</voisinage>) et qui modifie ces paramètres suite aux informations dans celui ci. Retourne true si cela a modifié quelque chose, false sinon.
    */
    bool setParametersXML(QDomNode&) override;

    /*!
    * \brief Retourne les paramètres du voisinage dans un unorderd_map<string,int>.
    */
    unordered_map<string, int> getParameters() const override {
        unordered_map<string, int> parameters;
        parameters.emplace("rayon",radius);
        return parameters;
    }

};

/*!
* \class VoisinageVonNeumanGeneral
* \brief Classe héritée de Voisinage permettant de gérer le voisinage de VonNeuman généralisé.
*/
class VoisinageVonNeumanGeneral : public Voisinage {

    unsigned int radius = 2;

public:

    /*!
    * \brief Destructeur de la classe VoisinageVonNeumanGeneral.
    */
    ~VoisinageVonNeumanGeneral() = default;

    /*!
    * \brief Remplit la liste des voisins d'une cellule d'un réseau donné.
    * \param r Réseau sur lequel on travaille.
    * \param i Numéro de la ligne où se trouve la cellule dont on souhaite les voisins.
    * \param j Numéro de la colonne où se trouve la cellule dont on souhaite les voisins.
    * \param listeVoisin Tableau de voisins que la méthode doit remplir.
    */
    void listeVoisin(Reseau& r, unsigned int i, unsigned int j, Etat** listeVoisin) const override;

    /*!
    * \brief Renvoie le nombre de voisins dont le voisinage s'occupe.
    */
    unsigned int getNbVoisin() const override { return 2*radius*(radius+1); }

    /*!
    * \brief Définit le rayon.
    * \param ray Rayon
    */
    void setRadius(unsigned int ray) { radius = ray; }

    /*!
    * \brief Retourne une copie dynamique du voisinage.
    */
    VoisinageVonNeumanGeneral* getCopyDynamic() const override { return new VoisinageVonNeumanGeneral(*this); }

    /*!
    * \brief Demande à l'utilisateur des paramètres pour le voisinage et qui modifie ces paramètres suite aux réponses de l'utilisateur. Retourne true si cela a modifié quelque chose, false sinon.
    */
    bool setParameters() override;

    /*!
    * \brief Récupère des paramètres pour le voisinage via un noeud XML (<voisinage...>xxx</voisinage>) et qui modifie ces paramètres suite aux informations dans celui ci. Retourne true si cela a modifié quelque chose, false sinon.
    */
    bool setParametersXML(QDomNode&) override;

    /*!
    * \brief Retourne les paramètres du voisinage dans un unorderd_map<string,int>.
    */
    unordered_map<string, int> getParameters() const override {
        unordered_map<string, int> parameters;
        parameters.emplace("rayon",radius);
        return parameters;
    }

};


/*!
* \class VoisinageArbitraire
* \brief Classe héritée de Voisinage permettant de gérer un voisinage arbitraire.
*/
class VoisinageArbitraire : public Voisinage{

    vector<int> arrayVoisin = vector<int>(8,-1);

public:

    /*!
    * \brief Destructeur de la classe VoisinageArbitraire.
    */
    ~VoisinageArbitraire() override {}

    /*!
    * \brief Renvoie le nombre de voisins dont le voisinage s'occupe.
    */
    unsigned int getNbVoisin() const override {
        unsigned int nb = 0;
        for(unsigned int i=0; i<8; i++)
            if(arrayVoisin[i] == 1) nb++;
        return nb;
    }

    /*!
    * \brief Remplit la liste des voisins d'une cellule d'un réseau donné.
    * \param r Réseau sur lequel on travaille.
    * \param i Numéro de la ligne où se trouve la cellule dont on souhaite les voisins.
    * \param j Numéro de la colonne où se trouve la cellule dont on souhaite les voisins.
    * \param listeVoisin Tableau de voisins que la méthode doit remplir.
    */
    void listeVoisin(Reseau& r, unsigned int i, unsigned int j, Etat** listeVoisin) const override;

    /*!
    * \brief Retourne une copie dynamique du voisinage.
    */
    VoisinageArbitraire* getCopyDynamic() const override { return new VoisinageArbitraire(*this); }

    /*!
    * \brief Demande à l'utilisateur des paramètres pour le voisinage et qui modifie ces paramètres suite aux réponses de l'utilisateur. Retourne true si cela a modifié quelque chose, false sinon.
    */
    bool setParameters() override;

    /*!
    * \brief Récupère des paramètres pour le voisinage via un noeud XML (<voisinage...>xxx</voisinage>) et qui modifie ces paramètres suite aux informations dans celui ci. Retourne true si cela a modifié quelque chose, false sinon.
    */
    bool setParametersXML(QDomNode&) override;

    /*!
    * \brief Retourne les paramètres du voisinage dans un unorderd_map<string,int>.
    */
    unordered_map<string, int> getParameters() const override {
        unordered_map<string, int> parameters;
        parameters.emplace("hautGauche",arrayVoisin[0]);
        parameters.emplace("haut",arrayVoisin[1]);
        parameters.emplace("hautDroite",arrayVoisin[2]);
        parameters.emplace("gauche",arrayVoisin[3]);
        parameters.emplace("droite",arrayVoisin[4]);
        parameters.emplace("basGauche",arrayVoisin[5]);
        parameters.emplace("bas",arrayVoisin[6]);
        parameters.emplace("basDroite",arrayVoisin[7]);
        return parameters;
    }

};

/*!
* \class Automate
* \brief Objet qui appliquera des transitions sur un réseau.
*/
class Automate {

protected:

    const Voisinage* voisinage = nullptr;
    unsigned int nbEtat;

public:

    /*!
    * \brief Destructeur de la classe Automate.
    */
    virtual ~Automate() = default;

    /*!
    * \brief Applique une transition sur un réseau pour passer à la configuration suivante (au pas de temps suivant). La méthode est virtuelle car chacun des voisinages agira différemment.
    * \param r Réseau où on souhaite appliquer la transition.
    */
    virtual void appliquerTransition(Reseau& r) const = 0;

    /*!
    * \brief Permet de définir (ou modifier) le voisinage utilisé par l'automate.
    * \param v Pointeur sur le voisinage à utiliser.
    */
    void setVoisinage(const Voisinage* v) { voisinage = v; }

    /*!
    * \brief Permet d'obtenir un pointeur sur le voisinage utilisé par l'automate.
    */
    const Voisinage* getVoisinage() const { return voisinage; }

    /*!
    * \brief Renvoie le nombre d'états dans la fonction de transition utilisée.
    */
    virtual unsigned int getNbEtat() const { return nbEtat; }

    /*!
    * \brief Demande à l'utilisateur des paramètres pour le voisinage et qui modifie ces paramètres suite aux réponses de l'utilisateur. Retourne true si cela a modifié quelque chose, false sinon.
    */
    virtual bool setParameters() { return false; }

    /*!
    * \brief Retourne les paramètres de l'automate.
    */
    virtual unordered_map<string, int> getParameters() const = 0;

    /*!
    * \brief Récupère des paramètres pour le voisinage via un noeud XML (<voisinage...>xxx</voisinage>) et qui modifie ces paramètres suite aux informations dans celui ci. Retourne true si cela a modifié quelque chose, false sinon.
    */
    virtual bool setParametersXML(QDomNode&)  {return false; }

};

/*!
* \class AutomateGriffeath
* \brief Classe héritée de Automate permettant de gérer l'automate de Griffeath
*/
class AutomateGriffeath : public Automate {

    unsigned int nbEtat = 4;

public:

    /*!
    * \brief Destructeur de l'automate, détruit aussi le voisinage pointé.
    */
    ~AutomateGriffeath() { delete voisinage; }

    /*!
    * \brief Permet de définir (ou modfier) le voisinage utilisé par l'automate.
    * \param v Pointeur sur le voisinage à utiliser.
    */
    AutomateGriffeath(const Voisinage* a = nullptr) { voisinage = a; }

    /*!
    * \brief Applique une transition sur un réseau pour passer à la configuration suivante (au pas de temps suivant).
    * \param r Réseau où on souhaite appliquer la transition.
    */
    void appliquerTransition(Reseau& r) const override;

    /*!
    * \brief Renvoie le nombre d'états dans la fonction de transition utilisée.
    */
    unsigned int getNbEtat() const override { return nbEtat; }

    /*!
    * \brief Retourne les paramètres de l'automate.
    */
    unordered_map<string, int> getParameters() const override {
        unordered_map<string, int> parameters;
        return parameters;
    }

};

/*!
* \class AutomateGameLife
* \brief Classe héritée de Automate permettant de gérer l'automate Life Game
*/
class AutomateGameLife : public Automate {

    unsigned int nbEtat = 2;

public:

    /*!
    * \brief Destructeur de l'automate, détruit aussi le voisinage pointé.
    */
    ~AutomateGameLife() { delete voisinage; }

    /*!
    * \brief Permet de définir (ou modifier) le voisinage utilisé par l'automate.
    * \param v Pointeur sur le voisinage à utiliser.
    */
    AutomateGameLife(const Voisinage* a = nullptr) { voisinage = a; }

    /*!
    * \brief Applique une transition sur un réseau pour passer à la configuration suivante (au pas de temps suivant).
    * \param r Réseau où on souhaite appliquer la transition.
    */
    void appliquerTransition(Reseau& r) const override;

    /*!
    * \brief Renvoie le nombre d'états dans la fonction de transition utilisée.
    */
    unsigned int getNbEtat() const override { return nbEtat; }

    /*!
    * \brief Retourne les paramètres de l'automate.
    */
    unordered_map<string, int> getParameters() const override {
        unordered_map<string, int> parameters;
        return parameters;
    }

};

/*!
* \class AutomateBriansBrain
* \brief Classe héritée de Automate permettant de gérer l'automate Brian's Brain
*/
class AutomateBriansBrain : public Automate {

    unsigned int nbEtat = 3;

public:

    /*!
    * \brief Destructeur de l'automate, détruit aussi le voisinage pointé.
    */
    ~AutomateBriansBrain(){ delete voisinage; }

    /*!
    * \brief Permet de définir (ou modifier) le voisinage utilisé par l'automate.
    * \param v Pointeur sur le voisinage à utiliser.
    */
    AutomateBriansBrain(const Voisinage* a = nullptr) { voisinage = a; }

    /*!
    * \brief Applique une transition sur un réseau pour passer à la configuration suivante (au pas de temps suivant).
    * \param r Réseau où on souhaite appliquer la transition.
    */
    void appliquerTransition(Reseau& r) const override;

    /*!
    * \brief Renvoie le nombre d'états dans la fonction de transition utilisée.
    */
    unsigned int getNbEtat() const override { return nbEtat; }

    /*!
    * \brief Retourne les paramètres de l'automate.
    */
    unordered_map<string, int> getParameters() const override {
        unordered_map<string, int> parameters;
        return parameters;
    }
};

/*!
* \class AutomateLangtonLoop
* \brief Classe héritée de Automate permettant de gérer l'automate Langston's Loop
*/
class AutomateLangtonLoop : public Automate {

    unsigned int nbEtat = 8;

public:

    /*!
    * \brief Destructeur de l'automate, détruit aussi le voisinage pointé.
    */
    ~AutomateLangtonLoop() { delete voisinage; }

    /*!
    * \brief Permet de définir (ou modifier) le voisinage utilisé par l'automate.
    * \param v Pointeur sur le voisinage à utiliser.
    */
    AutomateLangtonLoop(const Voisinage* a = nullptr) { voisinage = a; }

    /*!
    * \brief Applique une transition sur un réseau pour passer à la configuration suivante (au pas de temps suivant).
    * \param r Reseau où on souhaite appliquer la transition.
    */
    void appliquerTransition(Reseau& r) const override;

    /*!
    * \brief Renvoie le nombre d'états dans la fonction de transition utilisée.
    */
    unsigned int getNbEtat() const override { return nbEtat; }

    /*!
    * \brief Retourne les paramètres de l'automate.
    */
    unordered_map<string, int> getParameters() const override {
        unordered_map<string, int> parameters;
        return parameters;
    }

};

/*!
* \class AutomateWireWorld
* \brief Classe héritée de Automate permettant de gérer l'automate Wire World
*/
class AutomateWireWorld : public Automate {

    unsigned int nbEtat = 4;

public:

    /*!
    * \brief Destructeur de l'automate, détruit aussi le voisinage pointé.
    */
    ~AutomateWireWorld() { delete voisinage; }

    /*!
    * \brief Permet de définir (ou modifier) le voisinage utilisé par l'automate.
    * \param v Pointeur sur le voisinage à utiliser.
    */
    AutomateWireWorld(const Voisinage* a = nullptr) { voisinage = a; }

    /*!
    * \brief Applique une transition sur un réseau pour passer à la configuration suivante (au pas de temps suivant).
    * \param r Réseau où on souhaite appliquer la transition.
    */
    void appliquerTransition(Reseau& r) const override;

    /*!
    * \brief Renvoie le nombre d'états dans la fonction de transition utilisée.
    */
    unsigned int getNbEtat() const override { return nbEtat; }

    /*!
    * \brief Retourne les paramètres de l'automate.
    */
    unordered_map<string, int> getParameters() const override {
        unordered_map<string, int> parameters;
        return parameters;
    }

};

#endif // AUTOMATE_H
