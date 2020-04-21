#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>

using namespace std;

typedef struct transition transition;
typedef struct state state;

struct transition {
    char character;
    state* dest_state;
};

struct state {
    vector<transition> transition_list;
    int id;
    bool initial;
    bool terminal;
};

class Automate {
private:
    string name; // Nom de l'automate
    int state_number;
    vector<state> state_list; // Liste des transitions

    int alphabet_size;
    char *alphabet; // Alphabet
    bool synchronous;
    bool determinized;
    bool complete;
    bool minimized;
public:
    Automate();

    explicit Automate(int number); // Constructeur
    explicit Automate(vector<state>); // Constructeur à partir d'un vecteur d'état
    Automate(Automate &cp); // Constructeur de copie

    friend ostream &operator<<(ostream &os, const Automate &ar); // Affichage par surcharge de l'opérateur <<
    // Determinisation et complétion
    friend void determinizationCompletion(Automate &ar);

    friend Automate determinizationCompletionAsynchronous(Automate &ar);

    friend Automate completion(Automate &ar);

    friend Automate complementarization(Automate &ar);

    friend Automate standardisation(Automate &ar);

    friend Automate determinizationCompletionSynchronous(Automate &ar);

    bool isSynchronous();

    bool isDeterminized();

    bool isComplete();
};


#endif //DATA_STRUCTURES_H
