#include "automate.h"
#include "rules.h"

void VoisinageMoore::listeVoisin(Reseau& r, unsigned int i, unsigned int j,Etat** listeVoisin) const {

    unsigned int colonne = r.getWidth();
    unsigned int ligne = r.getHeight();
    listeVoisin[0] = r.getEtat(i,(j-1+colonne)%colonne); // gauche
    listeVoisin[1] = r.getEtat(i,(j+1)%colonne); // droite
    listeVoisin[2] = r.getEtat((i-1+ligne)%ligne,j); // haut
    listeVoisin[3] = r.getEtat((i+1)%ligne,j); // bas
    listeVoisin[4] = r.getEtat((i-1+ligne)%ligne,(j-1+colonne)%colonne); // diagonal haut gauche
    listeVoisin[5] = r.getEtat((i-1+ligne)%ligne,(j+1)%colonne); // diagonle haut droite
    listeVoisin[6] = r.getEtat((i+1)%ligne,(j+1)%colonne); // diagonale bas droite
    listeVoisin[7] = r.getEtat((i+1)%ligne,(j-1+colonne)%colonne); // diagonale bas gauche

}

void VoisinageVonNeuman::listeVoisin(Reseau& r, unsigned int i, unsigned int j,Etat** listeVoisin) const {

    unsigned int colonne = r.getWidth();
    unsigned int ligne = r.getHeight();
    listeVoisin[0] = r.getEtat(i,(j-1+colonne)%colonne); // gauche
    listeVoisin[1] = r.getEtat(i,(j+1)%colonne); // droite
    listeVoisin[2] = r.getEtat((i-1+ligne)%ligne,j); // haut
    listeVoisin[3] = r.getEtat((i+1)%ligne,j); // bas

}

void VoisinageMooreGeneral::listeVoisin(Reseau& r, unsigned int i, unsigned int j, Etat **listeVoisin) const {

    unsigned int colonne = r.getWidth();
    unsigned int ligne = r.getHeight();

    unsigned int v = 0;
    for (int k = -static_cast<int>(radius); k <= static_cast<int>(radius);k++) {
        for (int l = -static_cast<int>(radius); l <= static_cast<int>(radius); l++) {
            if (!(k==0 && l ==0)) { // pour ne pas ajouter au voisinage la case étudiée elle-même
                if (k<0) { // à gauche strictement
                    if (l<0) // en bas strictement
                        listeVoisin[v] = r.getEtat((i+k+ligne)%ligne,(j+l+colonne)%colonne); //k et l négatifs
                    else // cas où j>=0, en haut ou à la même longitude
                        listeVoisin[v] = r.getEtat((i+k+ligne)%ligne,(j+l)%colonne); //k négatif et l positif
                }
                else { // cas où i>=0, à droite ou à la même latitude
                    if (l<0) // en bas strictement
                        listeVoisin[v] = r.getEtat((i+k)%ligne,(j+l+colonne)%colonne); //k positif et l négatif
                    else // cas où j>=0, en haut ou à la même longitude
                        listeVoisin[v] = r.getEtat((i+k)%ligne,(j+l)%colonne); //k et l positifs
                }
                v++;
            }
        }
    }

}

bool VoisinageMooreGeneral::setParameters() {

    bool ok;
    unsigned int rayon = QInputDialog::getInt(nullptr,"Choix du rayon","Choisissez votre rayon :",2,1,50,1,&ok);
    if(!ok || rayon == radius) return false;
    setRadius(rayon);
    return true;

}

bool VoisinageMooreGeneral::setParametersXML(QDomNode& noeud) {

    QDomElement e = noeud.toElement();
    QDomAttr a = e.attributeNode("rayon");
    radius= a.value().toUInt();
    if(radius <= 0) throw(FichierException("Le rayon du fichier XML n'est pas valide"));
    return true;

}

void VoisinageVonNeumanGeneral::listeVoisin(Reseau& r, unsigned int i, unsigned int j,Etat** listeVoisin) const {

    unsigned int colonne = r.getWidth();
    unsigned int ligne = r.getHeight();
    unsigned int indice = 0;

    for (int l = -static_cast<int>(radius); l <= static_cast<int>(radius); l++) {
        for (int c = -static_cast<int>(radius); c <= static_cast<int>(radius); c++) {
            if (abs(c)+abs(l) <= static_cast<int>(radius) && (c != 0 || l != 0)) {
                if (c >= 0 && l >= 0){
                    listeVoisin[indice] = r.getEtat((l+i)%ligne,(c+j)%colonne);
                    indice++;
                }
                else if (c < 0 && l >= 0){
                    listeVoisin[indice] = r.getEtat((l+i)%ligne,(c+j+colonne)%colonne);
                    indice++;
                }
                else if (c >= 0 && l < 0){
                    listeVoisin[indice] = r.getEtat((l+i+ligne)%ligne,(c+j)%colonne);
                    indice++;
                }
                else if (c < 0 && l < 0){
                    listeVoisin[indice] = r.getEtat((l+i+ligne)%ligne,(c+j+colonne)%colonne);
                    indice++;
                }
            }
        }
    }

}

bool VoisinageVonNeumanGeneral::setParameters() {

    bool ok;
    unsigned int rayon = QInputDialog::getInt(nullptr,"Choix du rayon","Choisissez votre rayon :",2,1,50,1,&ok);
    if(!ok || rayon == radius) return false;
    setRadius(rayon);
    return true;

}

bool VoisinageVonNeumanGeneral::setParametersXML(QDomNode& noeud) {

    QDomElement e = noeud.toElement();
    QDomAttr a = e.attributeNode("rayon");
    radius= a.value().toUInt();
    if(radius <= 0) throw(FichierException("Le rayon du fichier XML n'est pas valide"));
    return true;

}

bool VoisinageArbitraire::setParameters() {

    QStringList items;
    map<string, int> coord;
    items << "En haut à gauche" << "En haut" << "En haut à droite" << "à gauche" << "à droite" << "En bas à gauche" << "En bas" << "En bas à droite";
    coord.insert(std::make_pair("En haut à gauche", 0));
    coord.insert(std::make_pair("En haut", 1));
    coord.insert(std::make_pair("En haut à droite", 2));
    coord.insert(std::make_pair("à gauche", 3));
    coord.insert(std::make_pair("à droite", 4));
    coord.insert(std::make_pair("En bas à gauche", 5));
    coord.insert(std::make_pair("En bas", 6));
    coord.insert(std::make_pair("En bas à droite", 7));
    bool ok = true;
    bool modif = false;
    while(ok && !items.isEmpty()){
        QInputDialog fenetre;
        QString item = fenetre.getItem(nullptr,"Choix d'un voisin","Choisissez un nouveau voisin (ou cliquer sur cancel si vous ne souhaitez pas ou plus ajouter) :",items, 0, false, &ok);
        if(ok && !item.isEmpty()){
            int pos = items.indexOf(item);
            items.remove(pos);
            map<string, int>::iterator it = coord.find(item.toStdString());
            pos = it->second;
            arrayVoisin[pos] = 1;
            modif = true;
        }
    }
    for(unsigned int i=0; i<items.size(); i++) {
        int pos;
        map<string, int>::iterator it = coord.find(items[i].toStdString());
        pos = it->second;
        arrayVoisin[pos] = -1;
    }
    if(modif == true) {
        QMessageBox msgBox;
        msgBox.setText("Voisinage correctement ajouté !");
        msgBox.exec();
    } else {
        QMessageBox msgBox;
        msgBox.setText("Attention vous n'avez choisi aucun voisin!\nOn a un voisinage vide.");
        msgBox.exec();
    }
    return modif;

}

void VoisinageArbitraire::listeVoisin(Reseau& r, unsigned int i, unsigned int j, Etat** listeVoisin) const {
    unsigned int colonne = r.getWidth();
    unsigned int ligne = r.getHeight();
    unsigned int indice = 0;
    if (arrayVoisin[0] == 1) {
        listeVoisin[indice] = r.getEtat((i-1+ligne)%ligne,(j-1+colonne)%colonne); // diagonal haut gauche
        indice++;
    }
    if (arrayVoisin[1] == 1) {
        listeVoisin[indice] = r.getEtat((i-1+ligne)%ligne,j); // haut
        indice++;
    }
    if (arrayVoisin[2] == 1) {
        listeVoisin[indice] = r.getEtat((i-1+ligne)%ligne,(j+1)%colonne); // diagonle haut droite
        indice++;
    }
    if (arrayVoisin[3] == 1) {
        listeVoisin[indice] = r.getEtat(i,(j-1+colonne)%colonne); // gauche
        indice++;
    }
    if (arrayVoisin[4] == 1) {
        listeVoisin[indice] = r.getEtat(i,(j+1)%colonne); // droite
        indice++;
    }
    if (arrayVoisin[5] == 1) {
        listeVoisin[indice] = r.getEtat((i+1)%ligne,(j-1+colonne)%colonne); // diagonale bas gauche
        indice++;
    }
    if (arrayVoisin[6] == 1) {
        listeVoisin[indice] = r.getEtat((i+1)%ligne,j); // bas
        indice++;
    }
    if (arrayVoisin[7] == 1) {
        listeVoisin[indice] = r.getEtat((i+1)%ligne,(j+1)%colonne); // diagonale bas droite
        indice++;
    }

}

bool VoisinageArbitraire::setParametersXML(QDomNode& noeud) {

    QDomElement e = noeud.toElement();
    QDomAttr a0 = e.attributeNode("hautGauche");
    QDomAttr a1 = e.attributeNode("haut");
    QDomAttr a2 = e.attributeNode("hautDroite");
    QDomAttr a3 = e.attributeNode("gauche");
    QDomAttr a4 = e.attributeNode("droite");
    QDomAttr a5 = e.attributeNode("basGauche");
    QDomAttr a6 = e.attributeNode("bas");
    QDomAttr a7 = e.attributeNode("basDroite");
    arrayVoisin[0] = a0.value().toInt();
    arrayVoisin[1] = a1.value().toInt();
    arrayVoisin[2] = a2.value().toInt();
    arrayVoisin[3] = a3.value().toInt();
    arrayVoisin[4] = a4.value().toInt();
    arrayVoisin[5] = a5.value().toInt();
    arrayVoisin[6] = a6.value().toInt();
    arrayVoisin[7] = a7.value().toInt();
    for(unsigned int i = 0; i<8; i++)
        if(arrayVoisin[i] != 1 && arrayVoisin[i] != -1) throw(FichierException("Un des voisins n'a pas la bonne valeur dans le fichier XML"));
    return true;

}

void AutomateGriffeath::appliquerTransition(Reseau& r) const {

    // Récupération nbLigne et nbColonne
    unsigned int nbLigne = r.getHeight();
    unsigned int nbColonne = r.getWidth();

    // Création d'un tableau temporaire contenant des pointeurs sur les états après transitions (afin de ne pas modifier le réseau en cours)
    Etat*** tmp = new Etat**[nbLigne];
    for(unsigned int i = 0; i < nbLigne; i++) {
        tmp[i] = new Etat*[nbColonne];
        for(unsigned int j = 0; j < nbColonne; j++) {
            tmp[i][j] = r.getEtat(i,j);
        }
    }

    // Recupération du Manager d'états qui va nous aider à trouver les états suivants
    EtatManager& em = EtatManager::getManager();

    // On récupère le nombre de voisins de notre règle de voisinage, puis on créer un tableau qui contiendra ce nombre de pointeur vers les etats, ce sont les états voisins
    unsigned int nbVoisin = voisinage->getNbVoisin();
    Etat** liste = new Etat*[nbVoisin];

    // Réalisation de la transtion
    for(unsigned int i = 0; i < nbLigne; i++) {
        for(unsigned int j = 0; j < nbColonne; j++) {
            // On obtient la liste des voisins de la cellule en cours de traitement
            voisinage->listeVoisin(r,i,j,liste);
            // On initiliase le nombre de voisins suivants à 0 et on récupère l'état suivant
            unsigned int nbVoisinSuivant = 0;
            Etat* etatSuivant = em.getEtat((r.getEtat(i,j)->getIndice()+1)%em.getNbEtats());
            // On compte le nombre de voisin étant dans l'état suivant
            for(unsigned int k = 0; k < nbVoisin; k++)
                if(liste[k] == etatSuivant) nbVoisinSuivant +=1;
            // Si ce nombre est >=  3 alors on passe la cellule à l'état suivant
            if(nbVoisinSuivant >= 3) tmp[i][j] = etatSuivant;
        }
    }

    // On modifie enfin le réseau en modifiant les états des cellules
    for(unsigned int i = 0; i< nbLigne; i++) {
        for(unsigned int j = 0; j< nbColonne; j++)
            r.setCellule(i,j,tmp[i][j]);
    }

    // On désalloue le tableau que l'on a alloué dynamiquement
    for(unsigned int i = 0; i< nbLigne; i++)
         delete[] tmp[i];
    delete[] tmp;
    delete[] liste;

}

void AutomateGameLife::appliquerTransition(Reseau& r) const {

    // Récupération nbLigne et nbColonne
    unsigned int height = r.getHeight();
    unsigned int width = r.getWidth();

    // Création d'un tableau temporaire contenant des pointeurs sur les états après transition (afin de ne pas modifier le réseau en cours)
    Etat*** tmp = new Etat**[height];
    for(unsigned int i = 0; i < height; i++) {
        tmp[i] = new Etat*[width];
        for(unsigned int j = 0; j < width; j++)
            tmp[i][j] = r.getEtat(i,j);
    }

    // Récupération du Manager d'états qui va nous aider à trouver les états suivants
    EtatManager& em = EtatManager::getManager();

    // On récupère le nombre de voisins de notre règle de voisinage, puis on crée un tableau qui contiendra ce nombre de pointeurs vers les états, ce sont les états voisins
    unsigned int nbVoisin = voisinage->getNbVoisin();
    Etat** liste = new Etat*[nbVoisin];

    // Réalisation de la transtion
    for(unsigned int i = 0; i < height; i++) {
        for(unsigned int j = 0; j < width; j++) {
            // On obtient la liste des voisins de la cellule en cours de traitement
            voisinage->listeVoisin(r,i,j,liste);
            unsigned int nbVoisinVivant = 0;
            // On compte le nombre de voisins vivants
            for(unsigned int k = 0; k < nbVoisin; k++)
                if(liste[k]->getIndice() == 1) nbVoisinVivant +=1;
            // Si la cellule est vivante et que le nombre de voisins vivants est égal à 2 ou 3 alors la cellule reste vivante
            if(r.getEtat(i,j)->getIndice() == 1 and (nbVoisinVivant == 2 or nbVoisinVivant == 3))
                tmp[i][j] = r.getEtat(i,j);
            // Sinon elle meurt
            else if (r.getEtat(i,j)->getIndice() == 1)
                tmp[i][j] = em.getEtat((r.getEtat(i,j)->getIndice()+1)%em.getNbEtats());
            // Si la cellule est morte et que le nombre de voisins vivants est égal à 3 alors la cellule naît
            else if(r.getEtat(i,j)->getIndice() == 0 and nbVoisinVivant == 3)
                tmp[i][j] = em.getEtat((r.getEtat(i,j)->getIndice()+1)%em.getNbEtats());
            // Sinon elle reste à l'état mort
            else if (r.getEtat(i,j)->getIndice() == 0)
                tmp[i][j] = r.getEtat(i,j);
        }
    }

    // On modifie enfin le réseau en modifiant les états des cellules
    for(unsigned int i = 0; i< height; i++) {
        for(unsigned int j = 0; j< width; j++)
            r.setCellule(i, j, tmp[i][j]);
    }

    // On désalloue le tableau que l'on a alloué dynamiquement
    for(unsigned int i = 0; i < height; i++)
         delete[] tmp[i];
    delete[] tmp;
    delete[] liste;

}

void AutomateBriansBrain::appliquerTransition(Reseau& r) const {

    // Récupération nbLigne et nbColonne
    unsigned int height = r.getHeight();
    unsigned int width = r.getWidth();

    // Création d'un tableau temporaire contenant des pointeurs sur les états après transition (afin de ne pas modifier le réseau en cours)
    Etat*** tmp = new Etat**[height];
    for(unsigned int i = 0; i < height; i++) {
        tmp[i] = new Etat*[width];
        for(unsigned int j = 0; j < width; j++)
            tmp[i][j] = r.getEtat(i,j);
    }

    // Récupération du Manager d'états qui va nous aider à trouver les états suivants
    EtatManager& em = EtatManager::getManager();

    // On récupère le nombre de voisins de notre règle de voisinage, puis on créer un tableau qui contiendra ce nombre de pointeurs vers les états, ce sont les états voisins
    unsigned int nbVoisin = voisinage->getNbVoisin();
    Etat** liste = new Etat*[nbVoisin];

    // Réalisation de la transtion
    for(unsigned int i = 0; i < height; i++) {
        for(unsigned int j = 0; j < width; j++) {
            // On obtient la liste des voisins de la cellule en cours de traitement
            voisinage->listeVoisin(r,i,j,liste);
            unsigned int nbVoisinVivant = 0;
            // On compte le nombre de voisins vivants
            for(unsigned int k = 0; k < nbVoisin; k++)
                if(liste[k]->getIndice() == 1) nbVoisinVivant +=1;
            // Si la cellule est vivante alors elle devient mourante
            if(r.getEtat(i,j)->getIndice() == 1)
                tmp[i][j] = em.getEtat((r.getEtat(i,j)->getIndice()+1)%em.getNbEtats());
            // Si la cellule est mourante alors elle meurt
            else if (r.getEtat(i,j)->getIndice() == 2)
                tmp[i][j] = em.getEtat((r.getEtat(i,j)->getIndice()+1)%em.getNbEtats());
            // Si la cellule est morte et que le nombre de voisins vivants est égal à 2 alors la cellule naît
            else if(r.getEtat(i,j)->getIndice() == 0 and nbVoisinVivant == 2)
                tmp[i][j] = em.getEtat((r.getEtat(i,j)->getIndice()+1)%em.getNbEtats());
            // Sinon elle reste à l'état mort
            else if (r.getEtat(i,j)->getIndice() == 0)
                tmp[i][j] = r.getEtat(i,j);
        }
    }

    // On modifie enfin le réseau en modifiant les états des cellules
    for(unsigned int i = 0; i< height; i++) {
        for(unsigned int j = 0; j< width; j++)
            r.setCellule(i, j, tmp[i][j]);
    }

    // On désalloue le tableau que l'on a alloué dynamiquement
    for(unsigned int i = 0; i < height; i++)
         delete[] tmp[i];
    delete[] tmp;
    delete[] liste;

}

void AutomateLangtonLoop::appliquerTransition(Reseau& r) const {

    // Récupération nbLigne et nbColonne
    unsigned int height = r.getHeight();
    unsigned int width = r.getWidth();

    // Recupération du Manager d'états qui va nous aider à trouver les états suivants
    EtatManager& em = EtatManager::getManager();
    unsigned int nbVoisin = voisinage->getNbVoisin();
    if(nbVoisin < 4) return;
    Etat** liste = new Etat*[nbVoisin];

    // Création d'un tableau temporaire contenant des pointeurs sur les états après transition (afin de ne pas modifier le réseau en cours)
    Etat*** tmp = new Etat**[height];
    for(unsigned int i = 0; i < height; i++) {
        tmp[i] = new Etat*[width];
        for(unsigned int j = 0; j < width; j++) {
            tmp[i][j] = r.getEtat(i,j);
        }
    }

    for(unsigned int i = 0; i < height; i++) {
        for(unsigned int j = 0; j < width; j++) {
            // On obtient la liste des voisins de la cellule en cours de traitement
            voisinage->listeVoisin(r,i,j,liste);
            std::string to_change = std::to_string(r.getEtat(i,j)->getIndice());
            bool find = false;
            string a1 = std::to_string(liste[2]->getIndice());
            string a2 = std::to_string(liste[1]->getIndice());
            string a3 = std::to_string(liste[3]->getIndice());
            string a4 = std::to_string(liste[0]->getIndice());
            // On teste si la cellule étudié est dans une position (son état et ses voisins) qui correspond à une position listé des 29 règles
            for (unsigned int t=0;t<4;t++){
                string chaine;
                switch (t) {
                case 0:
                    chaine = to_change+a1+a2+a3+a4;
                    break;
                case 1:
                    chaine = to_change+a2+a3+a4+a1;
                    break;
                case 2:
                    chaine = to_change+a3+a4+a1+a2;
                    break;
                case 3:
                    chaine = to_change+a4+a1+a2+a3;
                    break;
                }
                if(!find && rules.find(chaine)!=rules.end()) {
                     find = true;
                     int change = std::stoi(rules[chaine]);
                     switch (change)
                          {
                             case 0:
                         tmp[i][j] = em.getEtat(0);
                                break;
                             case 1:
                         tmp[i][j] = em.getEtat(1);
                                break;
                             case 2:
                         tmp[i][j] = em.getEtat(2);
                                break;
                             case 3:
                         tmp[i][j] = em.getEtat(3);
                                 break;
                             case 4:
                         tmp[i][j] = em.getEtat(4);
                                  break;
                             case 5:
                         tmp[i][j] = em.getEtat(5);
                                 break;
                             case 6:
                         tmp[i][j] = em.getEtat(6);
                                 break;
                             case 7:
                         tmp[i][j] = em.getEtat(7);
                                 break;
                          }
                 }
            }
            if(!find) tmp[i][j] = r.getEtat(i,j);
        }
    }

    // On modifie enfin le réseau en modifiant les états des cellules
    for(unsigned int i = 0; i< height; i++) {
        for(unsigned int j = 0; j< width; j++)
            r.setCellule(i, j, tmp[i][j]);
    }

    // On désalloue le tableau que l'on a alloué dynamiquement
    for(unsigned int i = 0; i < height; i++)
         delete[] tmp[i];
    delete[] tmp;
    delete[] liste;

}

void AutomateWireWorld::appliquerTransition(Reseau& r) const {

    // Récupération nbLigne et nbColonne
    unsigned int height = r.getHeight();
    unsigned int width = r.getWidth();

    // Recupération du Manager d'états qui va nous aider à trouver les états suivants
    EtatManager& em = EtatManager::getManager();

    // Création d'un tableau temporaire contenant des pointeurs sur les états après transition (afin de ne pas modifier le réseau en cours)
    Etat*** tmp = new Etat**[height];
    for(unsigned int i = 0; i < height; i++) {
        tmp[i] = new Etat*[width];
        for(unsigned int j = 0; j < width; j++) {
            tmp[i][j] = r.getEtat(i,j);
        }
    }

    unsigned int nbVoisin = voisinage->getNbVoisin();
    Etat** liste = new Etat*[nbVoisin];
    for(unsigned int i = 0; i < height; i++) {
        for(unsigned int j = 0; j < width; j++) {
            // On obtient la liste des voisins de la cellule en cours de traitement
            voisinage->listeVoisin(r,i,j,liste);
            unsigned int nbVoisinHead = 0;
            // On compte le nombre de voisins blue
            for(unsigned int k = 0; k < nbVoisin; k++)
                if(liste[k]->getIndice() == 2) nbVoisinHead +=1;
            switch (tmp[i][j]->getIndice()){
                case 1:
                    if (nbVoisinHead == 1 || nbVoisinHead == 2)
                        tmp[i][j] = em.getEtat(2);
                    break;
                case 2:
                    tmp[i][j] = em.getEtat(3);
                    break;

                case 3:
                    tmp[i][j] = em.getEtat(1);
                    break;
                default:
                    break;
            }
        }
    }

    // On modifie enfin le réseau en modifiant les états des cellules
    for(unsigned int i = 0; i< height; i++) {
        for(unsigned int j = 0; j< width; j++)
            r.setCellule(i, j, tmp[i][j]);
    }

    // On désalloue le tableau que l'on a alloué dynamiquement
    for(unsigned int i = 0; i < height; i++)
         delete[] tmp[i];
    delete[] tmp;
    delete[] liste;

}
