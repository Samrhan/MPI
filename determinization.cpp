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
    Automate ar2(ar);


    ar2 =completion(ar2);
    return ar2;
}

Automate completion(Automate &ar) {
    bool verif= false;
    state trash;
    trash.terminal = false;
    trash.initial = false;
    trash.id = -1;
    for (int i = 0; i < ar.alphabet_size - 1; i++) {
        transition tmp;
        tmp.state= -1;
        tmp.character=ar.alphabet[i];
        trash.transition_list.push_back(tmp);
        }
    ar.state_list.push_back(trash);
    for (auto &i : ar.state_list) {
        if(i.transition_list.size()!=ar.alphabet_size ){
            for (int j = 0;j < ar.alphabet_size - 1; j++) {
                for (auto &k : i.transition_list){
                    if(ar.alphabet[j]==k.character){
                        verif=true;
                        break;
                    }
                }
                if(!verif){
                    transition tmp;
                    tmp.state= -1;
                    tmp.character=ar.alphabet[j];
                    i.transition_list.push_back(tmp);

                }
                verif=false;
            }
        }
    }

    return ar;
}

Automate determinizationCompletionSynchronous(Automate &ar){
    return ar;
}
