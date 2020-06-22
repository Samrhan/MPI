#include "prototypes.h"

Automate *Automate::complete() {
    if (!_complete) {
        auto newFA = completion(*this);
        newFA->_complete = true;
        newFA->name = name + " Complet";
        return newFA;
    } else {
        return this;
    }

}

Automate *completion(Automate &ar) {
    auto af_complete = new Automate(ar);
    auto new_state = new State;
    new_state->id = to_string(ar.states_list.size());
    new_state->terminal = false;
    new_state->initial = false;
    for (auto &i : af_complete->alphabet) {
        auto tmp = new Transition;
        tmp->trans = i;
        tmp->dest = new_state;
        new_state->transition_list.push_back(tmp);
    }
    for (auto &i: af_complete->states_list) {
        for (auto &j : af_complete->alphabet) {
            bool in = false;
            for (auto &k : i->transition_list) {
                if (k->trans == j)
                    in = true;
            }
            if (!in) {
                auto tmp = new Transition;
                tmp->trans = j;
                tmp->dest = new_state;
                i->transition_list.push_back(tmp);
            }
        }
    }
    bool in = false;
    for (auto &i : af_complete->states_list) {
        for (auto &j : i->transition_list) {
            if (j->dest == new_state)
                in = true;
        }
    }
    if (in) {
        af_complete->states_list.push_back(new_state);
    } else delete new_state;
    return af_complete;

}

