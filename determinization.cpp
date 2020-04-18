#include "data_structures.h"

void determinizationCompletion(Automate &ar) {
    Automate afdc;
    if (!ar.synchronous) {
        cout << "L'automate est asynchrone" << endl;
        afdc = determinizationCompletionAsynchronous(ar);
    } else {
        if (ar.determinized) {
            cout << "L'automate est déjà deterministe" << endl;
            if (ar.complete) {
                cout << "l'automate est déjà complet" << endl;
                afdc = Automate(ar);
            } else
                afdc = completion(ar);
        }
        else
            afdc = determinizationCompletionSynchronous(ar);
    }
    //cout << afdc << endl;
}

Automate determinizationCompletionAsynchronous(Automate &ar) {
    return ar;
}

Automate completion(Automate &ar) {
    return ar;
}

Automate determinizationCompletionSynchronous(Automate &ar){
    return ar;
}