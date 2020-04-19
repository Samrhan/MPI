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
    cout << afdc << endl;
}

Automate determinizationCompletionAsynchronous(Automate &ar) {
    Automate ar2;
    ar2 = Automate(ar);


    ar2 =completion(ar2);
    return ar2;
}

Automate completion(Automate &ar) {
    Automate ar2;
    ar2 = Automate(ar);

    bool verif= false;
    state trash;
    trash.terminal = false;
    trash.initial = false;
    trash.id = -1;
    for (int i = 0; i < ar2.alphabet_size - 1; i++) {
        transition tmp;
        tmp.state= -1;
        tmp.character=ar2.alphabet[i];
        trash.transition_list.push_back(tmp);
        }
    ar2.state_list.push_back(trash);

    for (auto &i : ar2.state_list) {
        if(i.transition_list.size()!=ar2.alphabet_size ){
            for (int j = 0;j < ar2.alphabet_size - 1; j++) {
                for (auto &k : i.transition_list){
                    if(ar2.alphabet[j]==k.character){
                        verif=true;
                        break;
                    }
                }
                if(!verif){
                    transition tmp;
                    tmp.state= -1;
                    tmp.character=ar2.alphabet[j];
                    i.transition_list.push_back(tmp);

                }
                verif=false;
            }
        }
    }
    return ar2;
}

Automate determinizationCompletionSynchronous(Automate &ar){
    Automate ar2;
    ar2 = Automate(ar);

    int number_initial = 0;
    bool test =false;
    for (auto &i : ar2.state_list) {
        if (i.initial)
            number_initial++;
        if (number_initial > 1){
            test =true;
            break;}
    }
    if(test){
        state newentry;
        newentry.terminal = false;
        newentry.initial = true;
        newentry.id = -2;
        ar2.state_list.push_back(newentry);
        for (auto &i : ar2.state_list) {
            if (i.initial && i.id!=-2){
                i.initial=false;
                for (auto &k : i.transition_list){
                    transition tmp;
                    tmp.state= k.state;
                    tmp.character=k.character;
                    newentry.transition_list.push_back(tmp);
                }
            }
        }
    }
    char tmp;
    int tmp2;
    int tmp3;
    for (auto &i : ar2.state_list) {
        for (auto &k : i.transition_list){
            if (k.character == tmp) {
                cout << "ca marchote" << endl;
                cout << k.character << endl;
                cout << i.id << endl;
                cout << "ca marchote" << endl;
                state newstate;
                newstate.terminal = false;
                newstate.initial = false;
                newstate.id = 10*tmp3 + k.state;
            }
            tmp=k.character;
            tmp3=k.state;

        }
    }



    ar2 =completion(ar2);
    return ar2;
}
