#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <string>
#include <QMessageBox>

/*!
* \class FichierException
* \brief Classe permettant de gérer les exceptions rencontrées aux différents moments sensibles de l'application.
*/
class FichierException {

    std::string info;

public:

    /*!
    * \brief Constructeur de la classe FichierException.
    * \param message Message renvoyé par l'exception.
    */
    FichierException(const std::string& message) : info(message) {}

    /*!
    * \brief Affiche sur l'interface graphique un message pour l'utilisateur qui explicite le problème rencontré.
    */
    void getInfo() const {
        QMessageBox msgBox;
        msgBox.setText(QString::fromStdString(info));
        msgBox.exec();
        return;
    }

};

#endif // EXCEPTION_H
