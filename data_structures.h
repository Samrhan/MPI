#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>

using namespace std;

typedef struct transition {
    char character;
    int state;
} transition;

typedef struct state {
    vector<transition> transition_list;
    int id;
    bool initial;
    bool terminal;
} state;

class Automate {
private:
    string name; // Nom de l'automate
    int state_number;
    vector<state> state_list; // Liste des transitions

    int alphabet_size;
    char *alphabet; // Alphabet
    bool synchronous;
    bool determined;
    bool complete;
    bool minimized;
public:
    explicit Automate(int number); // Constructeur
    Automate(Automate &cp); // Constructeur de copie

    friend ostream &operator<<(ostream &os, const Automate &ar); // Affichage par surcharge de l'opérateur <<

    bool isSynchronous();

    bool isDeterminized();

    Automate pre_determinization();

    state determinization(vector<state> &similar_state, vector<state> &new_list_state);

};


#endif //DATA_STRUCTURES_H
