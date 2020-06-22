#include "prototypes.h"


Automate *Automate::minimize() {

    if (_complete && _determinized) {
        auto partition = new vector<Part>;
        auto finals = new Part;
        auto non_finals = new Part;

        for (State *state: states_list) {
            if (state->terminal) {
                finals->state_group.push_back(state); //Final
            } else {
                non_finals->state_group.push_back(state); //NonFinal
            }
        }
        if (!non_finals->state_group.empty()) {
            partition->push_back(*non_finals);
        }
        if (!finals->state_group.empty()) {
            partition->push_back(*finals);
        }


        cout << *this << endl;
        printPartition(partition, 0, alphabet);


        partition = partitioning(partition, alphabet, 1);

        //Crée un nouvel automate
        auto minized_af = new Automate;

        minized_af->name = name + " Minimal";
        minized_af->correspondance << endl << "Table de correspondance " << name << " vers " << minized_af->name << ":"
                                   << endl << endl;
        minized_af->states_list = *partitionToState(partition, alphabet, minized_af->correspondance);
        if (minized_af->states_list.size() == states_list.size()) {
            cout << "L'automate " << name << " est deja minimal" << endl;
            _minimized = true;
        }

        minized_af->alphabet = alphabet;
        minized_af->_complete = true;
        minized_af->_determinized = true;
        minized_af->_minimized = true;
        minized_af->_synchronous = true;
        return minized_af;

    } else {
        cout << "L'automate doit être complet et déterministe" << endl;
        return nullptr;
    }

}


static Partition partitioning(Partition initial_partition, const vector<char> &alphabet, int n) {
    vector<Partition> all_partitions;
    vector<int> *part;
    Partition next_part;
    Part *new_part_group;
    int p_size = initial_partition->size();
    vector<Part>::iterator end_part_groups;

    for (auto const &parts_group  : *initial_partition) {
        next_part = new vector<Part>;

        for (State *St : parts_group.state_group) {
            part = getPart(initial_partition, St->transition_list, alphabet);
            for (end_part_groups = next_part->begin(); end_part_groups != next_part->end(); ++end_part_groups) {
                if (!isSamePart(*part, end_part_groups->sous_part)) continue;
                end_part_groups->state_group.push_back(St);
                break;
            }
            if (end_part_groups == next_part->end()) {
                new_part_group = new Part;
                new_part_group->state_group.push_back(St);
                new_part_group->sous_part = (*part);
                next_part->push_back(*new_part_group);
            }
        }
        //Il est important d’avoir un vecteur de partitions puisque des parties similaires ne devraient pas
        // se mélanger pendant le partitionnement s’ils proviennent de différents groupes de parties
        all_partitions.push_back(next_part);
    }

    next_part = new vector<Part>;
    for (auto &i: all_partitions) {
        next_part->insert(next_part->end(), i->begin(), i->end());
    }

    if (next_part->size() <= p_size) {
        printPartition(next_part, -1, alphabet);
    } else {
        printPartition(next_part, n, alphabet);
        next_part = partitioning(next_part, alphabet, n + 1);
    }

    return next_part;
}

static vector<State *> *partitionToState(Partition partition, vector<char> &alphabet, stringstream &correspondance) {
    auto fa_states = new vector<State *>;
    State *new_state;

    for (int l = 0; l < partition->size(); l++) {
        new_state = new State;
        new_state->id = to_string(l);
        correspondance << " " << l << ":  ";

        for (auto &i : (*partition)[l].state_group) {
            if (i != (*partition)[l].state_group[0]) {
                correspondance << " + ";
            }
            correspondance << i->id;
        }
        correspondance << endl;

        new_state->initial = State::isAnyInitial((*partition)[l].state_group);
        new_state->terminal = State::isAnyFinal((*partition)[l].state_group);
        fa_states->push_back(new_state);
    }

    Transition *new_trans;

    for (int i = 0; i < fa_states->size(); i++) {
        for (int j = 0; j < alphabet.size(); j++) {

            /*Pour chaque état, on crée les transitions. Pour ce faire, il passe par tous les caractères et pour chacun,
             il regarde le groupe à partir duquel il a été fait dans la partition originale, il regarde cette partie
             des groupes, en regardant la transition du caractère dans cette partie, il trouve l’index du groupe
             auquel il a été attribué. Cet index est équivalent à l’index de son état correspondant dans la nouvelle
             liste des états et donc, nous pouvons accéder à cet état pour l’ajouter en tant que transition*/

            new_trans = new Transition;
            new_trans->trans = alphabet[j];
            new_trans->dest = (*fa_states)[(*partition)[i].sous_part[j]];
            (*fa_states)[i]->transition_list.push_back(new_trans);
        }
    }

    return fa_states;
}

static vector<int> *getPart(Partition prec_partition, vector<Transition *> &state_exits, const vector<char> &alphabet) {
    auto new_part = new vector<int>;
    for (char c: alphabet) {
        for (Transition *T : state_exits) {
            //Permet de s'assurer que l'ordre des parties et cohérant
            if (T->trans == c) {
                for (int i = 0; i < prec_partition->size(); i++) {
                    if (State::idSearch(T->dest->id, (*prec_partition)[i].state_group)) {
                        new_part->push_back(i);
                    }
                }
                break;
            }
        }
    }
    return new_part;
}


static bool isSamePart(vector<int> &part_1, vector<int> &part_2) {
    auto i = part_1.begin(), j = part_2.begin();
    for (i, j; i != part_1.end() && j != part_2.end(); ++i, ++j) {
        if (*i != *j)
            return false;
    }

    return i == part_1.end() && j == part_2.end();

}

static void printPartition(Partition partition, int n, vector<char> alphabet) {
    if (n != -1)
        cout << endl << endl << "             " << "Partition " << n;
    else
        cout << endl << endl << "         Partition Finale";

    //Affiche chaque groupe
    for (int i = 0; i < partition->size(); i++) {
        cout << endl << endl << "   Partie " << i << endl << "      Sous-Partie: ";
        if (n != 0) {
            n == -1 ? cout << "(transition vers les groupes de cette partition)" : cout
                    << "(transition vers les groupes de la partition " << n - 1 << ")";
            for (int j = 0; j < alphabet.size(); j++) {
                cout << endl << "         " << alphabet[j] << " : Partie " << (*partition)[i].sous_part[j];
            }
        } else {
            cout << "Etat ";
            if (!State::isAnyFinal((*partition)[i].state_group))
                cout << "Non-";
            cout << "Terminaux" << endl;
        }
        //Affiche les état qui appartiennent à ce groupe
        cout << endl << "   Etats: ";
        for (int c = 0; c < (*partition)[i].state_group.size(); c++) {
            if (c != 0) cout << ", ";
            cout << (*partition)[i].state_group[c]->id;
        }
    }
}


