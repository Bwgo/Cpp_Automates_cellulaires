#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "simulateur.h"
#include "caretaker.h"
#include "exception.h"
#include <QMainWindow>
#include <QWidget>
#include <QTableWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QFileDialog>

class QAction;

class QMenu;

/*!
* \class MainWindow
* \brief Classe contenant les éléments d'IHM de l'application.
*/
class MainWindow : public QMainWindow {

    Q_OBJECT

    QMenuBar* menuBar;

    QMenu* fileMenu;
    QMenu* infoMenu;

    QAction* exitAction;
    QAction* aboutAction;

    QGroupBox* modeleGroupBox;
    QGroupBox* modelParameterGroupBox;
    QGroupBox* gridParameterGroupBox;
    QGroupBox* configurationGroupBox;
    QGroupBox* simulationGroupBox;
    QGroupBox* controlGroupBox;

    QPushButton* newModeleButton;
    QPushButton* openModeleButton;
    QPushButton* saveModeleButton;
    QPushButton* neighborhoodButton;
    QPushButton* ruleButton;
    QPushButton* etatButton;
    QPushButton* gridWidthButton;
    QPushButton* gridHeightButton;
    QPushButton* gridRandomButton;
    QPushButton* configurationSaveButton;
    QPushButton* configurationLoadButton;
    QPushButton* configurationInitButton;
    QPushButton* previousStateButton;
    QPushButton* playButton;
    QPushButton* pauseButton;
    QPushButton* nextStateButton;
    QPushButton* resetButton;
    QPushButton* speedButton;
    QPushButton* previousNumberButton;

    QSpinBox* gridWidthSpinBox;
    QSpinBox* gridHeightSpinBox;
    QSpinBox* speedSpinBox;
    QSpinBox* previousNumberSpinBox;

    QVBoxLayout* gridLayout;

    void createMenu();
    void createModelGroupBox();
    void createModelParameterGroupBox();
    void createGridParameterGroupBox();
    void createConfigurationGroupBox();
    void createSimulationGroupBox();
    void createControlGroupBox();
    void createGridGroupBox();

    Reseau res = Reseau(20,20);

    CareTaker care = CareTaker(10);

    Simulateur* simu = nullptr;

    std::string modele;
    QString rule = "";
    QString neighborhood = "";

public:

    /*!
    * \brief Constructeur de la classe MainWindow.
    */
    MainWindow(QWidget *parent = nullptr);

    /*!
    * \brief Destructeur de la classe MainWindow.
    */
    ~MainWindow();

private slots:

    /*!
    * \brief Accède à des informations sur l'application.
    */
    void about();

    /*!
    * \brief Définit la largeur.
    */
    void setGridWidth();

    /*!
    * \brief Définit la hauteur de la grille.
    */
    void setGridHeight();

    /*!
    * \brief Génère une configuration aléatoirement.
    */
    void setRandomGrid();

    /*!
    * \brief Règle la vitesse de déroulement de la simulation.
    */
    void setSpeed();

    /*!
    * \brief Avance d'un pas dans la simulation.
    */
    void pasSuivant();

    /*!
    * \brief Revient un pas en arrière dans la simulation.
    */
    void pasPrecedent();

    /*!
    * \brief Permet de choisir la fonction de transition.
    */
    void setRegle();

    /*!
    * \brief Permet de choisir un voisinage.
    */
    void setVoisinage();

    /*!
    * \brief Permet choisir les états.
    */
    void setEtat();

    /*!
    * \brief Permet la création d'un modèle.
    */
    void setCreateModele();

    /*!
    * \brief Met à jour le nombre de configurations précédentes que l'on peut sauvegarder dans l'historique.
    */
    void setNbBackup();

    /*!
    * \brief Sauvegarde le modèle. Retourne 1 si succès, 0 sinon.
    */
    int saveModele();

    /*!
    * \brief Réinitialise la configuration initiale.
    */
    void resetConfiguration();

    /*!
    * \brief Définit la configuration initiale.
    */
    void setConfigurationInit();

    /*!
    * \brief Lance la simulation sur le modele et la configuration en cours.
    */
    void lancerSimulation();

    /*!
    * \brief Arrête la simulation.
    */
    void arreterSimulation();

    /*!
    * \brief Charge une configuration via un fichier xml.
    */
    void loadConfiguration();

    /*!
    * \brief Charge un modèle via un fichier xml.
    */
    void loadModele();

    /*!
    * \brief Sauvegarde une configuration dans un fichier xml.
    */
    void saveConfiguration();

    /*!
    * \brief Sauvegarde la configuration actuelle à chaque pas dans un fichier xml.
    */
    void saveSimuConfiguration();

    /*!
    * \brief Désactive le bouton précédent lors de l'exécution de la simulation en mode automatique.
    */
    void desactivatePreviousButtonAutomaticSimulation();

};

/*!
* \class saveModelDialog
* \brief Classe permettant de créer une fenêtre de dialogue personnalisée pour récapituler la saisie des informations lors de la sauvegarde d'un modèle.
*/
class saveModelDialog : public QDialog {

    Q_OBJECT
    QTabWidget *tabWidget;
    QDialogButtonBox *buttonBox;

public:

    /*!
    * \brief Constructeur de la classe saveModelDialog.
    */
    explicit saveModelDialog(const QString &rules, const unordered_map<string, int>& rulesParameters, const QString &neighborhood, const unordered_map<string, int>& neighborhoodParameters, const QString &name, const QString &description, const QString &author, const QString &date, QWidget *parent = nullptr);

};

/*!
* \class rulesTab
* \brief Classe permettant de créer un onglet dans la fenêtre de dialogue personnalisée pour récapituler la saisie des informations concernant les régles de transition lors de la sauvegarde d'un modèle.
*/
class rulesTab : public QWidget
{
    Q_OBJECT

public:

    /*!
    * \brief Constructeur de la classe rulesTab.
    */
    explicit rulesTab(const QString &rules, const unordered_map<string, int>& rulesParameters, QWidget *parent = nullptr);

};

/*!
* \class neighborhoodTab
* \brief Classe permettant de créer un onglet dans la fenêtre de dialogue personnalisée pour récapituler la saisie des informations concernant le voisinage lors de la sauvegarde d'un modèle.
*/
class neighborhoodTab : public QWidget {

    Q_OBJECT

public:

    /*!
    * \brief Constructeur de la classe neighborhoodTab.
    */
    explicit neighborhoodTab(const QString &neighborhood, const unordered_map<string, int>& neighborhoodParameters, QWidget *parent = nullptr);

};

/*!
* \class etatsTab
* \brief Classe permettant de créer un onglet dans la fenêtre de dialogue personnalisée pour récapituler la saisie des informations concernant les états lors de la sauvegarde d'un modèle.
*/
class etatsTab : public QWidget {

    Q_OBJECT

public:

    /*!
    * \brief Constructeur de la classe etatsTab.
    */
    explicit etatsTab(QWidget *parent = nullptr);

};

#endif // MAINWINDOW_H
