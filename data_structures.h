// Contient toutes les structures de données necessaire au fonctionnement de l'autoamte

#ifndef DATA_STRUCTURES_HEADER
#define DATA_STRUCTURES_HEADER

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <exception>
#include <algorithm>

using namespace std;

typedef struct Transition {
    //Transition unique
    struct State *dest;
    char trans;

    /// Cherche une transition avec le caractère spécifié
    /// \param list liste de transition à vérifier
    /// \param c caractère à chercher
    /// \return première occurence, nullptr sinon
    static Transition *searchByCharacter(vector<Transition *> &list, char c);

    /// Retrouve toutes les occurences d'un carcactère dans une liste de transition
    /// \param list liste de transition à vérifier
    /// \param c caractère à chercher
    /// \param occurence_list liste d'occurence
    static void searchOccurrence(const vector<Transition *> &list, char c, vector<Transition *> &occurence_list);

    /// Trie les transition dans l'ordre alphabétique
    /// \param list liste de transition
    static void sortTransitions(vector<Transition *> &list);
} Transition;


typedef struct State {
    // Etat unique
    string id = "";
    bool initial = false;
    bool terminal = false;
    vector<Transition *> transition_list;

    /// Cherche un état avec un id
    /// \param list liste à vérifer
    /// \param id id à rechercher
    /// \return pointeur vers l'état, nullptr sinon
    static State *idSearch(string id, vector<State *> list);

    /// Cherche si il y a un état initial dans une liste
    /// \param list liste d'état
    /// \return bool
    static bool isAnyInitial(const vector<State *> &list);

    /// Cherche si il y a un état terminal dans une liste
    /// \param list liste d'état
    /// \return bool
    static bool isAnyFinal(const vector<State *> &list);

    /// Retrouve les état initiaux et finaux d'une liste
    /// \param list liste d'état
    /// \param list_initials liste des état initiaux
    /// \param list_finals Liste des état finaux
    static void
    getInitFinal(const vector<State *> &list, vector<State *> *list_initials, vector<State *> *list_finals);

    /// Ajoute à une liste d'état tout les états accessible par une transition epsilon
    /// \param state_list Liste d'état
    static void addEpsilon(vector<State *> &state_list);

    /// Trie les état dans l'ordre croissant
    /// \param list liste à trier
    static void sortStates(vector<State *> &list);
} State;


class Automate {
private:
    string name = "Automate Finis";
    vector<State *> states_list;  //Liste des états de l'automate
    vector<char> alphabet;  // Alphabet reconnu

    stringstream correspondance; // Table des correspondance

    bool _minimized = false;
    bool _determinized = false;
    bool _complete = false;
    bool _synchronous = false;
    bool _standard = false;

    /// Libère un automate de la mémoire
    void freeFA();

    /// \param nom du fichier
    /// \return si l'opération à réussi
    bool faFromFile(ifstream &file);

    /// \param baseState état de l'automate à copier
    /// \param newID id du nouvel automate
    /// \return Pointeur vers l'automate crée
    State *copyStatesProcess(const string &newID, vector<State *> &baseState);

    /// Vérifie si un automate est synchrone
    void checkSynchronous();

    /// Vérifie si un automate est deterministe
    void checkDeterministic();

    /// Vérifie si un automate est complet
    void checkComplete();

    /// Vérifie si un automate est standard
    bool checkStandard();

    /// Trie les transitions et les états de l'automate
    void sort();

public:
    Automate();

    /// Génére un automate à partir d'un fichier
    /// \param nom du fichier, et son chemin
    explicit Automate(const string &nameFile);

    /// Crée un automate à partir d'une liste d'état et d'un alphabet
    /// \param states Vecteur d'état
    /// \param alphabet Vecteur d'alphabet
    Automate(vector<State *> *states, vector<char> &alphabet);

    /// Constructeur de copie
    /// \param cp Automate à copier
    Automate(Automate &cp);

    ~Automate();

    ///Surcharge de <<
    /// \param os flux d'entrée
    /// \param ar Automate à afficher
    /// \return le flux
    friend ostream &operator<<(ostream &os, const Automate &ar);

    /// Reconnaissance de mot
    /// \param chaine à tester
    /// \return bool
    bool recognizeWord(const string &word);

    /// Vérifie si un automate est complet
    /// \param display s'il faut afficher le process
    /// \return bool
    bool isComplete(bool display = false) const;

    /// Vérifie si un automate est deterministe
    /// \param s'il faut afficher le process
    /// \return bool
    bool isDeterministic(bool display = false) const;

    /// Vérifie si un automate est standard
    /// \param display s'il faut afficher le process
    /// \return bool
    bool isStandard(bool display = false) const;

    /// Complete un automate
    /// \return pointeur vers un automate
    Automate *complete();

    friend Automate *completion(Automate &ar);

    /// Determinise un automate
    /// \return pointeur vers un automate
    Automate *determinize();

    friend State *determinisation(vector<State *> &new_states,
                                  const vector<State *> &same_states, const vector<State *> &init,
                                  const vector<State *> &fin, Automate &af);

    ///Standardise un automate
    /// \return pointeur vers un automate
    Automate *standardisation();

    ///Minimise un automate
    /// \param s'il faut afficher le process
    /// \return pointeur vers un automate
    Automate *minimize();


    /// Complementarise un automate
    /// \return pointeur vers un automate
    Automate *complementarize();

    /// Lance tous les test
    void runTest();

    /// getters vers le nom
    string getName();


};

typedef struct Part {
    vector<State *> state_group;
    vector<int> sous_part;
} Part;

typedef vector<Part> *Partition;


#endif //DATA_STRUCTURES_HEADER
