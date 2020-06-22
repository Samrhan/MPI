#include "prototypes.h"


State *determinisation(vector<State *> &new_states, const vector<State *> &same_states,
                       const vector<State *> &init, const vector<State *> &fin, Automate &af) {

    bool is_init = init == same_states;
    vector<State *> group_state = same_states;
    if (!af._synchronous) {
        // On récupère tout les états accessible avec une transition epsilon si l'automate est asynchrone
        State::addEpsilon(group_state);
    }

    string new_id = concatenateId(same_states);
    // On vérifie si l'état n'existe pas déjà
    auto search = State::idSearch(new_id, new_states);
    if (search != nullptr) {
        return search;
    }

    // On crée un nouvel état
    auto *actual = new State;
    actual->id = new_id;
    actual->initial = is_init;
    actual->terminal = State::isAnyFinal(same_states);
    new_states.push_back(actual);
    // On fusionne les transitions avec le même caractère
    for (auto c: af.alphabet) {
        auto same_final_state = new vector<State *>;
        for (State *st: same_states) {
            for (Transition *tr: st->transition_list) {
                // Retourne vrai ou faux selon si la destination de la transition est déjà dans la lite
                bool not_present = same_final_state->end() ==
                                   find_if(same_final_state->begin(), same_final_state->end(),
                                           [tr](const State *s) -> bool { return s == tr->dest; });
                if (tr->trans == c && not_present) {
                    // Si l'état n'est pas dans la liste, et que le caractère correspond
                    same_final_state->push_back(tr->dest);
                }
            }
        }
        // Génération de la transition avec le caractère et l'adresse de l'état terminal
        if (!same_final_state->empty()) {
            auto tr = new Transition;
            tr->trans = c;
            tr->dest = determinisation(new_states, *same_final_state, init, fin, af);
            actual->transition_list.push_back(tr);
        }
    }
    return actual;
}

Automate *Automate::determinize() {
    if (_determinized) {
        return this;
    }

    auto initials = new vector<State *>;
    auto finals = new vector<State *>;
    auto new_state = new vector<State *>;

    // On récupère les états initiaux et finaux
    State::getInitFinal(states_list, initials, finals);
    determinisation(*new_state, *initials, *initials, *finals, *this);
    // Création de l'automate
    auto determinized = new Automate(new_state, alphabet);
    determinized->name = name + " Deterministe";
    return determinized;
}