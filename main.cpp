#include "prototypes.h"

int main() {
    auto list_automata = new vector<Automate *>;
    int n = 1;
    while (n) {
        if (!list_automata->empty()) {
            cout << "Automate - Projet Maths pour l'Informatique" << endl;
            cout << "Choisissez une action : " << endl;
            cout << "[1] Generer un automate a partir d'un fichier" << endl;
            cout << "[2] Afficher un automate" << endl;
            cout << "[3] Effectuer une operation" << endl;
            cout << "[4] Reconnaissance de mot" << endl;
            cout << "[5] Executer toutes les operations (Trace dans un fichier)" << endl;
            cout << "[6] Executer toutes les operations sur tous les automates (Trace dans un fichier)" << endl;
            cout << "[0] Quitter" << endl;
            cin >> n;
        }
        while (n < 0 || n > 6) {
            cout << "Choix invalide" << endl;
            cin >> n;
        }
        if (n)
            actionMenu(n, list_automata);
    }
    return 0;
}


void actionMenu(int action, vector<Automate *> *automate_list) {
    switch (action) {
        case 1:
            createAutomateFromFiles(automate_list);
            break;

        case 2:
            Automate *temp;
            temp = chooseAf(automate_list);
            if (temp != nullptr)
                cout << *temp << endl;
            break;

        case 3:
            Automate *temp1;
            temp1 = chooseAf(automate_list);
            if (temp1 == nullptr)
                break;
            int i;
            i = displayActionMenu();
            actionAutomate(i, temp1, automate_list);
            break;
        case 4:
            wordReco(automate_list);
            break;
        case 5:
            executeAll(automate_list);
            break;
        case 6:
            executeAllforAll();
            break;
        default:
            cout << "Erreur" << endl;
            break;
    }
}

void createAutomateFromFiles(vector<Automate *> *automate_list) {
    string my_file, trace;
    string n = to_string(askNumber());
    if (n != "0") {
        my_file = "automate/B3-" + n + ".txt";

        for (auto &i: *automate_list) {
            if (i->getName() == my_file) {
                cout << "Automate deja en memoire" << endl;
                return;
            }
        }
        auto *temp = new Automate(my_file);
        automate_list->push_back(temp);
        cout << *temp << endl;
    } else {
        for (int i = 1; i < 45; i++) {
            my_file = "automate/B3-" + to_string(i) + ".txt";
            bool exist = false;
            for (auto &j: *automate_list) {
                if (j->getName() == my_file) {
                    cout << "Automate " + my_file + " deja en memoire" << endl;
                    exist = true;
                    break;
                }
            }
            if (!exist) {
                auto *temp = new Automate(my_file);
                automate_list->push_back(temp);
            }
        }
    }
}


Automate *chooseAf(vector<Automate *> *automate_list) {
    if (automate_list->size() > 1) {
        cout << "Choisissez l'Automate :" << endl;
        for (int i = 0; i < automate_list->size(); i++) {
            cout << "[" << i + 1 << "] : " << (*automate_list)[i]->getName() << endl;
        }
        int i = 0;
        while (i < 1 || i > automate_list->size()) {
            cout << "(0 pour quitter):";
            cin >> i;
            if (i > -1 && i < automate_list->size() + 1) {
                if (!i)
                    return nullptr;
                return (*automate_list)[i - 1];
            }
        }
    } else
        return (*automate_list)[0];
    return nullptr;
}

int askNumber() {
    int r = 0;
    while (r <= 0 || r > 44) {
        cout << "Saisissez le numero de l'automate : " << endl;
        cout << "(0 pour charger tous les automates)" << endl;
        cin >> r;
        if (r >= 0 && r < 45)
            return r;
        cout << "Automate inexistant" << endl;
    }
    return 0;
}


int displayActionMenu() {
    cout << "Choisissez une action : " << endl;
    cout << "[1] Determinisation et complete" << endl;
    cout << "[2] Standardisation" << endl;
    cout << "[3] Minimisation" << endl;
    cout << "[4] Complementarisation" << endl;
    cout << "[5] Sortie" << endl;

    int i = 0;
    cin >> i;
    while (i < 1 || i > 6) {
        cout << "Mauvais choix" << endl;
        cin >> i;
    }
    return i;
}

void actionAutomate(int n, Automate *automate, vector<Automate *> *automate_list) {
    switch (n) {
        case 1:
            determinizeCompleteAndAdd(automate, automate_list);
            break;
        case 2:
            standardizeAndAdd(automate, automate_list);
            break;
        case 3:
            minimizeAndAdd(automate, automate_list);
            break;
        case 4:
            complementarizationAndAdd(automate, automate_list);
            break;
        default:
        case 5:
            break;
    }
}


Automate *completionAndAdd(Automate *automate, vector<Automate *> *automate_list) {
    if (!automate->isComplete(true)) {
        Automate *temp;
        temp = automate->complete();
        if (temp == nullptr)
            cout << "erreur" << endl;
        else {
            cout << *temp << endl;
            automate_list->push_back(temp);
            cout << "Completion reussi" << endl;
            return temp;
        }
    }
    return nullptr;
}

Automate *determinizeCompleteAndAdd(Automate *automate, vector<Automate *> *automate_list) {

    if (automate->isDeterministic(true)) {
        if (automate->isComplete(true)) {
            cout << "Automate deja determinise et complet" << endl;
            return automate;
        } else {
            return completionAndAdd(automate, automate_list);
        }
    } else {
        Automate *temp;
        temp = automate->determinize();
        automate_list->push_back(temp);
        cout << "Determinisation reussi" << endl;
        auto temp_c = completionAndAdd(temp, automate_list);
        if (temp_c)
            return temp_c;
        else
            return temp;
    }
}

void standardizeAndAdd(Automate *automate, vector<Automate *> *automate_list) {
    if (automate->isStandard(true)) {
        cout << "Automate deja standard" << endl;
    } else {
        Automate *temp;
        temp = automate->standardisation();
        cout << *temp << endl;
        automate_list->push_back(temp);
        cout << "Standardisation reussi" << endl;
    }
}

Automate *minimizeAndAdd(Automate *automate, vector<Automate *> *automate_list) {
    Automate *temp;
    temp = automate->minimize();
    if (temp != nullptr) {
        cout << *temp << endl;
        automate_list->push_back(temp);
        cout << "Minimisation reussi" << endl;
    }
    return temp;
}

Automate *complementarizationAndAdd(Automate *automate, vector<Automate *> *automate_list) {
    Automate *temp;
    temp = automate->complementarize();
    if (temp != nullptr) {
        cout << *temp << endl;
        automate_list->push_back(temp);
        cout << "Complementarisation reussi" << endl;
    } else
        cout << "Impossible, l'automate doit être complet" << endl;
    return temp;
}

void wordReco(vector<Automate *> *automate_list) {
    Automate *temp = chooseAf(automate_list);
    if (temp != nullptr) {
        cout << *temp << endl;
        string myWord;
        do {
            cout << "entrer un mot ('exit' pour quitter): ";
            cin >> myWord;
            if (temp->recognizeWord(myWord))
                cout << "Le mot est reconnu" << endl;
            else if (myWord != "exit")
                cout << "Le mot n'est pas reconnu" << endl;
        } while (myWord != "exit");
    }
}

void executeAll(vector<Automate *> *automate_list) {
    streambuf *backup;
    backup = cout.rdbuf();

    auto temp_list = new vector<Automate *>;
    auto *to_execute = chooseAf(automate_list);
    string filename = "trace/Trace " + split(to_execute->getName(), '/')[1];
    if (split(filename, '.')[1] != "txt") {
        filename += ".txt";
    }
    ofstream file(filename);
    cout.rdbuf(file.rdbuf());
    temp_list->push_back(to_execute);
    cout << *to_execute << endl;
    Automate *to_execute_d_c = determinizeCompleteAndAdd(to_execute, temp_list);
    Automate *to_execute_d_c_m = minimizeAndAdd(to_execute_d_c, temp_list);
    Automate *to_execute_d_c_m_c = complementarizationAndAdd(to_execute_d_c_m, temp_list);
    standardizeAndAdd(to_execute_d_c_m_c, temp_list);
    file.close();
    delete temp_list;

    cout.rdbuf(backup);

}

void executeAllforAll() {
    streambuf *backup;
    backup = cout.rdbuf();
    for (int i = 1; i < 45; i++) {
        string filename = ("trace/Trace B3-" + to_string(i) + ".txt");
        ofstream file(filename);
        cout.rdbuf(file.rdbuf());
        string my_file = "automate/B3-" + to_string(i) + ".txt";
        auto temp_list = new vector<Automate *>;
        auto *to_execute = new Automate(my_file);
        temp_list->push_back(to_execute);
        cout << *to_execute << endl;
        Automate *to_execute_d_c = determinizeCompleteAndAdd(to_execute, temp_list);
        Automate *to_execute_d_c_m = minimizeAndAdd(to_execute_d_c, temp_list);
        Automate *to_execute_d_c_m_c = complementarizationAndAdd(to_execute_d_c_m, temp_list);
        standardizeAndAdd(to_execute_d_c_m_c, temp_list);
        delete to_execute;
        delete temp_list;
        file.close();
    }
    cout.rdbuf(backup);

}