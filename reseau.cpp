#include "reseau.h"

void Reseau::drawGrid(QBoxLayout* parent) {

    if (gridTableWidget != nullptr)
        delete gridTableWidget;

    gridTableWidget = new QTableWidget(height, width);
    parent->addWidget(gridTableWidget);

    gridTableWidget->setFixedSize((650.0/max(height, width))*width, (650.0/max(height, width))*height);
    gridTableWidget->horizontalHeader()->setVisible(false);
    gridTableWidget->verticalHeader()->setVisible(false);
    gridTableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gridTableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gridTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    gridTableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    gridTableWidget->horizontalHeader()->setStyleSheet("QHeaderView { font-size: 1pt; }");
    gridTableWidget->verticalHeader()->setStyleSheet("QHeaderView { font-size: 1pt; }");
    gridTableWidget->setStyleSheet("QTableWidget::item {font-size: 1pt;}");

    for (unsigned int i=0; i < height; i++) {
        gridTableWidget->setRowHeight(i, 650.0/max(height, width));
        for (unsigned int j=0; j < width; j++) {
            gridTableWidget->setColumnWidth(j, 650.0/max(height, width));
            gridTableWidget->setItem(i, j, new QTableWidgetItem());
        }
    }

    connect(gridTableWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(cellActivation(QModelIndex)));

}

Reseau::Reseau(unsigned int w, unsigned int h): width(w), height(h), heightInit(h) {
    cells = new Etat**[h];
    cellsInit = new Etat**[h];
    for(unsigned int i = 0; i < height; i++) {
        cells[i] = new Etat*[w];
        cellsInit[i] = new Etat*[w];
        for(unsigned int j = 0; j < width; j++) {
            cells[i][j] = nullptr;
            cellsInit[i][j] = nullptr;
        }
    }
}

Reseau::~Reseau() {
    delete gridTableWidget;

    for (unsigned int i=0; i<height; i++){
        delete[] cells[i];
   }

   delete[] cells;
   if(cellsInit != nullptr){
       for (unsigned int i=0; i<heightInit; i++){
            delete[] cellsInit[i];
       }
       delete[] cellsInit;
   }
}

// Changement manuel de l'état d'une cellule
void Reseau::cellActivation(const QModelIndex& index) {

    EtatManager& em = EtatManager::getManager();
    Etat* etatSuivant = em.getEtat((getEtat(index.row(),index.column())->getIndice()+1)%em.getNbEtats());
    setCellule(index.row(), index.column(), etatSuivant);
    actualiserGrille(1);

}

// Mise à jour de la largeur du réseau
void Reseau::setWidth(unsigned int w) {

    // On crée le nouveau réseau selon les nouvelles dimensions puis on supprime l'ancien
    EtatManager& em = EtatManager::getManager();
    Etat*** newtab = new Etat**[height];
    for(unsigned int i = 0; i < height; i++) {
       newtab[i] = new Etat*[w];
       for(unsigned int j = 0; j < w; j++)
            newtab[i][j] = em.getEtat(0);
    }
    for(unsigned int i = 0; i < height; i++)
       delete[] cells[i];
    delete[] cells;
    cells = newtab;
    width = w;

}

// Mise à jour de la hauteur du réseau
void Reseau::setHeight(unsigned int h) {

    // On crée le nouveau réseau selon les nouvelles dimensions puis on supprime l'ancien
    EtatManager& em = EtatManager::getManager();
    Etat*** newtab = new Etat**[h];
    for(unsigned int i = 0; i < h; i++) {
       newtab[i] = new Etat*[width];
       for(unsigned int j = 0; j < width; j++)
            newtab[i][j] = em.getEtat(0);
    }
    for(unsigned int i = 0; i < height; i++)
       delete[] cells[i];
    delete[] cells;
    cells = newtab;
    height = h;

}

Reseau::Memento::Memento(unsigned int w, unsigned int h, Etat*** e) : width(w), height(h), cells(new Etat**[h]) {

    for (unsigned int i=0; i<height; i++) {
        cells[i] = new Etat*[width];
        for (unsigned int j=0; j<width; j++)
            cells[i][j] = e[i][j];
    }

}

Reseau::Memento::~Memento() {

    for (unsigned int i=0; i<height; i++)
        delete[] cells[i];
    delete[] cells;

}

Reseau::Memento::Memento(const Memento& a){

    width = a.getWidth();
    height = a.getHeight();
    cells = new Etat**[height];
    for (unsigned int i=0; i<height; i++) {
        cells[i] = new Etat*[width];
        for (unsigned int j=0; j<width; j++)
            cells[i][j] = a.getEtat(i,j);
    }

}

Reseau::Memento Reseau::Memento::operator=(const Memento& a){

    if (this != &a){
        width = a.getWidth();
        height = a.getHeight();
        for (unsigned int i=0; i<height; i++)
            delete[] cells[i];
        delete[] cells;
        cells = nullptr;
        if (a.cells != nullptr) {
            cells = new Etat**[height];
            for (unsigned int i=0; i<height; i++) {
                cells[i] = new Etat*[width];
                for (unsigned int j=0; j<width; j++)
                    cells[i][j] = a.getEtat(i,j);
            }
        }
    }
    return *this;

}

Reseau::Memento Reseau::save() {

    Reseau::Memento memento = Reseau::Memento(width, height, cells);
    return memento;

}

void Reseau::restore(Reseau::Memento backup) {

    width = backup.getWidth();
    height = backup.getHeight();
    cells = backup.getEtat();

}

// On met à jour la grille en récupérant la couleur de chaque cellule du réseau
// saveSignal = 0 lorsque l'on actualise la grille suite au "bouton précédent"
// saveSignal = 1 sinon
void Reseau::actualiserGrille(unsigned int saveSignal) {

    if (saveSignal == 1)
        emit signalForSaveSimuConfiguration();
    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            if(cells[i][j] != nullptr)
                gridTableWidget->item(i,j)->setBackground(cells[i][j]->getCouleur());
            else
                gridTableWidget->item(i,j)->setBackground(Qt::white);
        }
    }

}

// Génération d'une grille aléatoire
void Reseau::generateRandomGrid() {

    EtatManager& em = EtatManager::getManager();
    for(unsigned int i = 0; i < height; i++) {
        cells[i] = new Etat*[width];
        for(unsigned int j = 0; j < width; j++)
            cells[i][j] = nullptr;
    }
    for(unsigned int i = 0; i < height; i++) {
        for(unsigned int j = 0; j < width; j++)
            setCellule(i, j, em.getEtat(rand()%em.getNbEtats()));
    }
    gridTableWidget->setEnabled(true);

}

// Définition de la configuration initiale pour la simulation
void Reseau::setInit() {

    for(unsigned int i = 0; i < heightInit; i++)
        delete[] cellsInit[i];
    delete[] cellsInit;
    heightInit = height;
    cellsInit = new Etat**[height];
    for(unsigned int i = 0; i < height; i++) {
        cellsInit[i] = new Etat*[width];
        for(unsigned int j = 0; j < width; j++)
            setCelluleInit(i, j, getEtat(i, j));
    }

}

// Retour à la configuration initiale au cours de la simulation
void Reseau::resetToInit() {

    for(unsigned int i = 0; i < height; i++) {
        for(unsigned int j = 0; j < width; j++)
            setCellule(i, j, getEtatInit(i, j));
    }
    printInstance();

}

// Affichage de la période de l'instance
void Reseau::printInstance() {

    std::string info = "Vous êtes revenu à la configuration initiale en " + std::to_string(nbPasToInit) + " pas.";
    QMessageBox::about(this, tr("Retour à la configuration initiale"), tr(info.c_str()));
    resetNbPasToInit();

}
