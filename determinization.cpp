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
    trash.id = -1;
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
    int number_initial = 0;
    bool test = false;

    for (auto &i : ar2.state_list) {
        if (i.initial)
            number_initial++;
        if (number_initial > 1) {
            test = true;
            break;
        }
    }
    while (test) {
        state newentry;
        newentry.terminal = false;
        newentry.initial = true;
        newentry.id = -2;

        for (auto &i : ar2.state_list) {
            if (i.initial && i.id != -2) {
                i.initial = false;
                for (auto &k : i.transition_list) {
                    transition tmp;
                    tmp.dest_state = k.dest_state;
                    tmp.character = k.character;
                    newentry.transition_list.push_back(tmp);
                }
            }
        }
        ar2.state_list.push_back(newentry);
        number_initial=0;
        for (auto &i : ar2.state_list) {
            if (i.initial)
                number_initial++;
            if (number_initial > 1) {
                test = true;
                break;
            }
        }
    }

    char tmp;
    int tmp2;
    int tmp3;
    test = false;
    transition tmp4;
    for (auto &i : ar2.state_list) {
        for (auto &k : i.transition_list) {
            if (k.character == tmp) {

                if(!test){
                    state newstate;
                    newstate.terminal = false;
                    newstate.initial = false;
                    newstate.id = 10 * tmp3 + k.dest_state->id;
                    cout << "ca marchote" << endl;
                    cout << k.character << endl;
                    cout << i.id << endl;
                    cout << tmp3 << endl;
                    cout << newstate.id << endl;
                    cout << "ca marchote" << endl;

                    test=true;

                    transition tmptrans;
                    tmptrans.dest_state = k.dest_state;
                    tmptrans.character = k.character;
                    newstate.transition_list.push_back(tmptrans);

                    transition tmptrans2;
                    tmptrans2.dest_state = tmp4.dest_state;
                    tmptrans2.character = tmp4.character;
                    newstate.transition_list.push_back(tmptrans2);
                    
                    ar2.state_list.push_back(newstate);


                }

            }
            else{
                test = false;
            }
            tmp4 = k;
            tmp = k.character;
            tmp3 = k.dest_state->id;

        }
    }


    //ar2 = completion(ar2);
    return ar2;
}
Automate complementarization(Automate &ar) {
    Automate ar2;
    ar2 = Automate(ar);
    if(ar.isComplete() && ar.isComplete()){
        for (auto &i : ar2.state_list) {
            if(i.initial){
                i.initial=false;
            }else{
                i.initial=true;
            }
            if(i.terminal){
                i.terminal=false;
            }else{
                i.terminal=true;
            }
        }
        return ar2;
    }

}

Automate standardisation(Automate &ar) {
    Automate ar2;
    ar2 = Automate(ar);

    state newentry;
    newentry.terminal = false;
    newentry.initial = true;
    newentry.id = -2;
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

        return ar2;
    }
    ar2.state_list.push_back(newentry);
}
