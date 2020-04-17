#include "data_structures.h"

Automate::Automate(int number) {
    ifstream file("../AF/test" + to_string(number) + ".txt");
    if (!file) {
        cout << "Automate inexistant" << endl;
        file.close();
    } else {
        name = "test" + to_string(number);
        string line;
        int n = 1;

        while (getline(file, line)) //Tant qu'on n'est pas à la fin, on lit
        {
            if (n == 1) { // Implémentation de l'alphabet
                string str_alphabet;
                const char *tmp;
                alphabet_size = stoi(line);
                for (int i = 0; i < alphabet_size; i++)
                    str_alphabet += 'a' + i;
                tmp = str_alphabet.c_str();
                alphabet = new char[alphabet_size];
                for (int i = 0; i < alphabet_size; i++) {
                    alphabet[i] = tmp[i];
                }

            }
            if (n == 2) { // On reserve l'espace pour les états
                state_number = stoi(line);
                for (int i = 0; i < state_number; i++) {
                    state tmp;
                    tmp.id = i;
                    state_list.push_back(tmp);
                }
            }


            if (n == 3) { // On indique si les états sont initiaux
                for (int i = 2; i <= line.size(); i += 2) {
                    state_list[line[i] - 48].initial = true;
                }

            }
            if (n == 4) {
                for (int i = 2; i <= line.size(); i += 2) {
                    state_list[line[i] - 48].terminal = true;
                }

            }
            if (n >= 6) {
                transition tmp;
                tmp.character = line[1];
                tmp.state = line[2] - 48;
                if (line[0] - 48 >= state_number)
                    cout << "Il n'y a pas assez d'état pour ces transitions" << endl;
                else
                    state_list[line[0] - 48].transition_list.push_back(tmp);
            }
            n++;


        }
    }
    file.close();
    synchronous = isSynchronous();
    determined = isDeterminized();
}

Automate::Automate(Automate &cp) {
    name = cp.name + " copie";
    state_number = cp.state_number;
    alphabet_size = cp.alphabet_size;
    alphabet = new char[alphabet_size];
    for (int i = 0; i < alphabet_size; i++) {
        alphabet[i] = cp.alphabet[i];
    }

    for (auto &i : cp.state_list) {
        state tmp;
        tmp.terminal = i.terminal;
        tmp.initial = i.initial;
        state_list.push_back(tmp);
        for (auto &j : i.transition_list) {
            transition tmp;
            tmp.character = j.character;
            tmp.state = j.state;
            state_list[i].transition_list.push_back(tmp);
        }
    }
}


ostream &operator<<(ostream &os, const Automate &af) {
    os << "Nom : " << af.name << endl;
    os << "Alphabet reconnu : {";
    for (int i = 0; i < af.alphabet_size - 1; i++) {
        os << af.alphabet[i] << ", ";
    }
    os << af.alphabet[af.alphabet_size - 1] << "}" << endl;
    os << "Il a " << af.state_number << " etats" << endl;
    for (auto &i : af.state_list) {
        os << "L'etat " << i.id << " :" << endl;
        if (i.initial)
            os << "    - est initial" << endl;
        if (i.terminal)
            os << "    - est terminal" << endl;
        os << "    - possede " << i.transition_list.size() << " transition(s)" << endl;
        for (auto &j : i.transition_list) {
            os << "une transition " << j.character << " vers l'etat "
               << j.state << endl;
        }
        if (af.synchronous)
            os << "   - est synchrone" << endl;
        else
            os << "   - est asynchrone" << endl;
        if (af.determined)
            os << "   - est deterministe" << endl;
        else
            os << "   - est non deterministe" << endl;
        os << endl;
    }
    return os;
}

bool Automate::isSynchronous() {
    for (auto &i : state_list) {
        for (auto &j : i.transition_list)
            if (j.character == '*')
                return false;
    }
    return true;
}

bool Automate::isDeterminized() {
    int number_initial = 0;
    int number_char = 0;
    char tmp;
    for (auto &i : state_list) {
        if (i.initial)
            number_initial++;
        if (number_initial > 1)
            return false;
        for (auto &j : i.transition_list) {
            if (j.character != tmp) {
                number_char = 0;
                tmp = j.character;
            } else
                number_char++;
            if (number_char > 1)
                return false;
        }
    }

    return true;
}

Automate Automate::pre_determinization() {
    vector<state> similar_state;
    vector<state> new_list_state;
    for (auto &i : state_list)
        if (i.initial)
            similar_state.push_back(i);
    determinization(similar_state, new_list_state);
    Automate new_automate(1);
    return new_automate;
}

state Automate::determinization(vector<state> &similar_states, vector<state> &new_list_state) {
    if (!synchronous) {
        for (auto &i :state_list) {
            for (auto &j : i.transition_list) {
                if (j.character == '*') {
                    similar_states.push_back(state_list[j.state]);
                }
            }
        }
    }
    state new_state;
    return new_state;

}