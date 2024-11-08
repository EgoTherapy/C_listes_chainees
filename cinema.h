#ifndef __CINEMA_H__
#define __CINEMA_H__
#define MAX_STRING_LENGTH 71                                                        //constante -> determine le nombre maximun de caracteres dans les chaines de carac.
#define NB_SALLES 14                                                                //constante -> nombre de salles
#define PRIX_3D 1.5                                                                 //constante -> prix en 3D

typedef struct List_salles
{
    int num;                                                                        //numero de la salle
    int placesDispo;                                                                //nbre de places dispo au maximum
}Salle;

typedef struct List_films
{
    char titre[MAX_STRING_LENGTH];                                                  //titre du film
    int type, duree, is3d, placesDispo, numSalle;                                   //type du film (1 = film, 2 = avant-premiere, 3 = art et essai)
                                                                                    //duree du film
    float prix;                                                                     //prix du film
    struct List_films *next;
}Film,ListeFilms;

typedef struct List_clients
{
    char nom[MAX_STRING_LENGTH], prenom[MAX_STRING_LENGTH];                         //nom et prenom du client
    int nbPlaces, numFilm;                                                          //nombre de places achetees
    struct List_clients *next;                                                      
}Client, ListeClients;
    

//prototypes de fonctions :
Film *createFilm(char *, int, int, int, int, int,float);                            //crée un objet de type Film
Client *createClient(char *,char *, int , int);                                     //crée un objet de type Client
ListeFilms *emptyFilmsList(void);                                                   //permet d'initialiser une liste de films (la rend vide)
ListeClients *emptyClientsList(void);                                               //permet d'initialiser une liste de clients/spectateurs (la rend vide)
int isEmptyFilmList(ListeFilms *);                                                  //permet de savoir si une liste de films est vide
int isEmptyClientsList(ListeClients *);                                             //permet de savoir si une liste de clients/spectateurs est vide
long lenFilmsList(ListeFilms *);                                                    //retourne le nombre d'elements que contient une liste de films
int lenClientsList(ListeClients *);                                                 //retourne le nombre d'elements que contient une liste de clients
Film *addFilmAt(Film *,char *, int , int , int , int ,float ,int ,int );            //Ajoute un film à une position précise dans la ListeFilms entree en argument
Client *addClientAt(Client *,char *,char *, int , int, int);                        //ajoute un élément à la liste de clienst/spectateurs à une position donnees en parametre
void printFilmsListe(ListeFilms *, int);                                            //imprime la liste detaillee des films dans la console
void replaceChar(char *, char, char);                                               //remplace un caractère dans une chaine de caractère
void decalChar(char *);                                                             //decale la chaine de caractère si il y a un espace devant
void introduction(Salle *, Client *, ListeFilms *, FILE *);                         //affiche le menu utilisateur du client
void introAdmin(ListeClients *, ListeFilms *, Salle *);                             //affiche le menu utilisateur de l'administrateur
void tailleDesSalles(Salle *);                                                      //Menu qui permet à l'utilisateur d'afficher les salles selon leurs capacites
float calculerPrix(int, int);                                                       //fonction qui calcule le prix d'une place de cinéma
void ventePlace(Film *, Client *,FILE *);                                           //Methode qui lance la vente de place(s)
void acheterPlace(Film *, Client *, int , int , char *, char *);                    //Methode qui permet au client d'acheter des places
void printClients(ListeClients *);                                                  //Methode qui retourne la liste detaillee de clients du cinema
float getPrixFilmAt(ListeFilms *, int);                                             //Methode qui retourne le Prix d'un film de la liste a une position precise
Client *freeClientAt(ListeClients *, int);                                          //Fonction qui supprime un client a une position precise de la liste des clients
Client *freeClientsList(ListeClients *);                                            //Foonction qui vide la liste des clients entree en parametre
void filmsListPlaceInit(ListeFilms *,int , int );                                   //Fonction qui initialise les places de film restante selon les clients/ spectateurs encodes


#endif