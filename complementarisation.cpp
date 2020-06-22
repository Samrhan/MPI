#include "prototypes.h"

Automate *Automate::complementarize() {
    if (_complete) {
        cout << "Automate complementaire de";
        if (_minimized) {
            cout << " sa version minimise." << endl;
        } else {
            cout << " sa version complete." << endl;
        }

        auto complementaire = new Automate(*this);
        for (State *i: complementaire->states_list) {
            i->terminal = !(i->terminal);
        }

        complementaire->name = name + " Complementaire";

        return complementaire;
    }
    return nullptr;
}