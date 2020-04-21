#include "data_structures.h"

const vector<string> split(const string &s, const char &c) {
    string buff{""};
    vector<string> v;

    for (auto n:s) {
        if (n != c) buff += n;
        else if (n == c && buff != "") {
            v.push_back(buff);
            buff = "";
        }
    }
    if (buff != "") v.push_back(buff);

    return v;
}

Automate::Automate() {
    name = "new Automate";
    state_number = 0;
    alphabet_size = 0;
    alphabet = {};
    synchronous = false;
    determinized = false;
    complete = false;
    minimized = false;
}

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
                    tmp.id = to_string(i);
                    state_list.push_back(tmp);
                }
            }


            if (n == 3) { // On indique si les états sont initiaux
                vector<string> init{split(line, ' ')};
                for (int i = 1; i < init.size(); i++) {
                    state_list[stoi(init[i])].initial = true;
                }

            }
            if (n == 4) {
                vector<string> termin{split(line, ' ')};
                for (int i = 1; i < termin.size(); i++) {
                    state_list[stoi(termin[i])].terminal = true;
                }

            }
            if (n >= 6) {
                transition tmp;
                vector<char> transi(line.begin(), line.end());
                string from_where;
                char which_id;
                string to_where;
                bool passed = false;
                for (auto &i : transi) {
                    if (i - 97 < 0) {
                        if (!passed) {
                            from_where += i;
                        } else {
                            to_where += i;
                        }
                    } else {
                        passed = true;
                        which_id = i;
                    }
                }


                tmp.character = which_id;
                tmp.dest_state = &state_list[stoi(to_where)];
                if (line[0] - 48 >= state_number)
                    cout << "Il n'y a pas assez d'état pour ces transitions" << endl;
                else
                    state_list[stoi(from_where)].transition_list.push_back(tmp);
            }
            n++;


        }
    }
    file.close();
    synchronous = isSynchronous();
    determinized = isDeterminized();
}

Automate::Automate(vector<state>) {

}

Automate::Automate(Automate &cp) {
    name = cp.name + " copie";
    state_number = cp.state_number;
    alphabet_size = cp.alphabet_size;
    alphabet = new char[alphabet_size];
    for (int i = 0; i < alphabet_size; i++) {
        alphabet[i] = cp.alphabet[i];
    }
    int k = 0;
    for (auto &i : cp.state_list) {
        state tmp_state;
        tmp_state.terminal = i.terminal;
        tmp_state.initial = i.initial;
        tmp_state.id = i.id;
        state_list.push_back(tmp_state);
        for (auto &j : i.transition_list) {
            transition tmp_transition;
            tmp_transition.character = j.character;
            tmp_transition.dest_state = j.dest_state;
            state_list[k].transition_list.push_back(tmp_transition);
        }
        k++;
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
               << j.dest_state->id << endl;
        }
        os << endl;
    }
    if (af.synchronous)
        os << "   - est synchrone" << endl;
    else
        os << "   - est asynchrone" << endl;
    if (af.determinized)
        os << "   - est deterministe" << endl;
    else
        os << "   - est non deterministe" << endl;
    os << endl;
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
        tmp = i.transition_list[0].character;
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

bool Automate::isComplete() {
    bool in = false;
    for (auto &i : state_list) {
        if (i.transition_list.size() < alphabet_size) {
            return false;
        }
        for (int j = 0; j < alphabet_size; j++) {
            for (auto &k : i.transition_list) {
                if (alphabet[j] == k.character) {
                    in = true;
                }
            }
            if (!in)
                return false;
            in = false;
        }
    }
    return false;
}

