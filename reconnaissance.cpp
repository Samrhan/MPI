#include "prototypes.h"

#include <iostream>

using namespace std;

bool Automate::recognizeWord(const string &word) {
    vector<State *> cur;
    State::getInitFinal(states_list, &cur, nullptr); // recuperation des etats initiaux
    for (char c : word) {
        if (cur.empty()) {
            break;
        }

        cur = readChar(c, cur);
    }
    State::addEpsilon(cur); // On ajoute à cur les états accessible par une transition epsilon

    return State::isAnyFinal(cur);
}

vector<State *> readChar(char c, vector<State *> current) {
    vector<State *> next;

    State::addEpsilon(current); //On ajoute à current les états accessible par une transition epsilon

    if (c == '*') {
        next = current;
    } else {
        for (State *st : current) {
            for (Transition *t : st->transition_list) {
                if (t->trans == c) {
                    if (!State::idSearch(t->dest->id, next)) {
                        next.push_back(t->dest);
                    }
                }
            }
        }
    }

    return next;
}

