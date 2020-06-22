#include "prototypes.h"

Automate *Automate::standardisation() {

    if (_standard) {
        cout << "L'automate est deja standardise" << endl;
        return this;
    } else if (checkStandard()) { // On recheck parce qu'on sait jamais
        _standard = true;
        cout << "L'automate est deja standardise" << endl;
        return this;
    }

    auto *standard_fa = new Automate(*this);
    standard_fa->name = name + " Standard";
    auto *newIni = new State;
    newIni->initial = true;
    newIni->id = to_string(standard_fa->states_list.size() + 1);

    for (State *st: standard_fa->states_list) {
        if (st->initial) {

            if (!newIni->terminal) {
                if (st->terminal)
                    newIni->terminal = true;
            }

            for (auto i : st->transition_list) {
                newIni->transition_list.push_back(i);
            }

            st->initial = false;
        }
    }

    standard_fa->states_list.push_back(newIni);
    standard_fa->_standard = true;
    return standard_fa;

}

