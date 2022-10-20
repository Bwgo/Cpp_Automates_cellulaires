#ifndef FACTORY_H
#define FACTORY_H
#include <map>
#include <string>
#include "automate.h"

template<class T> Automate* constructorAutomate() { return new T(); }

/*!
* \class AutomateFactory
* \brief Classe permettant de créer toutes les sortes d'automates définies dans le programme (main.cpp) en ayant uniquement le nom de l'automate en string. Classe singleton.
*/
class AutomateFactory {

   typedef Automate*(*constructor_t)();
   std::map<std::string, constructor_t> listAutomate;
   AutomateFactory() = default;
   ~AutomateFactory() = default;
   AutomateFactory(const AutomateFactory& a) = delete;
   AutomateFactory& operator=(const AutomateFactory& a) = delete;

public:

   /*!
   * \brief Retourne l'instance de la classe AutomateFactory.
   */
   static AutomateFactory& getFactory() {
       static AutomateFactory a;
       return a;
   }

   /*!
   * \brief Permet d'ajouter à un map une nouvelle paire contenant le nom de l'automate en format string et un pointeur sur le constructeur de celui-ci.
   * \param n Nom de l'automate (ne pas oublier de rajouter le classe automate voulu entre <> en appelant la méthode).
   */
   template <class T>
   void addClass(const std::string n) { listAutomate.insert(std::make_pair(n, &constructorAutomate<T>)); }

   /*!
   * \brief Contruit un nouvel objet de la classe du nom donné en paramètre (s'il se trouve bien dans le map) et retourne un pointeur sur celu- ci, sinon retourne un pointeur null.
   * \param n Nom de la classe dont on veut créer un nouvel objet.
   */
   Automate* construct(std::string const& n) {
      if(!exist(n)) return nullptr;
      map<std::string, constructor_t>::iterator it = listAutomate.find(n);
      return it->second();
   }
   
   /*!
   * \brief Retourne un const iterator sur le debut de la liste d'automates de la factory.
   */
   std::map<std::string, constructor_t>::const_iterator cbegin() { return listAutomate.cbegin(); }

   /*!
   * \brief Retourne un const iterator sur la fin de la liste d'automate de la factory.
   */
   std::map<std::string, constructor_t>::const_iterator cend() { return listAutomate.cend(); }

   /*!
   * \brief Verifie si un nom de clase existe dans la factory.
   * \param str Nom de la classe dont on souhaite vérifier l'existance.
   */
   bool exist(std::string str) {
       map<std::string, constructor_t>::iterator it = listAutomate.find(str);
       return (it != listAutomate.end());
   }

};

template<class T> Voisinage* constructorVoisinage() { return new T(); }

/*!
* \class VoisinageFactory
* \brief Classe permettant de créer toutes les sortes de voisinages définis dans le programme (main.cpp) en ayant uniquement le nom du voisinage en string. Classe singleton.
*/
class VoisinageFactory {

   typedef Voisinage*(*constructor_t)();
   std::map<std::string, constructor_t> listVoisinage; // {"VoisinageMoore" : *VoisinageMoore}
   VoisinageFactory() = default;
   ~VoisinageFactory() = default;
   VoisinageFactory(const VoisinageFactory& a) = delete;
   VoisinageFactory& operator=(const VoisinageFactory& a) = delete;

public:

   /*!
   * \brief Retourne l'instance de la classe VoisinageFactory.
   */
    static VoisinageFactory& getFactory() {
        static VoisinageFactory v;
        return v;
    }

    /*!
    * \brief Permet d'ajouter à un map une nouvelle paire contenant le nom du voisinage en format string et un pointeur sur le constructeur de celui-ci.
    * \param n Nom du voisinage (ne pas oublier de rajouter le classe voisinage voulu entre <> en appelant la méthode).
    */
   template <class T>
   void addClass(const std::string n) { listVoisinage.insert(std::make_pair(n, &constructorVoisinage<T>)); }

   /*!
   * \brief Contruit un nouvel objet de la classe du nom donné en paramètre (s'il se trouve bien dans le map) et retourne un pointeur sur celui-ci, sinon retourne un pointeur null.
   * \param n Nom de la classe dont on veut créer un nouvel objet
   */
   Voisinage* construct(std::string const& n) {
      if(!exist(n)) return nullptr;
      map<std::string, constructor_t>::iterator it = listVoisinage.find(n);
      return it->second();
   }
   
   /*!
   * \brief Retourne un const iterator sur le debut de la liste de voisinage de la factory.
   */
   std::map<std::string, constructor_t>::const_iterator cbegin() { return listVoisinage.cbegin(); }

   /*!
   * \brief Retourne un const iterator sur la fin de la liste de voisinage de la factory.
   */
   std::map<std::string, constructor_t>::const_iterator cend() { return listVoisinage.cend(); }

   /*!
   * \brief Verifie si un nom de clase existe dans la factory.
   * \param str Nom de la classe dont on souhaite vérifier l'existance.
   */
   bool exist(std::string str) {
       map<std::string, constructor_t>::iterator it = listVoisinage.find(str);
       return (it != listVoisinage.end());
   }

};

#endif // FACTORY_H
