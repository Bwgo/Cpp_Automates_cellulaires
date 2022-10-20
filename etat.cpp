#include "etat.h"

// Initialisation à null de l'instance static de EtatManager
EtatManager* EtatManager::instance = nullptr;

// Destruction de l'EtatManager, ainsi on supprime tous les états et le tableau d'états
EtatManager::~EtatManager() {

    for(unsigned int i =0; i<nbEtats; i++)
        delete etats[i];
    delete[] etats;

}

// Retourne l'instance de EtatManager
EtatManager& EtatManager::getManager() {

    if(instance == nullptr) instance = new EtatManager;
    return *instance;

}

// Libération de l'instance de EtatManager (appel au destructeur)
void EtatManager::libereManager() {

    delete instance;
    instance = nullptr;

}

// Ajout d'un nouvel état au tableau en vérifiant qu'il n'y en ai déjà pas trop
void EtatManager::newEtat(const std::string str, QColor color) {

    if(nbEtats == 0){
        delete[] etats;
        etats = new Etat*[nbMaxEtats];
    }
    etats[nbEtats] = new Etat(nbEtats,str,color);
    nbEtats++;

}

void EtatManager::setNbMaxEtats(unsigned int nb) {

    if(nb > nbEtats) {
        Etat** temp = new Etat*[nb];
        for(unsigned int i =0; i<nbEtats; i++)
            temp[i] = etats[i];
        Etat ** old = etats;
        etats = temp;
        nbMaxEtats = nb;
        delete[] old;
    }

}
