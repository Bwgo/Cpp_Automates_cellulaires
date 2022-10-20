#include "mainwindow.h"
#include "reseau.h"
#include <QtWidgets>

// Création des différents widgets de la fenêtre et ajout de ceux-ci au layout
MainWindow::MainWindow(QWidget *parent):QMainWindow(parent){

    setWindowTitle(tr("Cellulut"));

    createMenu();
    createModelGroupBox();
    createModelParameterGroupBox();
    createGridParameterGroupBox();
    createConfigurationGroupBox();
    createSimulationGroupBox();
    createControlGroupBox();

    QVBoxLayout* commandLayout = new QVBoxLayout;
    commandLayout->addWidget(modeleGroupBox);
    commandLayout->addWidget(modelParameterGroupBox);
    commandLayout->addWidget(configurationGroupBox);
    commandLayout->addWidget(gridParameterGroupBox);
    commandLayout->addWidget(simulationGroupBox);
    commandLayout->addWidget(controlGroupBox);

    gridLayout = new QVBoxLayout;
    res.drawGrid(gridLayout);
    res.getGridTable()->setEnabled(false);

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->setMenuBar(menuBar);
    mainLayout->addLayout(commandLayout);
    mainLayout->addLayout(gridLayout);

    QWidget* wrapper = new QWidget(this);
    wrapper->setLayout(mainLayout);
    setCentralWidget(wrapper);

}

// Destruction de la MainWindow ainsi que du simulateur dont elle s'occupe
MainWindow::~MainWindow() {

    delete simu;

}

// Création de la barre de menu
void MainWindow::createMenu() {

    menuBar = new QMenuBar;

    fileMenu = new QMenu(tr("Fichier"), this);
    exitAction = fileMenu->addAction(tr("Quitter"));
    exitAction->setStatusTip(tr("Fichier"));
    menuBar->addMenu(fileMenu);
    connect(exitAction, &QAction::triggered, this, &QApplication::quit);

    infoMenu = new QMenu(tr("Informations"), this);
    aboutAction = infoMenu->addAction(tr("À propos"));
    aboutAction->setStatusTip(tr("À propos"));
    menuBar->addMenu(infoMenu);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);

}

// Création du groupe pour gérer les modèles
void MainWindow::createModelGroupBox() {

    modeleGroupBox = new QGroupBox(tr("Modèle d'automate"));
    QHBoxLayout* layout = new QHBoxLayout;

    openModeleButton = new QPushButton("Ouvrir un modèle existant");
    layout->addWidget(openModeleButton);
    connect(openModeleButton, SIGNAL(clicked()), this, SLOT(loadModele()));

    saveModeleButton = new QPushButton("Enregistrer le modèle courant");
    layout->addWidget(saveModeleButton);
    saveModeleButton->setEnabled(false);
    connect(saveModeleButton, SIGNAL(clicked()), this, SLOT(saveModele()));

    modeleGroupBox->setLayout(layout);

}

// Création du groupe pour gérer les paramètres du modèle
void MainWindow::createModelParameterGroupBox() {

    modelParameterGroupBox = new QGroupBox(tr("Paramètres du modèle"));
    QHBoxLayout* layout = new QHBoxLayout;
	
    ruleButton = new QPushButton("Choisir une règle de transition");
    layout->addWidget(ruleButton);
    connect(ruleButton, SIGNAL(clicked()), this, SLOT(setRegle()));

    neighborhoodButton = new QPushButton("Choisir et configurer un voisinage");
    layout->addWidget(neighborhoodButton);
    neighborhoodButton->setEnabled(false);
    connect(neighborhoodButton, SIGNAL(clicked()), this, SLOT(setVoisinage()));

    etatButton = new QPushButton("Configurer les états");
    layout->addWidget(etatButton);
    etatButton->setEnabled(false);
    connect(etatButton, SIGNAL(clicked()), this, SLOT(setEtat()));

    modelParameterGroupBox->setLayout(layout);

}

// Création du groupe pour gérer les paramètres de la grille
void MainWindow::createGridParameterGroupBox() {

    gridParameterGroupBox = new QGroupBox(tr("Paramètres de la grille"));
    QHBoxLayout* layout1 = new QHBoxLayout;
    QVBoxLayout* layout2 = new QVBoxLayout;
    QVBoxLayout* layout = new QVBoxLayout;

    QLabel* gridWidthLabel = new QLabel("Largeur de la grille");
    gridWidthSpinBox = new QSpinBox;
    gridWidthSpinBox->setRange(1,50);
    gridWidthSpinBox->setValue(20);
    gridWidthSpinBox->setEnabled(false);
    gridWidthButton = new QPushButton("Valider");
    gridWidthButton->setEnabled(false);
    connect(gridWidthButton, SIGNAL(clicked()), this, SLOT(setGridWidth()));
    QVBoxLayout* gridWidthLayout = new QVBoxLayout;
    gridWidthLayout->addWidget(gridWidthLabel);
    gridWidthLayout->addWidget(gridWidthSpinBox);
    gridWidthLayout->addWidget(gridWidthButton);
    layout1->addLayout(gridWidthLayout);

    QLabel* gridHeightLabel = new QLabel("Hauteur de la grille");
    gridHeightSpinBox = new QSpinBox;
    gridHeightSpinBox->setRange(1,50);
    gridHeightSpinBox->setValue(20);
    gridHeightSpinBox->setEnabled(false);
    gridHeightButton = new QPushButton("Valider");
    gridHeightButton->setEnabled(false);
    connect(gridHeightButton, SIGNAL(clicked()), this, SLOT(setGridHeight()));
    QVBoxLayout* gridHeightLayout = new QVBoxLayout;
    gridHeightLayout->addWidget(gridHeightLabel);
    gridHeightLayout->addWidget(gridHeightSpinBox);
    gridHeightLayout->addWidget(gridHeightButton);
    layout1->addLayout(gridHeightLayout);

    gridRandomButton = new QPushButton("Générer une grille aléatoire");
    gridRandomButton->setEnabled(false);
    connect(gridRandomButton, SIGNAL(clicked()), this, SLOT(setRandomGrid()));

    layout->addLayout(layout1);
    layout->addLayout(layout2);
    layout->addWidget(gridRandomButton);

    gridParameterGroupBox->setLayout(layout);

}

// Création du groupe pour gérer les configurations
void MainWindow::createConfigurationGroupBox() {

    configurationGroupBox = new QGroupBox(tr("Configuration"));
    QHBoxLayout* layout = new QHBoxLayout;

    configurationLoadButton = new QPushButton("Charger une configuration");
    layout->addWidget(configurationLoadButton);
    configurationLoadButton->setEnabled(false);
    connect(configurationLoadButton, SIGNAL(clicked()), this, SLOT(loadConfiguration()));

    configurationSaveButton = new QPushButton("Sauvegarder la configuration actuelle");
    layout->addWidget(configurationSaveButton);
    configurationSaveButton->setEnabled(false);
    connect(configurationSaveButton, SIGNAL(clicked()), this, SLOT(saveConfiguration()));

    configurationGroupBox->setLayout(layout);

}

// Création du groupe pour gérer la simulation
void MainWindow::createSimulationGroupBox() {

    simulationGroupBox = new QGroupBox(tr("Simulation"));
    QHBoxLayout* layout0 = new QHBoxLayout;
    QHBoxLayout* layout1 = new QHBoxLayout;
    QHBoxLayout* layout2 = new QHBoxLayout;
    QVBoxLayout* layout = new QVBoxLayout;

    configurationInitButton = new QPushButton("Définir la configuration actuelle comme configuration initiale");
    configurationInitButton->setEnabled(false);
    connect(configurationInitButton, SIGNAL(clicked()), this, SLOT(setConfigurationInit()));
    layout0->addWidget(configurationInitButton);

    previousStateButton = new QPushButton("Etat précédent");
    previousStateButton->setEnabled(false);
    connect(previousStateButton, SIGNAL(clicked()), this, SLOT(pasPrecedent()));
    layout1->addWidget(previousStateButton);

    QLabel* nbPreviousLabel = new QLabel("Nombre de configurations précédentes enregistrables");
    previousNumberSpinBox = new QSpinBox;
    previousNumberSpinBox->setRange(1,50);
    previousNumberSpinBox->setValue(10);
    previousNumberButton = new QPushButton("Valider");
    connect(previousNumberButton, SIGNAL(clicked()), this, SLOT(setNbBackup()));
    QVBoxLayout* nbPreviousLayout = new QVBoxLayout;
    nbPreviousLayout->addWidget(nbPreviousLabel);
    nbPreviousLayout->addWidget(previousNumberSpinBox);
    nbPreviousLayout->addWidget(previousNumberButton);
    layout2->addLayout(nbPreviousLayout);

    playButton = new QPushButton("Simulation");
    playButton->setEnabled(false);
    connect(playButton, SIGNAL(clicked()), this, SLOT(lancerSimulation()));
    layout1->addWidget(playButton);

    pauseButton = new QPushButton("Arrêt");
    pauseButton->setEnabled(false);
    connect(pauseButton, SIGNAL(clicked()), this, SLOT(arreterSimulation()));
    layout1->addWidget(pauseButton);

    nextStateButton = new QPushButton("Etat suivant");
    nextStateButton->setEnabled(false);
    connect(nextStateButton, SIGNAL(clicked()), this, SLOT(pasSuivant()));
    layout1->addWidget(nextStateButton);

    resetButton = new QPushButton("Réinitialiser");
    resetButton->setEnabled(false);
    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetConfiguration()));
    layout1->addWidget(resetButton);

    //signal à chaque actualisation de la grille
    connect(&res, SIGNAL(signalForSaveSimuConfiguration()), this, SLOT(saveSimuConfiguration()));

    layout->addLayout(layout0);
    layout->addLayout(layout1);
    layout->addLayout(layout2);

    simulationGroupBox->setLayout(layout);

}

// Création du groupe qui gère les controles de la simulation
void MainWindow::createControlGroupBox() {

    controlGroupBox = new QGroupBox(tr("Contrôles"));
    QHBoxLayout* layout = new QHBoxLayout;

    QLabel* speedLabel = new QLabel("Vitesse");
    speedSpinBox = new QSpinBox;
    speedSpinBox->setRange(1,50);
    speedSpinBox->setValue(10);
    speedButton = new QPushButton("Valider");
    connect(speedButton, SIGNAL(clicked()), this, SLOT(setSpeed()));
    QVBoxLayout* speedLayout = new QVBoxLayout;
    speedLayout->addWidget(speedLabel);
    speedLayout->addWidget(speedSpinBox);
    speedLayout->addWidget(speedButton);
    layout->addLayout(speedLayout);

    controlGroupBox->setLayout(layout);

}


// Affichage des informations sur le projet
void MainWindow::about() {

    QMessageBox::about(this, tr("À propos"),
        tr("L'application <b>Cellulut</b> est destinée à simuler des automates cellulaires. "
        "Elle a été conçue et développée dans le cadre des projets de l'UV LO21 par "
        "Aurélie Law-Yen, Damien Willett, Théo Brochard, Thomas Bossuat et Yanan Fu."));

}


// Mise à jour de la largeur de la grille
void MainWindow::setGridWidth() {

    if(res.getWidth() == static_cast<unsigned int>(gridWidthSpinBox->value())) return;
    res.setWidth(gridWidthSpinBox->value());
    res.drawGrid(gridLayout);
    res.actualiserGrille(1);
    care.emptyPreviousConfigurations();
    previousStateButton->setEnabled(false);
    nextStateButton->setEnabled(false);
    pauseButton->setEnabled(false);
    playButton->setEnabled(false);
    resetButton->setEnabled(false);
    res.getGridTable()->setEnabled(true);

}

// Mise à jour de la hauteur de la grille
void MainWindow::setGridHeight() {

    if(res.getHeight() == static_cast<unsigned int>(gridHeightSpinBox->value())) return;
    res.setHeight(gridHeightSpinBox->value());
    res.drawGrid(gridLayout);
    res.actualiserGrille(1);
    care.emptyPreviousConfigurations();
    previousStateButton->setEnabled(false);
    nextStateButton->setEnabled(false);
    pauseButton->setEnabled(false);
    playButton->setEnabled(false);
    resetButton->setEnabled(false);
    res.getGridTable()->setEnabled(true);

}

// Génération d'une grille aléatoire
void MainWindow::setRandomGrid() {

    res.generateRandomGrid();
    res.actualiserGrille(1);
    care.setNewClickPrevious(1);
    openModeleButton->setEnabled(true);
    saveModeleButton->setEnabled(true);
    neighborhoodButton->setEnabled(true);
    ruleButton->setEnabled(true);
    gridWidthButton->setEnabled(true);
    gridHeightButton->setEnabled(true);
    gridRandomButton->setEnabled(true);
    configurationSaveButton->setEnabled(true);
    configurationLoadButton->setEnabled(true);
    gridWidthSpinBox->setEnabled(true);
    gridHeightSpinBox->setEnabled(true);
    configurationInitButton->setEnabled(true);
    previousStateButton->setEnabled(false);
    nextStateButton->setEnabled(false);
    pauseButton->setEnabled(false);
    playButton->setEnabled(false);
    resetButton->setEnabled(false);

}

// Définition de la configuration initiale
void MainWindow::setConfigurationInit() {

    care.emptyPreviousConfigurations();
    res.setInit();
    res.resetNbPasToInit();
    saveSimuConfiguration();
    playButton->setEnabled(true);
    nextStateButton->setEnabled(true);
    previousStateButton->setEnabled(false);
    resetButton->setEnabled(false);
    speedButton->setEnabled(true);
    speedSpinBox->setEnabled(true);

}

// Passage au pas suivant de la simulation
void MainWindow::pasSuivant() {

    // On regarde si on a fait un pas précédent avant ce pas suivant pour savoir si on doit stocker l'état
    if(care.getNewClickPrevious()==0)
        saveSimuConfiguration();

    simu->suivant();

    care.setNewClickPrevious(1);
    resetButton->setEnabled(true);
    if (care.getnbprevious() != 0)
        previousStateButton->setEnabled(true);
    else
        previousStateButton->setEnabled(false);

}

// Retour au pas précédent de la simulation
void MainWindow::pasPrecedent() {

    EtatManager& em = EtatManager::getManager();
    if (care.getNewClickPrevious()==1){
        care.removelastone();
        care.setNewClickPrevious(0);
    }
    Reseau::Memento *tableau_prece = care.depilerprevious();
    for (unsigned int i=0;i<res.getHeight();i++){
        for(unsigned int j = 0;j<res.getWidth();j++){
            unsigned int valij = tableau_prece->getEtat(i, j)->getIndice();
            res.setCellule(i,j,em.getEtat(valij));//renvoi des valeurs incohérentes
        }
    }
    res.actualiserGrille(0);
    res.decrementNbPasToInit();
    if (care.getnbprevious() != 0 && !(care.getnbprevious() == 1 && care.getNewClickPrevious()==1))
        previousStateButton->setEnabled(true);
    else
        previousStateButton->setEnabled(false);
    resetButton->setEnabled(true);

}

// Lancement de la simulation
void MainWindow::lancerSimulation() {

    // Mise à jour de l'utilisation possible ou non des boutons de l'interface
    openModeleButton->setEnabled(false);
    saveModeleButton->setEnabled(false);
    neighborhoodButton->setEnabled(false);
    ruleButton->setEnabled(false);
    gridWidthButton->setEnabled(false);
    gridHeightButton->setEnabled(false);
    gridRandomButton->setEnabled(false);
    configurationSaveButton->setEnabled(false);
    configurationLoadButton->setEnabled(false);
    configurationInitButton->setEnabled(false);
    previousStateButton->setEnabled(false);
    playButton->setEnabled(false);
    nextStateButton->setEnabled(false);
    gridWidthSpinBox->setEnabled(false);
    gridHeightSpinBox->setEnabled(false);
    etatButton->setEnabled(false);
    pauseButton->setEnabled(true);
    speedButton->setEnabled(true);
    speedSpinBox->setEnabled(true);

    pasSuivant();
    resetButton->setEnabled(false);
    previousStateButton->setEnabled(false);

    if (res.getNbPasToInit() > 0)
        simu->simulation();

    connect(simu->getTimer(),SIGNAL(timeout()),this,SLOT(desactivatePreviousButtonAutomaticSimulation()));
    care.setNewClickPrevious(1);

}

// Arrêt de la simulation
void MainWindow::arreterSimulation() {

    // Mise à jour de l'utilisation possible ou non des boutons de l'interface
    openModeleButton->setEnabled(true);
    saveModeleButton->setEnabled(true);
    neighborhoodButton->setEnabled(true);
    ruleButton->setEnabled(true);
    gridWidthButton->setEnabled(true);
    gridHeightButton->setEnabled(true);
    gridRandomButton->setEnabled(true);
    configurationSaveButton->setEnabled(true);
    configurationLoadButton->setEnabled(true);
    configurationInitButton->setEnabled(true);
    if (care.getnbprevious() != 0 && !(care.getnbprevious() == 1 && care.getNewClickPrevious()==1))
        previousStateButton->setEnabled(true);
    else
        previousStateButton->setEnabled(false);
    playButton->setEnabled(true);
    pauseButton->setEnabled(false);
    nextStateButton->setEnabled(true);
    resetButton->setEnabled(true);
    gridWidthSpinBox->setEnabled(true);
    gridHeightSpinBox->setEnabled(true);
    etatButton->setEnabled(true);
    simu->arret();

}

// Réinitialisation de la simulation et retour à la configuration initiale
void MainWindow::resetConfiguration() {
    //le cas d'un vide du careTaker et d'un appel à réinitialiser qui vient en dépiler 2 alors qu'il y en a qu'un
    if (care.getnbprevious()==0)
        saveSimuConfiguration();
    res.resetToInit();
    res.actualiserGrille(1);
    care.setNewClickPrevious(1);
    if (care.getnbprevious() != 0)
        previousStateButton->setEnabled(true);
    else
        previousStateButton->setEnabled(false);
    resetButton->setEnabled(false);

}

// Changement de la vitesse de la simulation
void MainWindow::setSpeed() {

    simu->setSpeed(speedSpinBox->value());

}

// Chargement d'un modèle
void MainWindow::loadModele() {

    // Ouverture de la fenêtre pour choisir le fichier xml
    QString fileName = QFileDialog::getOpenFileName(this, tr("Ouvrir un modèle"), "../projet_lo21/modeles", tr("XML files (*.xml)"));

    if(fileName.isEmpty()) return;

    // Récupération du nom du modèle
    QFileInfo info(fileName);
    QString modeleName = info.baseName();

    if(modeleName.toStdString() == modele) {
        bool ok;
        QStringList items;
        items << "Oui" << "Non";
        QString item = QInputDialog::getItem(this, tr("Choix de la règle de transition"),tr("Le modèle que vous souhaitez charger à la même nom que le modèle courant, voulez vous tout de même le charger ? (cela supprime la grille)"), items, 0, false, &ok);
        if(!ok) return;
        if(item == "Oui") modele = "";
    }

    // Si le nom du modèle est différent de celui en cours, alors on modifie le modèle
    if(modeleName.toStdString() != modele) {

        // Ouverture du fichier et création d'un document qui contiendra les infos du fichier
        QDomDocument xml;
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox msgBox(this);
            msgBox.setText("Erreur pour ouvrir le fichier en lecture." );
            msgBox.exec();
            return;
        }
        if (!xml.setContent(&file)) {
            QMessageBox msgBox(this);
            msgBox.setText("Erreur pour parser le fichier en un arbre DOM.");
            msgBox.exec();
            file.close();
            return;
        }
        file.close();

        // Récupération des noeuds puis des informations qui nous interesse : Regle et Vosinage
        QDomElement xmlElem = xml.documentElement();

        QDomNodeList regleListe = xmlElem.elementsByTagName("regle");
        QDomNodeList voisinageListe = xmlElem.elementsByTagName("voisinage");
        QDomNodeList etatListe = xmlElem.elementsByTagName("etat");

        QDomNode regleNoeud = regleListe.at(0);
        QDomNode voisinageNoeud = voisinageListe.at(0);

        std::string regle = regleNoeud.toElement().text().toStdString();
        std::string voisinage = voisinageNoeud.toElement().text().toStdString();

        // On vérifie que tout existe
        try {
            if(!AutomateFactory::getFactory().exist(regle)) throw FichierException("La règle n'est pas valide dans le fichier xml (aucune règle ne correspond dans l'application)");
            if(!VoisinageFactory::getFactory().exist(voisinage)) throw FichierException("Le Voisinage n'est pas valide dans le fichier xml (aucun voisinage ne correspond dans l'application)");
        }  catch (FichierException e) {
            e.getInfo();
            return;
        }

        // Si c'est bon on peut créer les voisinages et l'automate
        Voisinage* v = VoisinageFactory::getFactory().construct(voisinage);
        Automate* a = AutomateFactory::getFactory().construct(regle);
        a->setVoisinage(v);

        int nbEtat = a->getNbEtat();
        try {
            // On vérifie que le nombre d'etat necessaire correspond au nombre d'etat dans le XML
            if(nbEtat != etatListe.count()) throw(FichierException("Nombre d'etat incompatible avec la règle choisit."));
            // Gestion des paramètres du voisinage et des automates (on verifie également qu'il n'y a aps d'erreur)
            v->setParametersXML(voisinageNoeud);
            a->setParametersXML(regleNoeud);
        }  catch (FichierException e) {
            e.getInfo();
            delete a;
            return;
        }

        // Si ok, libéreation des états déjà créés, récupération et création des états nécessaires au modèle choisi
        EtatManager::libereManager();
        EtatManager& em = EtatManager::getManager();
        if(etatListe.count() > 8)
            em.setNbMaxEtats(etatListe.count()); // Si le nombre d'états est supérieur à 8, il faut autoriser plus que 8 états max
        for(int i = 0; i < etatListe.count(); i++) {
            QDomNode etatNoeud = etatListe.at(i);
            QDomElement e = etatNoeud.toElement();
            QDomAttr a = e.attributeNode("label");
            std::string label = a.value().toStdString();
            a = e.attributeNode("couleur");
            QString colorstr = a.value();
            QColor color(colorstr);
            em.newEtat(label, color);
        }



        // Mise à jour des informations du modèle chosit qui devient le modèle courant
        modele = modeleName.toStdString();
        rule = regleNoeud.toElement().text();
        neighborhood = voisinageNoeud.toElement().text();

        // Suppression du simulateur si il en existait un et création d'un nouveau avec l'automate créé précédemment
        unsigned int speed = speedSpinBox->value();
        delete simu;
        simu = new Simulateur(a);
        simu->setGrille(&res);
        simu->setParent(this);
        simu->setSpeed(speed);

        // GESTION DU RESEAU : on met les pointeurs des cellules sur le premier état et on actualise la grille ainsi qu'on supprime les états précédents
        for(unsigned int i = 0; i < res.getHeight(); i++) {
           for(unsigned int j = 0; j < res.getWidth(); j++)
                res.setCellule(i, j, em.getEtat(0));
        }
        res.actualiserGrille(0);
        care.emptyPreviousConfigurations();

        // Mise à jour de l'utilisation possible ou non des boutons de l'interface
        gridWidthSpinBox->setEnabled(true);
        gridWidthButton->setEnabled(true);
        gridHeightSpinBox->setEnabled(true);
        gridHeightButton->setEnabled(true);
        saveModeleButton->setEnabled(true);
        configurationSaveButton->setEnabled(true);
        previousStateButton->setEnabled(false);
        playButton->setEnabled(false);
        nextStateButton->setEnabled(false);
        resetButton->setEnabled(false);
        pauseButton->setEnabled(false);
        configurationLoadButton->setEnabled(true);
        neighborhoodButton->setEnabled(true);
        ruleButton->setEnabled(true);
        etatButton->setEnabled(true);
        gridRandomButton->setEnabled(true);
        configurationInitButton->setEnabled(true);
        res.getGridTable()->setEnabled(true);
    }

}

// Chargement d'une configuration
void MainWindow::loadConfiguration() {

    unsigned int width = 0;
    unsigned int height = 0;

    // Ouverture de la fenêtre pour choisir le fichier xml
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Choisir une configuration"), "../projet_lo21/configurations", tr("XML files (*.xml)"));

    if(fileName.isEmpty()) return;

    // Récupération du fichier de configuration
    QFile file(fileName);

    // On l'ouvre et on stocke son contenu dans un document
    if(!file.open( QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox msgBox(this);
        msgBox.setText("Erreur pour ouvrir le fichier en lecture." );
        msgBox.exec();
        return;
    }

    QDomDocument xml;

    if(!xml.setContent(&file)) {
        QMessageBox msgBox(this);
        msgBox.setText("Erreur pour parser le fichier en un arbre DOM.");
        msgBox.exec();
        file.close();
        return;
    }
    file.close();

    // Récupération des noeuds puis des informations qui nous intéressent : width, heigth, états des cellules
    QDomElement xmlElem = xml.documentElement();
    QDomNodeList confList = xmlElem.elementsByTagName("Configuration");
    QDomNode confNoeud = confList.at(0);
    QDomElement conf = confNoeud.toElement();
    QDomAttr model = conf.attributeNode("modele");
    QDomNodeList widthList = xmlElem.elementsByTagName("width");
    QDomNode widthNoeud = widthList.at(0);
    width = widthNoeud.toElement().text().toUInt();
    QDomNodeList heightList = xmlElem.elementsByTagName("height");
    QDomNode heightNoeud = heightList.at(0);
    height = heightNoeud.toElement().text().toUInt();

    // Si le modèle de la configuration ne correspond pas au modèle courant alors on le dit à l'utilisateur et on arrête
    if(model.value().toStdString() != modele) {
        QMessageBox msgBox(this);
        msgBox.setText("La configuration choisit n'est pas compatible avec le modèle en courant !");
        msgBox.exec();
        return;
    }

    // On récupère ensuite les informations des états des celulles de la grille
    EtatManager& em = EtatManager::getManager();
    int nbEtat = static_cast<int>(em.getNbEtats());

    // On va d'abord vérifier qu'il n'y a pas de soucis dans le fichier
    try {
        QDomNodeList lineList = xmlElem.elementsByTagName("line");
        if(lineList.count() != static_cast<int>(height)) throw(FichierException("Mauvaise taille dans le fichier xml (hauteur)"));
        for(int j=0; j< lineList.count(); j++){
            QDomNode lineNoeud = lineList.at(j);
            QDomElement lineElement = lineNoeud.toElement();
            QDomNodeList columnList = lineElement.elementsByTagName("column");
            if(columnList.count() != static_cast<int>(width)) throw(FichierException("Mauvaise taille dans le fichier xml (largeur)"));
            for(int k=0; k<columnList.count(); k++){
                QDomNode columnNoeud = columnList.at(k);
                if(columnNoeud.toElement().text().isEmpty()) throw(FichierException("Cellule n'ayant aucun etat dans le fichier xml"));
                if(columnNoeud.toElement().text().toInt() < 0 || columnNoeud.toElement().text().toInt() >= nbEtat) throw(FichierException("Etat qui n'existe pas dans le modèle présent dans le fichier xml"));
            }
        }
    } catch(FichierException e) {
        e.getInfo();
        return;
    }

    // Si tout est bon on met à jour ces informations dans la grille
    res.setHeight(height);
    res.setWidth(width);
    res.drawGrid(gridLayout);

    // Si tout est bon on récupère les etats des celulles et on les mets dans la grille
    QDomNodeList lineList = xmlElem.elementsByTagName("line");
    for(int j=0; j< lineList.count(); j++){
        QDomNode lineNoeud = lineList.at(j);
        QDomElement lineElement = lineNoeud.toElement();
        QDomNodeList columnList = lineElement.elementsByTagName("column");
        for(int k=0; k<columnList.count(); k++) {
            QDomNode columnNoeud = columnList.at(k);
            // On met a jour chaque cellule de la grille avec l'etat correspondant dans la configuration
            res.setCellule(j,k,em.getEtat(columnNoeud.toElement().text().toInt()));
        }
    }

    // On actualise enfin la grille
    care.emptyPreviousConfigurations();
    res.actualiserGrille(1);

    // Mise à jour de l'utilisation possible ou non des boutons de l'interface
    openModeleButton->setEnabled(true);
    saveModeleButton->setEnabled(true);
    neighborhoodButton->setEnabled(true);
    ruleButton->setEnabled(true);
    gridWidthButton->setEnabled(true);
    gridHeightButton->setEnabled(true);
    gridRandomButton->setEnabled(true);
    configurationSaveButton->setEnabled(true);
    configurationLoadButton->setEnabled(true);
    previousStateButton->setEnabled(false);
    playButton->setEnabled(false);
    pauseButton->setEnabled(false);
    resetButton->setEnabled(false);
    nextStateButton->setEnabled(false);
    gridWidthSpinBox->setEnabled(true);
    gridHeightSpinBox->setEnabled(true);
    speedButton->setEnabled(true);
    speedSpinBox->setEnabled(true);
    res.getGridTable()->setEnabled(true);
    configurationInitButton->setEnabled(true);

}

// Création d'un nouveau modèle
void MainWindow::setCreateModele() {

    ruleButton->setEnabled(true);

}

// Choix de la fonction de transition
void MainWindow::setRegle(){

    // Récupération des règles de transition possibles, on a besoin de parcourir les automates disponibles dans la factory
    QStringList items;
    AutomateFactory& af = AutomateFactory::getFactory();
    for(auto it = af.cbegin(); it!= af.cend(); it++ ){
       QString regle = QString::fromStdString((*it).first);
       regle.remove("Automate");
       regle.push_front("Règle de ");
       items << regle;
    }

    // Mise en place d'une boite de dialogue qui affiche les règles de transitions que l'utilisateur pourra choisir
    bool ok;
    if(items.isEmpty()) return;
    QString item = QInputDialog::getItem(this, tr("Choix de la règle de transition"),tr("Choix de la règle de transition :"), items, 0, false, &ok);
    if(!ok) return;
    item.remove("Règle de ");
    item.push_front("Automate");

    // Si l'utilisateur a choisit un modèle différent de celui en cours, alors on change de modèle
    if(ok && item != rule) {
           rule = item;
           modele = "";

           // On copie le voisinage existant s'l en existait un
           Voisinage *v = nullptr;
           if(simu != nullptr && simu->getAutomate() != nullptr && simu->getAutomate()->getVoisinage() != nullptr ){
               v = simu->getAutomate()->getVoisinage()->getCopyDynamic();
           }

           // Gestion des boutons, si on change de modèle, il faut interdire certaines fonctionnalités
           playButton->setEnabled(false);
           nextStateButton->setEnabled(false);
           resetButton->setEnabled(false);
           pauseButton->setEnabled(false);
           previousStateButton->setEnabled(false);
           configurationInitButton->setEnabled(false);
           gridRandomButton->setEnabled(false);
           gridWidthSpinBox->setEnabled(false);
           gridWidthButton->setEnabled(false);
           gridHeightSpinBox->setEnabled(false);
           gridHeightButton->setEnabled(false);
           res.getGridTable()->setEnabled(false);

           // On crée l'automate, ou lui affecte le voisinage s'l en existait un et on recrée un simulateur avec ce nouvel automate
           Automate *a =  af.construct(item.toStdString());
           unsigned int speed = speedSpinBox->value();
           a->setVoisinage(v);
           delete simu;
           simu = new Simulateur(a);
           simu->setGrille(&res);
           simu->setParent(this);
           simu->setSpeed(speed);

           // GESTION DES ETATS : on supprime les états qui existaient et on crée autant d'états que nécessaire dans la règle de transition choisie
           EtatManager::libereManager();
           EtatManager& em = EtatManager::getManager();
           unsigned int nbEtat = a->getNbEtat();
           if(nbEtat > 8) em.setNbMaxEtats(nbEtat); // Si le nombre d'etats est supérieur à 8, il faut autoriser plus que 8 états max
           for(unsigned int i =0; i< nbEtat; i++) {
               if(i%2 == 0) em.newEtat("Rien",Qt::black);
               else em.newEtat("Rien",Qt::white);
           }

           // GESTION DU RESEAU : on met les pointeurs des cellules sur le premier état et on actualise la grille ainsi qu'on supprime les états précédents
           for(unsigned int i = 0; i < res.getHeight(); i++) {
              for(unsigned int j = 0; j < res.getWidth(); j++)
                   res.setCellule(i, j, em.getEtat(0));
           }
           res.actualiserGrille(0);
           care.emptyPreviousConfigurations();

           // GESTION DES PARAMETRES DU VOISINAGES
           a->setParameters();

           // Une fois un modèle choisi, on peut configurer ou choisir un voisinage (et si déjà un vosinage configurer les états)
           neighborhoodButton->setEnabled(true);
           if(a->getVoisinage() != nullptr){
               configurationInitButton->setEnabled(true);
               gridRandomButton->setEnabled(true);
               gridWidthSpinBox->setEnabled(true);
               gridWidthButton->setEnabled(true);
               gridHeightSpinBox->setEnabled(true);
               gridHeightButton->setEnabled(true);
               etatButton->setEnabled(true);
               res.getGridTable()->setEnabled(true);
               saveModeleButton->setEnabled(true);
               configurationSaveButton->setEnabled(true);
           }

    }

    // Peut-être que l'utilisateur veut changer de paramètre s'il choisit la même règle
    if(simu != nullptr && simu->getAutomate() != nullptr) {
        bool ok = simu->getAutomate()->setParameters();
        if(ok) modele = "";
    }

}

// Choix du voisinage
void MainWindow::setVoisinage() {

    // Récupération des voisinages possibles, on a besoin de parcourir les voisinages disponibles dans la factory
    QStringList items;
    VoisinageFactory& vf = VoisinageFactory::getFactory();
    for(auto it = vf.cbegin(); it!= vf.cend(); it++ ){
       QString voisinage = QString::fromStdString((*it).first);
       voisinage.insert(9," ");
       items << voisinage;
    }

    // Mise en place d'une boite de dialogue qui affiche les voisinages que l'utilisateur pourra choisir
    bool ok;
    if(items.isEmpty()) return;
    QString item = QInputDialog::getItem(this, tr("Choix du voisinage"),tr("Choix du voisinage :"), items, 0, false, &ok);

    if(!ok) return;
    item.remove(" ");

    configurationInitButton->setEnabled(true);
    gridRandomButton->setEnabled(true);
    gridWidthSpinBox->setEnabled(true);
    gridWidthButton->setEnabled(true);
    gridHeightSpinBox->setEnabled(true);
    gridHeightButton->setEnabled(true);
    res.getGridTable()->setEnabled(true);

    // Si l'utilisateur a choisi un voisinage différent de celui en cours alors on change de modèle
    if(ok && item != neighborhood) {
            neighborhood = item;
            modele = "";

           // Gestion des boutons
           saveModeleButton->setEnabled(true);
           configurationSaveButton->setEnabled(true);
           configurationLoadButton->setEnabled(true);
           etatButton->setEnabled(true);

            // Construction du voisinage choisi
           Voisinage *v = vf.construct(item.toStdString());

           // GESTION DES PARAMETRES DU VOISINAGES
           v->setParameters();

           simu->getAutomate()->setVoisinage(v);
           return;
   }

    //Peut-être que l'utilisateur veut changer de paramètre s'il choisit le même voisinage
    if(simu != nullptr && simu->getAutomate() != nullptr && simu->getAutomate()->getVoisinage() != nullptr ) {
        bool ok = const_cast<Voisinage*>(simu->getAutomate()->getVoisinage())->setParameters();
        if(ok) modele = "";
    }

}

// Choix des états
void MainWindow::setEtat(){

    // Recupération du Manager d'états et du nombre d'états
    EtatManager& em = EtatManager::getManager();
    unsigned int nbEtat = em.getNbEtats();

    // Création d'une liste des états que l'utilisateur pourra choisir
    QStringList items;
    for(unsigned int i =0; i<nbEtat; i++) {
        QString nom = "Etat " + QString::number(i+1) + "; Label : " + QString::fromStdString(em.getEtat(i)->getLabel()) + "; Couleur : " + em.getEtat(i)->getCouleur().name();
        items << nom;
    }

    // Affichage de la boîte de dialog à l'utilisateur pour le choix de l'état à modifier
    bool ok;
    if(items.isEmpty()) return;
    QString item = QInputDialog::getItem(this, tr("Modification d'un etat"),tr("Choisissez l'etat à modifier :"), items, 0, false, &ok);

    // Récupération du numéro de l'état choisi
    if(!ok) return;
    item = item[5];
    unsigned int numero = item.toUInt()-1;

    // Création d'une boite de dialogue sur le choix de la modification à réaliser
    items.clear();
    items << "Changez la couleur" << "Changez le label";
    item = QInputDialog::getItem(this, tr("Choix de modification"),tr("Choisissez la modification à effectuer :"), items, 0, false, &ok);

    if(!ok) return;

    // Si l'utilisateur veut changer la couleur : création d'une boite de dialogue pour choisir la couleur puis modification de la couleur et modèle modifié si choix de couleur
    if(item == "Changez la couleur") {
        QColor couleur = QColorDialog::getColor(em.getEtat(numero)->getCouleur(), this, tr("Choisissez la couleur"));
        if(!couleur.isValid()) return;
        modele = "";
        em.getEtat(numero)->setCouleur(couleur);
        res.actualiserGrille(0);
    }

    // Si l'utilisateur veut changer de label : création d'une boite de dialogue pour entrer le label puis modification du label et modèle modifié
    else if(item == "Changez le label") {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Choix du label"),tr("Choissiez votre label :"), QLineEdit::Normal, QString::fromStdString(em.getEtat(numero)->getLabel()), &ok);
        if(!ok) return;
        modele = "";
        em.getEtat(numero)->setLabel(text.toStdString());
    }

}

void MainWindow::saveConfiguration(){

    if(modele.empty()){
        unsigned int i = saveModele();
        if(i == 1) return;
    }

    Reseau::Memento memento = res.save();

    string title;
    QString defaultTexte = "votretitre";

    bool ok;
    QString text = QInputDialog::getText(this, tr("Saisir le titre de la configuration"),tr("Titre : "), QLineEdit::Normal, defaultTexte, &ok);
    if(!ok) return;
    if (ok && !text.isEmpty())
        title = text.toStdString();

    care.addMemento(memento,title, modele);

}

void MainWindow::saveSimuConfiguration(){

    Reseau::Memento memento =res.save();
    Reseau::Memento * ptr_mem = new Reseau::Memento(memento);

    care.addMementoPrevious(ptr_mem);

}

void MainWindow::desactivatePreviousButtonAutomaticSimulation(){

    previousStateButton->setEnabled(false);

}

void MainWindow::setNbBackup(){

    care.setNbBackups(previousNumberSpinBox->value());
    if (care.getnbprevious() != 0)
        previousStateButton->setEnabled(true);
    else
        previousStateButton->setEnabled(false);

}

int MainWindow::saveModele(){

    QString name;
    QString NameDefaultTexte = "Nom du modèle obligatoire et sans espace";
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enregistrer le modèle courant"),tr("Nom du modèle (obligatoire !) :"), QLineEdit::Normal, NameDefaultTexte, &ok);
    if (ok && !text.isEmpty())
        name = text;
    if(!ok) return 1;

    QString description = "";
    QString descriptionDefaultTexte = "";
    text = QInputDialog::getText(this, tr("Enregistrer le modèle courant"),tr("Description :"), QLineEdit::Normal, descriptionDefaultTexte, &ok);
    if (ok && !text.isEmpty())
        description = text;
    if(!ok) return 1;

    QString author = "";
    QString authorDefaultTexte = "";
    text = QInputDialog::getText(this, tr("Enregistrer le modèle courant"),tr("Auteur (optionnel) :"), QLineEdit::Normal, authorDefaultTexte, &ok);
    if (ok && !text.isEmpty())
        author = text;
    if(!ok) return 1;

    QString date = "";
    QString dateDefaultTexte = "";
    text = QInputDialog::getText(this, tr("Enregistrer le modèle courant"),tr("Date (optionnel) :"), QLineEdit::Normal, dateDefaultTexte, &ok);
    if (ok && !text.isEmpty())
        date = text;
    if(!ok) return 1;

    unordered_map<string, int> neighborhoodParameters = simu->getAutomate()->getVoisinage()->getParameters();
    unordered_map<string, int> rulesParameters = simu->getAutomate()->getParameters();

    saveModelDialog* dialog = new saveModelDialog(rule, rulesParameters, neighborhood, neighborhoodParameters, name, description, author, date);

    if (dialog->exec() == QDialog::Accepted){

        QDomDocument xml;

        xml.appendChild(xml.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"") );
        QDomElement tagRoot = xml.createElement("root");

        QDomElement tagDescription = xml.createElement("description");
        QDomText textDescription = xml.createTextNode(description);
        tagDescription.appendChild(textDescription);
        tagRoot.appendChild(tagDescription);

        QDomElement tagAuthor = xml.createElement("auteur");
        QDomText textAuthor = xml.createTextNode(author);
        tagAuthor.appendChild(textAuthor);
        tagRoot.appendChild(tagAuthor);

        QDomElement tagDate = xml.createElement("dateCréation");
        QDomText textDate = xml.createTextNode(date);
        tagDate.appendChild(textDate);
        tagRoot.appendChild(tagDate);

        QDomElement modeleElement = xml.createElement("modele");

        EtatManager& em = EtatManager::getManager();
        unsigned int nbEtat = em.getNbEtats();
        for(unsigned int i =0; i<nbEtat; i++) {
            QDomElement tagEtat = xml.createElement("etat");
            tagEtat.setAttribute("label", QString::fromStdString(em.getEtat(i)->getLabel()));
            tagEtat.setAttribute("couleur", em.getEtat(i)->getCouleur().name());
            modeleElement.appendChild(tagEtat);
        }

        QDomElement tagNeighborhood = xml.createElement("voisinage");
        unordered_map<string, int>::const_iterator itNeighborhood = neighborhoodParameters.cbegin();
        for (; itNeighborhood != neighborhoodParameters.cend(); itNeighborhood++){
            string label = itNeighborhood->first;
            int value = itNeighborhood->second;
            tagNeighborhood.setAttribute(label.c_str(), value);
        }
        QDomText textNeighborhood = xml.createTextNode(neighborhood);
        tagNeighborhood.appendChild(textNeighborhood);
        modeleElement.appendChild(tagNeighborhood);

        QDomElement tagRules = xml.createElement("regle");
        unordered_map<string, int>::const_iterator itRules = rulesParameters.cbegin();
        for (; itRules != rulesParameters.cend(); itRules++){
            string label = itRules->first;
            int value = itRules->second;
            tagRules.setAttribute(label.c_str(), value);
        }
        QDomText textRules = xml.createTextNode(rule);
        tagRules.appendChild(textRules);
        modeleElement.appendChild(tagRules);

        tagRoot.appendChild(modeleElement);

        xml.appendChild(tagRoot);

        QString fileName = QFileDialog::getSaveFileName(this, tr("Enregistrer le modèle courant"), "../projet_lo21/modeles/"+name+".xml", tr("Nom du modèle(*.xml)"));

        if(fileName.isEmpty()) return 1;

        // Récupération du fichier de configuration
        QFile file(fileName);

        if( !file.open( QIODevice::WriteOnly | QIODevice::Text))
        qDebug("Erreur pour ouvrir le fichier en écriture.");
        QTextStream stream(&file);
        stream << xml.toString();
        file.close();

        modele = name.toStdString();
        return 0;
    }
    else return 1;

}

saveModelDialog::saveModelDialog(const QString &rules, const unordered_map<string, int> &rulesParameters, const QString &neighborhood, const unordered_map<string, int> &neighborhoodParameters, const QString &name, const QString &description, const QString &author, const QString &date, QWidget *parent):QDialog(parent){

    QLabel *nameLabel = new QLabel(tr("Nom :"));
    QLabel *nameEdit = new QLabel(name);
    nameEdit->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

    QLabel *descriptionLabel = new QLabel(tr("Description :"));
    QLabel *descriptionEdit = new QLabel(description);
    descriptionEdit->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

    QLabel *authorLabel = new QLabel(tr("Auteur :"));
    QLabel *authorEdit = new QLabel(author);
    authorEdit->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

    QLabel *dateLabel = new QLabel(tr("Date :"));
    QLabel *dateEdit = new QLabel(date);
    dateEdit->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

    QLabel *modelLabel = new QLabel(tr("Le nouveau modèle :"));
    tabWidget = new QTabWidget;
    tabWidget->addTab(new rulesTab(rules, rulesParameters), tr("Règles"));
    tabWidget->addTab(new neighborhoodTab(neighborhood, neighborhoodParameters), tr("Voisinage"));
    tabWidget->addTab(new etatsTab(), tr("Etats"));

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *infoLayout = new QVBoxLayout;
    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(nameEdit);
    infoLayout->addWidget(descriptionLabel);
    infoLayout->addWidget(descriptionEdit);
    infoLayout->addWidget(authorLabel);
    infoLayout->addWidget(authorEdit);
    infoLayout->addWidget(dateLabel);
    infoLayout->addWidget(dateEdit);

    QVBoxLayout *tabLayout = new QVBoxLayout;
    tabLayout->addWidget(modelLabel);
    tabLayout->addWidget(tabWidget);

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(buttonBox);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(infoLayout);
    mainLayout->addLayout(tabLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    setWindowTitle(tr("Composantes du nouveau modèle"));

}

rulesTab::rulesTab(const QString &rules, const unordered_map<string, int> &rulesParameters, QWidget *parent):QWidget(parent) {

    QLabel *rulesLabel = new QLabel(tr("Règle :"));
    QLabel *rulesValueLabel = new QLabel(rules);
    rulesValueLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(rulesLabel);
    mainLayout->addWidget(rulesValueLabel);

    unordered_map<string, int>::const_iterator it = rulesParameters.cbegin();
    for (;it != rulesParameters.cend(); it++) {
        string label = it->first;
        int value = it->second;
        QLabel *rulesParameterLabel = new QLabel(tr(label.c_str()));
        QLabel *rulesParameterValueLabel = new QLabel(QString::number((value)));
        rulesValueLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
        mainLayout->addWidget(rulesParameterLabel);
        mainLayout->addWidget(rulesParameterValueLabel);
    }

    setLayout(mainLayout);

}

neighborhoodTab::neighborhoodTab(const QString &neighborhood, const unordered_map<string, int>& neighborhoodParameters, QWidget *parent):QWidget(parent){

    QLabel *neighborhoodLabel = new QLabel(tr("Voisinage :"));
    QLabel *neighborhoodValueLabel = new QLabel(neighborhood);
    neighborhoodValueLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(neighborhoodLabel);
    mainLayout->addWidget(neighborhoodValueLabel);

    unordered_map<string, int>::const_iterator it = neighborhoodParameters.cbegin();
    for (; it != neighborhoodParameters.cend(); it++){
        string label = it->first;
        int value = it->second;
        QLabel *neighborhoodParameterLabel = new QLabel(tr(label.c_str()));
        QLabel *neighborhoodParameterValueLabel = new QLabel(QString::number((value)));
        neighborhoodValueLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
        mainLayout->addWidget(neighborhoodParameterLabel);
        mainLayout->addWidget(neighborhoodParameterValueLabel);
    }

    setLayout(mainLayout);

}

etatsTab::etatsTab(QWidget *parent):QWidget(parent){

    QLabel *etatsLabel = new QLabel(tr("Etats :"));

    EtatManager& em = EtatManager::getManager();
    unsigned int nbEtat = em.getNbEtats();

    QListWidget *listBox = new QListWidget;
    QStringList items;
    for(unsigned int i =0; i<nbEtat; i++){
        QString nom = "Etat " + QString::number(i+1) + "; Label : " + QString::fromStdString(em.getEtat(i)->getLabel()) + "; Couleur : " + em.getEtat(i)->getCouleur().name();
        items << nom;
    }

    listBox->insertItems(0, items);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(etatsLabel);
    mainLayout->addWidget(listBox);
    setLayout(mainLayout);

}
