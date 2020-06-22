// Prototype des fonctions utilisée

#ifndef PROTOTYPE_HEADER
#define PROTOTYPE_HEADER

#include "data_structures.h"

/// Execute une action
/// \param automate_list liste des automates
/// \param action id de l'action
void actionMenu(int action, vector<Automate *> *automate_list);

/// Charge un nouvel automate et l'ajoute à la liste
/// \param automate_list listes des automates
void createAutomateFromFiles(vector<Automate *> *automate_list);

/// menu de selection d'un automate
/// \param automate_list listes des automates
/// \return pointeur vers l'automate choisi
Automate *chooseAf(vector<Automate *> *automate_list);

/// Demande le numéro de l'automate avec saisie sécurisée
/// \return un int contenant le numéro de l'automate choisi
int askNumber();

/// Affiche le menu d'action sur l'automate
/// \return id de l'action
int displayActionMenu();

/// Effectue une action sur l'automate
/// \param l'id de l'action
/// \param Automate sur lequel effectuer l'action
/// \param automate_list listes des automates pour ajouter l'automate modifié
void actionAutomate(int n, Automate *automate, vector<Automate *> *automate_list);

/// Affiche si l'automate est complet, le complete sinon, puis le determinise si besoin
/// \param automate Automate sur lequel effectuer l'action
/// \param automate_list liste des automates
Automate *completionAndAdd(Automate *automate, vector<Automate *> *automate_list);

/// Affiche si l'automate est deterministe, puis le determinise si besoin
/// \param automate Automate sur lequel effectuer l'action
/// \param automate_list liste des automates
Automate *determinizeCompleteAndAdd(Automate *automate, vector<Automate *> *automate_list);

/// Affiche si l'automate est standard, puis le standardise si besoin
/// \param automate Automate sur lequel effectuer l'action
/// \param automate_list liste des automates
void standardizeAndAdd(Automate *automate, vector<Automate *> *automate_list);

/// minimise un automate
/// \param automate Automate sur lequel effectuer l'action
/// \param automate_list liste des automates
Automate *minimizeAndAdd(Automate *automate, vector<Automate *> *automate_list);

/// complementarise un automate
/// \param automate Automate sur lequel effectuer l'action
/// \param automate_list liste des automates
Automate *complementarizationAndAdd(Automate *automate, vector<Automate *> *automate_list);

/// reconnaissance de mot
/// \param automate_list liste des automates
void wordReco(vector<Automate *> *automate_list);

/// Execute toutes les actions sur un automate dans un fichier
/// \param automate_list liste d'automate
void executeAll(vector<Automate *> *automate_list);

/// Execute toutes les actions sur tous les automates dans un fichier
/// \param liste d'automate
void executeAllforAll();


/// Genère un ID à partir d'une liste d'état
/// \param sameStates liste d'état
/// \return ID généré
extern string concatenateId(vector<State *> sameStates);

/// Vérifie le nombre d'état initiaux
/// \param list liste d'état à vérifier
static void oneInitialState(const vector<State *> &list);

/// Vérifie si chacune des transition est unique
/// \param list liste d'état à vérifier
/// \param alphabet alphabet de l'automate
static void uniqueTransition(const vector<State *> &list, vector<char> alphabet);


vector<State *> readChar(char c, vector<State *> current);


/// Split une chaine en un vecteur de chaine
/// \param s chaine
/// \param c caractère d'arrêt
/// \return vecteur de chaine
vector<string> split(const string &s, const char &c);

/// Fonction qui determine si un fichier est vide
bool is_empty_file(ifstream &pFile);

/// Crée des partitions jusqu'à ce que le nombre d'augmente plus
/// \param initial_partition partition initiale
/// \param alphabet alphabet de l'automate
/// \return partition finale
static Partition partitioning(Partition initial_partition, const vector<char> &alphabet, int n);

/// transforme une partition en une liste d'état
/// \param partition Partition
/// \param alphabet alphabet de l'automate
/// \param correspondance table de transition de l'automate
/// \return liste d'état
static vector<State *> *partitionToState(Partition partition, vector<char> &alphabet, stringstream &correspondance);

/// Trouve le sous_part d'un état
/// \param prec_partition partiton précédente
/// \param state_exits sorties de l'état
/// \param alphabet alphabet de l'automate
/// \return sous_part de l'état
static vector<int> *getPart(Partition prec_partition, vector<Transition *> &state_exits, const vector<char> &alphabet);


/// Vérfie si deux sous_part sont égaux
/// \return bool
static bool isSamePart(vector<int> &part_1, vector<int> &part_2);

/// Affiche une partition et ses groupes de sous_part
/// \param partition partition à afficher
/// \param n numéro de la partition, -1 pour la dernière
/// \param alphabet alphabet de l'automate
static void printPartition(Partition partition, int n, vector<char> alphabet);

/// Lis une chaine jusqu'à un point
/// \param id chaine à lire
/// \return chaine lue
static string readingPartID(string &id);

/// compare deux état
/// \return Bool (st1 < st2)
static bool isIDSmaller(const State *st1, const State *st2);

/// compare deux transitions
/// \return Bool (t1 < t2)
static bool isTransitionSmaller(const Transition *t1, const Transition *t2);


#endif //PROTOTYPE_HEADER
