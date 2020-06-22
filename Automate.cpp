// Définitions des méthodes de Automate, State et Transition

#include "prototypes.h"

using namespace std;

//Constructeurs
Automate::Automate() = default;

Automate::Automate(Automate &cp)
        : alphabet(cp.alphabet), _synchronous(cp._synchronous), _minimized(cp._minimized), _complete(cp._complete),
          _determinized(cp._determinized) {

    auto init = new vector<State *>;
    auto final = new vector<State *>;
    State::getInitFinal(cp.states_list, init, final);

    for (State *st: *init) {
        copyStatesProcess(st->id, cp.states_list);
    }
}

Automate::Automate(vector<State *> *states, vector<char> &alphabet)
        : name("Automate Finis"), states_list(*states), alphabet(alphabet) {
    sort();
    runTest();
}

Automate::Automate(const string &nameFile)
        : name(nameFile) {
    ///Constructeur avec nom de fichier

    ifstream file(nameFile);

    if (is_empty_file(file)) {
        ifstream default_file("automate/B3-1.txt");
        cout << "Fichier vide, generation d'un automate par defaut" << endl;
        if (faFromFile(default_file)) {
            sort();
            runTest();
            file.close();
        }
    } else if (file) {
        if (faFromFile(file)) {
            sort();
            runTest();
            file.close();
        }
    }
}

Automate::~Automate() {
    /// Destructeur
    freeFA();
}

void Automate::freeFA() {
    for (State *st: states_list) {
        for (Transition *t: st->transition_list) {
            delete (t);
        }
        delete (st);
    }
    alphabet.clear();
    states_list.clear();
}

ostream &operator<<(ostream &os, const Automate &af) {
    /// Surcharge de << pour l'affichage

    // Affichage du nom, de l'alphabet reconnu et du nombre d'état
    os << "Nom : " << af.name << endl << "Alphabet reconnu : {";
    for (int i = 0; i < af.alphabet.size() - 1; i++) {
        os << af.alphabet[i] << ", ";
    }
    os << af.alphabet[af.alphabet.size() - 1] << "}" << endl;
    os << "Il a " << af.states_list.size() << " etats" << endl;

    // Création d'un tableau 2d pour l'affichage
    vector<vector<string>> table_transi;
    table_transi.emplace_back(vector<string>());
    table_transi[0].push_back("  ");
    table_transi[0].push_back("  ");
    table_transi[0].push_back(" ");
    table_transi[0].push_back(" | ");
    // ajout de la ligne de l'alphabet
    string tmp_alphabet(af.alphabet.begin(), af.alphabet.end());
    if (!af._synchronous)
        tmp_alphabet += "*";
    for (int i = 0; i < tmp_alphabet.size(); i++) {
        table_transi[0].push_back("");
        table_transi[0][4 + i].push_back(tmp_alphabet[i]);
        table_transi[0][4 + i] += " ";
    }
    // Ajout des lignes pour les transitions
    int n = 2;
    int max_lenght = 1;
    int max_lenght_state = 1;
    table_transi.emplace_back(vector<string>());


    for (auto &i : af.states_list) {
        table_transi.emplace_back(vector<string>());
        if (i->initial)
            table_transi[n].push_back("I ");
        else
            table_transi[n].push_back("  ");
        if (i->terminal)
            table_transi[n].push_back("T ");
        else
            table_transi[n].push_back("  ");
        string temp;

        temp += i->id;
        if (temp.size() > max_lenght_state)
            max_lenght_state = temp.size();

        table_transi[n].push_back(temp);
        table_transi[n].push_back(" | ");
        bool in = false;
        string tmp;
        for (char k : tmp_alphabet) {
            for (auto &j : i->transition_list) {
                if (j->trans == k) {
                    if (in)
                        tmp += ",";
                    tmp += j->dest->id;
                    in = true;
                }

            }

            if (!in) {
                table_transi[n].push_back("-");
            } else {
                tmp += "  ";
                table_transi[n].push_back(tmp);
                if (tmp.size() > max_lenght)
                    max_lenght = tmp.size();
                tmp = "";
            }
            in = false;


        }
        n++;


    }
    // Affichage avec équilibrage du tableau
    for (auto &i : table_transi) {
        int p = 0;
        for (auto &j : i) {
            if (j.size() < max_lenght && p >= 4) {
                while (j.size() < max_lenght) {
                    j += " ";
                }
            }
            if (j.size() <= max_lenght_state && p == 2)
                while (j.size() < max_lenght) {
                    j.insert(j.begin(), ' ');
                }
            p++;
            if (p > 4) {
                j += '|';
            }

            os << j;
        }


        os << endl;

    }
    if (af._minimized) {
        cout << af.correspondance.str() << endl;
    }
    if (af._synchronous)
        os << "   - est synchrone" << endl;
    else
        os << "   - est asynchrone" << endl;
    if (af._determinized)
        os << "   - est deterministe" << endl;
    else
        os << "   - est non deterministe" << endl;
    if (af._complete)
        os << "   - est complet" << endl;
    else
        os << "   - est non complet" << endl;
    if (af._minimized)
        os << "   - est minimal" << endl;
    else
        os << "   - n'est pas minimal" << endl;
    if (af._standard)
        os << "   - est standard" << endl;
    else
        os << "   - est non standard" << endl;
    return os;
}

vector<string> split(const string &s, const char &c) {
    ///Split une chaine en un vecteur de caractères
    string buff;
    vector<string> v;

    for (auto n:s) {
        if (n != c) buff += n;
        else if (n == c && !buff.empty()) {
            v.push_back(buff);
            buff = "";
        }
    }
    if (!buff.empty()) v.push_back(buff);

    return v;
}

bool is_empty_file(ifstream &pFile) {
    return pFile.peek() == std::ifstream::traits_type::eof();
}

bool Automate::faFromFile(ifstream &file) {
    ///Génération de l'automate à partir d'un fichier
    try {
        if (!file) {
            file.close();
            throw (bad_alloc());
        } else {
            if (is_empty_file(file))
                throw (bad_alloc());
            string line;
            int alphabet_size = 0;
            int state_number = 0;
            int n = 1;
            int n_transi = 0;
            while (getline(file, line)) //Tant qu'on n'est pas à la fin, on lit
            {
                if (n == 1) { // Implémentation de l'alphabet
                    string str_alphabet;
                    alphabet_size = stoi(line);
                    for (int i = 0; i < alphabet_size; i++) {
                        alphabet.push_back(char('a' + i));
                    }
                }
                if (n == 2) { // On reserve l'espace pour les états
                    state_number = stoi(line);
                    for (int i = 0; i < state_number; i++) {
                        auto tmp = new State;
                        tmp->id = to_string(i);
                        states_list.push_back(tmp);
                    }
                }
                if (n == 3) { // On indique si les états sont initiaux
                    vector<string> init{split(line, ' ')};
                    for (int i = 1; i < init.size(); i++) {
                        states_list[stoi(init[i])]->initial = true;
                    }
                    for (auto &i : states_list) {
                        if (!i->initial)
                            i->initial = false;
                    }
                }
                if (n == 4) { // On indique si les états sont terminaux
                    vector<string> termin{split(line, ' ')};
                    for (int i = 1; i < termin.size(); i++) {
                        states_list[stoi(termin[i])]->terminal = true;
                    }
                    for (auto &i : states_list) {
                        if (!i->terminal)
                            i->terminal = false;
                    }
                }
                if (n == 5) //Definition du nombre de transition à lire
                    n_transi = stoi(line);
                if (n >= 6 && n < 6 + n_transi) {//Génération des transitions
                    auto *tmp = new Transition;
                    vector<char> transi(line.begin(), line.end());
                    string from_where;
                    char which_id = 'a';
                    string to_where;
                    bool passed = false;
                    for (auto &i : transi) {
                        if (i - 97 < 0 && i != '*') {
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
                    tmp->trans = which_id;
                    tmp->dest = states_list[stoi(to_where)];

                    states_list[stoi(from_where)]->transition_list.push_back(tmp);

                }
                n++;
            }
            if (states_list.size() != state_number)
                throw (bad_alloc());
            int tmp = 0;
            for (auto &i : states_list)
                tmp += i->transition_list.size();
            if (tmp != n_transi)
                throw (bad_alloc());
            if (alphabet.size() != alphabet_size)
                throw (bad_alloc());

        }
        file.close();
        return true;
    }
    catch (bad_alloc &er) {
        cout << "Erreur dans le fichier. Generation d'un automate par defaut" << endl;
        return false;
    }

}

State *Automate::copyStatesProcess(const string &newID, std::vector<State *> &baseState) {
    // on vérifie si l'état n'existe pas déjà
    auto search = State::idSearch(newID, this->states_list);
    if (search != nullptr) {
        return search;
    }

    auto old = State::idSearch(newID, baseState);

    // On crée le nouvel état
    auto new_state = new State;
    new_state->id = newID;
    new_state->terminal = old->terminal;
    new_state->initial = old->initial;
    states_list.push_back(new_state);

    for (auto i: old->transition_list) {
        auto tr = new Transition;
        tr->trans = i->trans;
        tr->dest = copyStatesProcess(i->dest->id, baseState);
        new_state->transition_list.push_back(tr);
    }
    return new_state;
}

extern string concatenateId(vector<State *> sameStates) {
    State::sortStates(sameStates);
    string newID;
    for (State *st: sameStates) {
        if (!newID.empty()) { newID += "."; }
        newID += st->id;
    }
    return newID;
}

void Automate::checkSynchronous() {
    for (State *st: states_list) {
        for (Transition *tr: st->transition_list) {
            if (tr->trans == '*') {
                _synchronous = false;
                return;
            }
        }
    }
    _synchronous = true;

}

static void oneInitialState(const vector<State *> &list) {
    vector<State *> init;
    State::getInitFinal(list, &init, nullptr);
    if (init.size() > 1) {
        cout << "   Plus d'un état initial: ";
        for (State *st: init) {
            cout << st->id << " ";
        }
        cout << endl;
    }
}

static void uniqueTransition(const vector<State *> &list, vector<char> alphabet) {
    alphabet.push_back('*');
    for (State *st: list) {
        bool displayID = false;
        for (char c: alphabet) {
            auto sameCharacter = new vector<Transition *>;
            Transition::searchOccurrence(st->transition_list, c, *sameCharacter);
            if (sameCharacter->size() > 1) {
                if (!displayID) {
                    cout << "   Etat " << st->id << ", meme caractere pour plusieurs transitions" << endl;
                }
                displayID = true;
                cout << "    - caractere " << c << ", vers: ";
                for (Transition *tr: *sameCharacter) {
                    cout << tr->dest->id << " ";
                }
                cout << endl;
            }
        }
    }
}

bool Automate::isDeterministic(const bool display) const {
    if (display) {
        if (_determinized) {
            cout << "L'automate est deterministe" << endl;
        } else
            cout << "L'automate n'est pas deterministe" << endl;
        if (!_determinized) {
            if (!_synchronous) {
                cout << "   L'automate n'est pas sychrone" << endl;
            }
            oneInitialState(states_list);
            uniqueTransition(states_list, alphabet);
        }
    }
    return _determinized;
}

bool Automate::isStandard(const bool display) const {
    if (display) {
        if (!_standard) {
            auto *initials = new vector<State *>;
            State::getInitFinal(states_list, initials, nullptr);
            if (initials->size() != 1) {
                cout << "   Il ne devrait y avoir qu'un seul état initial. Il y a  : ";
                for (State *st: *initials)
                    cout << st->id << " ";
                cout << endl;
            } else {
                for (State *st: states_list) {
                    for (Transition *tr: st->transition_list) {
                        if (tr->dest == (*initials)[0]) {
                            cout << "   Etat " << st->id << ", Transition '" << tr->trans
                                 << "' vers l'état initial" << endl;
                        }
                    }
                }
            }
        }
    }
    return _standard;
}

void Automate::checkDeterministic() {
    bool det = false;

    if (_synchronous) {
        vector<State *> init;
        State::getInitFinal(states_list, &init, nullptr);
        if (init.size() > 1) {
            goto exit;
        }

        for (State *st: states_list) {
            for (char c: alphabet) {
                vector<Transition *> occurrences;
                Transition::searchOccurrence(st->transition_list, c, occurrences);
                if (occurrences.size() > 1) {
                    goto exit;
                }
            }
        }
        det = true;
    }
    exit:
    _determinized = det;
}


bool Automate::checkStandard() {
    bool std = true;
    auto init = new vector<State *>;
    State::getInitFinal(states_list, init, nullptr);
    if (init->size() == 1) {
        for (State *st: states_list) {
            for (Transition *tr: st->transition_list) {
                if (tr->dest->initial) {
                    std = false;
                    break;
                }
            }
        }
    } else {
        std = false;
    }
    return std;

}

void Automate::sort() {
    std::sort(alphabet.begin(), alphabet.end());
    State::sortStates(states_list);
    for (State *st: states_list) {
        Transition::sortTransitions(st->transition_list);
    }
}

bool Automate::isComplete(const bool display) const {
    if (display) {
        if (_complete) {
            cout << "L'automate est complet" << endl;
        } else
            cout << "L'automate n'est pas complet" << endl;
        if (!_complete) {
            if (!_determinized) {
                cout << "   L'automate n'est pas deterministe, il ne peut être complet."
                     << endl;
            } else {
                bool displayB = false;
                for (State *st: states_list) {
                    bool emptyTransition = false;
                    for (char c: alphabet) {
                        if (Transition::searchByCharacter(st->transition_list, c) == nullptr) {
                            emptyTransition = true;
                        }
                    }
                    if (emptyTransition) {
                        if (!displayB) {
                            displayB = true;
                            cout << "   Transition manquante aux etats: ";
                        }
                        cout << st->id << " ";
                    }
                }
                if (displayB) {
                    cout << endl;
                }
            }
        }
    }
    return _complete;
}

void Automate::checkComplete() {
    bool comp = true;
    if (_determinized) {
        for (State *st: states_list) {
            for (char c: alphabet) {
                vector<Transition *> occurrences;
                Transition::searchOccurrence(st->transition_list, c, occurrences);
                if (occurrences.size() != 1) {
                    comp = false;
                    break;
                }
            }
        }
    } else {
        comp = false;
    }
    _complete = comp;
}


void Automate::runTest() {
    checkSynchronous();
    checkDeterministic();
    checkComplete();
    checkStandard();
}

std::string Automate::getName() {
    return name;
}


// Méthodes de la structures State

State *State::idSearch(std::string id, std::vector<State *> list) {
    auto it = new vector<State *>::iterator;
    *it = find_if(list.begin(), list.end(), [&id](State *st) -> bool {
        return st->id == id;
    });
    if (*it != list.end()) {
        return **it;
    }
    return nullptr;
}


bool State::isAnyInitial(const vector<State *> &list) {
    for (State *st: list) {
        if (st->initial) {
            return true;
        }
    }
    return false;
}

bool State::isAnyFinal(const vector<State *> &list) {
    for (State *st: list) {
        if (st->terminal) {
            return true;
        }
    }
    return false;
}

void State::getInitFinal(const vector<State *> &list, vector<State *> *list_initials, vector<State *> *list_finals) {
    for (State *st: list) {
        if (list_initials) {
            if (st->initial) {
                list_initials->push_back(st);
            }
        }
        if (list_finals) {
            if (st->terminal) {
                list_finals->push_back(st);
            }
        }
    }
}

void State::addEpsilon(std::vector<State *> &state_list) {
    for (int i = 0; i < state_list.size(); i++) {
        for (auto &tr: state_list[i]->transition_list) {
            if (tr->trans == '*') {
                if (!(State::idSearch(tr->dest->id, state_list))) {
                    state_list.push_back(tr->dest);
                }
            }
        }
    }
}

static string readingPartID(string &id) {
    size_t pos;
    string value = id, delimiter = ".";
    if ((pos = id.find(delimiter)) != string::npos) {
        value = id.substr(0, pos);
        id.erase(0, pos + delimiter.length());
    }
    return value;
}

static bool isIDSmaller(const State *st1, const State *st2) {
    bool equal = true;
    string tmp1, tmp2;
    string id1 = st1->id, id2 = st2->id;
    do {
        tmp1 = readingPartID(id1);
        tmp2 = readingPartID(id2);
        if (stoi(tmp1) != stoi(tmp2)) {
            equal = false;
        }
    } while (equal);
    return stoi(tmp1) < stoi(tmp2);
}

void State::sortStates(std::vector<State *> &list) {
    sort(list.begin(), list.end(), isIDSmaller);
}

static bool isTransitionSmaller(const Transition *t1, const Transition *t2) {
    if (int(t1->trans) < int(t2->trans)) {
        return true;
    } else if (t1->trans == t2->trans) {
        return isIDSmaller(t1->dest, t2->dest);
    }
    return false;
}


// Méthodes de la structures Transition


void Transition::sortTransitions(std::vector<Transition *> &list) {
    sort(list.begin(), list.end(), isTransitionSmaller);
}


Transition *Transition::searchByCharacter(vector<Transition *> &list, char c) {
    vector<Transition *>::iterator it;
    it = find_if(list.begin(), list.end(), [&c](Transition *tr) -> bool {
        return tr->trans == c;
    });
    if (it != list.end()) {
        return *it;
    }
    return nullptr;
}

void Transition::searchOccurrence(const vector<Transition *> &list, char c, vector<Transition *> &occurence_list) {
    for (Transition *tr: list) {
        if (tr->trans == c) {
            occurence_list.push_back(tr);
        }
    }
}

