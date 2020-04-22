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
        } else
            afdc = determinizationCompletionSynchronous(ar);
    }
    cout << afdc << endl;
}

Automate determinizationCompletionAsynchronous(Automate &ar) {
    Automate ar2;
    ar2 = Automate(ar);


    ar2 = completion(ar2);
    return ar2;
}

Automate completion(Automate &ar) {
    Automate ar2;
    ar2 = Automate(ar);

    bool verif = false;
    state trash;
    trash.terminal = false;
    trash.initial = false;
    trash.id = "P";
    for (int i = 0; i < ar2.alphabet_size - 1; i++) {
        transition tmp;
        tmp.dest_state = &trash;
        tmp.character = ar2.alphabet[i];
        trash.transition_list.push_back(tmp);
    }
    ar2.state_list.push_back(trash);

    for (auto &i : ar2.state_list) {
        if (i.transition_list.size() != ar2.alphabet_size) {
            for (int j = 0; j < ar2.alphabet_size - 1; j++) {
                for (auto &k : i.transition_list) {
                    if (ar2.alphabet[j] == k.character) {
                        verif = true;
                        break;
                    }
                }
                if (!verif) {
                    transition tmp;
                    tmp.dest_state = &trash;
                    tmp.character = ar2.alphabet[j];
                    i.transition_list.push_back(tmp);

                }
                verif = false;
            }
        }
    }
    return ar2;
}

Automate determinizationCompletionSynchronous(Automate &ar) {
    Automate ar2;
    ar2 = Automate(ar);

    state newentry;
    newentry.terminal = false;
    newentry.initial = true;
    newentry.id = "I";
    for (auto &i : ar2.state_list) {
        if(i.initial){
            i.initial=false;
            if(i.terminal){
                newentry.terminal=true;
            }
            for (auto &k : i.transition_list) {
                transition tmptrans;
                tmptrans.dest_state = k.dest_state;
                tmptrans.character = k.character;
                newentry.transition_list.push_back(tmptrans);
            }
        }


    }
    ar2.state_list.push_back(newentry);
    remake:
    state tmp;
    transition tmp2;
    int a =0;
    for (auto &i : ar2.state_list) {
        for (auto &k : i.transition_list) {

            if(k.character==tmp2.character){
                state newstate;

                newstate.initial = false;
                newstate.terminal=false;
                newstate.id = tmp2.dest_state->id + ","+ k.dest_state->id;
                if(tmp2.dest_state->terminal== true ||  k.dest_state->terminal== true){
                    newstate.terminal=true;
                }


                for (auto &l : k.dest_state->transition_list) {
                    transition tmptrans;
                    tmptrans.dest_state = l.dest_state;
                    tmptrans.character = l.character;
                    newstate.transition_list.push_back(tmptrans);
                }
                for (auto &l : tmp2.dest_state->transition_list) {
                    transition tmptrans;
                    tmptrans.dest_state = l.dest_state;
                    tmptrans.character = l.character;
                    newstate.transition_list.push_back(tmptrans);
                }

                k.dest_state=&newstate;
                i.transition_list.erase(i.transition_list.begin()+(a-1));

                goto remake;
            }

            a++;

            }

    }



    ar2 = completion(ar2);
    return ar2;
}
Automate complementarization(Automate &ar) {
    Automate ar2;
    ar2 = Automate(ar);
    if(ar.isComplete() && ar.isComplete()){
        for (auto &i : ar2.state_list) {
            i.initial = !i.initial;
            i.terminal = !i.terminal;
        }

    }
    return ar2;
}

Automate standardisation(Automate &ar) {
    Automate ar2;
    ar2 = Automate(ar);

    state newentry;
    newentry.terminal = false;
    newentry.initial = true;
    newentry.id = "I";
    for (auto &i : ar2.state_list) {
        if(i.initial){
            i.initial=false;
            if(i.terminal){
                newentry.terminal=true;
            }
            for (auto &k : i.transition_list) {
                transition tmptrans;
                tmptrans.dest_state = k.dest_state;
                tmptrans.character = k.character;
                newentry.transition_list.push_back(tmptrans);
            }
        }


    }
    ar2.state_list.push_back(newentry);
    return ar2;
}
