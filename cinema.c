#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cinema.h"


int main(int argc, char const *argv[])
{
    //declarations des variables
    char titreFilm[MAX_STRING_LENGTH], nom[MAX_STRING_LENGTH], prenom[MAX_STRING_LENGTH];
    int typeFilm, dureeFilm,is3d ,i, numsalle, nbPlaces, choixFilm, numFilm,j, choix;
    Salle s[NB_SALLES+1];
    //declarations et initialisations des listes
    ListeFilms *films = emptyFilmsList();
    ListeClients *clients = emptyClientsList();

    //ouverture des fichiers
    FILE *fdatF,*fdatS , *fdatCl;
    fdatF = fopen("cinema01.dat","r");
    fdatS = fopen("cinema02.dat","r");
    fdatCl = fopen("cinema03.dat","r+");
    
    //LECTURE SALLES =============================================================================================================================
    for ( i = 1; i <= NB_SALLES; i++)
    {
        fscanf(fdatS,"%2d%3d",&s[i].num, &s[i].placesDispo);
    }
    
    
    
    //LECTURE FILMS =============================================================================================================================
    //
    i=0;
    //lecture anticipee
    fscanf(fdatF,"%1d%1d%3d%2d%41s",&typeFilm,&is3d,&dureeFilm,&numsalle,&titreFilm);
    replaceChar(titreFilm,'_',' ');
	films = addFilmAt(films,titreFilm,is3d,dureeFilm, typeFilm, s[numsalle].placesDispo, numsalle,calculerPrix(dureeFilm, is3d),i);
    while (!feof(fdatF))
    {
        fscanf(fdatF,"%1d%1d%3d%2d%41s",&typeFilm,&is3d,&dureeFilm,&numsalle,&titreFilm);
        replaceChar(titreFilm,'_',' ');
        i++;
	    films = addFilmAt(films,titreFilm,is3d,dureeFilm, typeFilm, s[numsalle].placesDispo, numsalle,calculerPrix(dureeFilm, is3d),i);
    }
    
    //lecture clients============================================================================================================================
    i = 0;
    fscanf(fdatCl,"%s %s %1d %2d", &prenom, &nom, &nbPlaces, &choixFilm);
	filmsListPlaceInit(films,nbPlaces,choixFilm);
    clients = addClientAt(clients,nom, prenom,nbPlaces,choixFilm, i);
    while (!feof(fdatCl))
    {
        i++;
        fscanf(fdatCl,"%s %s %1d %2d", &prenom, &nom, &nbPlaces, &choixFilm);
		filmsListPlaceInit(films,nbPlaces,choixFilm);
        clients = addClientAt(clients,nom, prenom,nbPlaces,choixFilm, i);
    }
    //entree dans le programme
    printf("***********************************\n******* Bienvenue au Cinema *******\n***********************************\n\n\n");

    printf("1) Acceder en tant que client\n2) Acceder en tant qu'administrateur\n3) Quitter le programme\n\nVotre choix ? (1-3)");
    scanf("%d",&choix);
    while (choix <= 2  && choix > 0)
    {
        switch (choix)
        {
        case 1:
            printf("***********************************\n*********** MENU CLIENT ***********\n***********************************\n\n");
            introduction(s,clients,films,fdatCl);
            break;
        
        case 2:
            introAdmin(clients,films,s);
            break; 

        default:
            break;
        }
    printf("1) Acceder en tant que client\n2) Acceder en tant qu'administrateur\n3) Quitter le programme\n\nVotre choix ? (1-3)");
    scanf("%d",&choix);
    }

    
    printf("*****************************************\n******* Merci Pour votre visite ! *******\n*****************************************\n\n\n");
    
    
    
    //=========================================================================================================================
    remove(fdatCl);
    fdatCl = fopen("spectateurs.res","r+");
    while (clients != NULL)
    {
        fprintf(fdatCl,"%-s %-s %1d %02d\n", clients->prenom, clients->nom,clients->nbPlaces, clients->numFilm);
        clients = clients->next;
    } 
    return 0;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------

Film *createFilm(char *titre, int is3d, int duree,int type, int places, int numSalle,float prix)
{
    Film *film = malloc(sizeof(Film));
    if(!film)
        return NULL;
    strcpy(film->titre, titre);
    film->is3d = is3d;
    film->type = type;
    film->duree = duree;
    film->placesDispo = places;
    film->prix = prix;
    film->numSalle = numSalle;
    film->next = NULL;
    return film;
}

Client *createClient(char *nom,char *prenom, int nbPlaces, int numFilm)
{
    Client *client= malloc(sizeof(Client));
    if(!client)
        return NULL;
    strcpy(client->nom, nom);
    strcpy(client->prenom, prenom);
    client->nbPlaces = nbPlaces;
    client->numFilm = numFilm;
    client->next = NULL;
    return client;
}

// Fonctions d'initialisation des listes----------------------


ListeFilms *emptyFilmsList(void)
{
    return NULL;
}

ListeClients *emptyClientsList(void)
{
    return NULL;
}

//Fonctions qui determine si une liste est vide----------------------------------------------------------------

int isEmptyFilmList(ListeFilms *L)
{
    return L == NULL;
}

int isEmptyClientsList(ListeClients *L)
{
    return L == NULL;
}

//Fonction qui retourne le nombre d'elements que contient une liste----------------------------------------


long lenFilmsList(ListeFilms *L)
{
    long len = 0;
    while (L != NULL)
    {
        len++;
        L = L->next;
    }
    
    return len;
}

void filmsListPlaceInit(ListeFilms *L,int nbPl, int choix)
{
	choix--;
    long len = 0;
    while (L != NULL)
    {
    	if(len == choix){
    		L->placesDispo -= nbPl;
		}
        len++;
        L = L->next;
    }
    
}

int lenClientsList(ListeClients *L)
{
    int len = 0;
    while (L)
    {
        len++;
        L = L->next;
    }
    
    return len;
}

// Fonctions d'ajout d'elements dans une liste, a une position definie en parametre---------------------------------------------------------------------------


//parametres : liste de type Film, titre du film, style(s) du film, type du film, position dans la liste
Film *addFilmAt(Film *L,char *titre, int is3d, int duree, int type, int places,float prix,int numSalle,int pos)
{
    Film *prec = L;
    Film *cur = L;
    int i;
    Film *film = createFilm(titre,is3d,duree,type, places,numSalle,prix);
    if(isEmptyFilmList(L))
    {
        return film;    
    }
    if(pos == 0)
    {
        film->next = L;
        return film;
    }
    i = 0;
    while (i < pos)
    {
        i++;
        prec = cur;
        cur = cur->next;
    }
    prec->next = film;
    film->next = cur;

    return L;
}


//fonction non finie
Client *addClientAt(Client *L,char *nom,char *prenom, int nbPlace, int numFilm,int pos)
{
    Client *prec = L;
    Client *cur = L;
    int i;
    Client *client = createClient(nom,prenom,nbPlace,numFilm);
    if(isEmptyClientsList(L))
    {
        return client;    
    }
    if(pos == 0)
    {
        client->next = L;
        return client;
    }
    i = 0;
    while (i < pos)
    {
        i++;
        prec = cur;
        cur = cur->next;
    }
    prec->next = client;
    client->next = cur;

    return L;
}

//print listes
void printFilmsListe(ListeFilms *L, int type)
{
    int i=0;
    if (type > 3 || type < 0)
    {
        return;
    }
    while (L != NULL)
    {
        if (L->type == type)
        {
        printf("Film %2d\n", i+1);
        printf("TITRE : %-s\n",L->titre);
        if (L->is3d == 0)
        {
            printf("3D : NON\n");
        }else{
            printf("3D : OUI\n");
        }
            
        printf("DUREE : %3d minutes\n", L->duree);
        printf("PRIX : %5.2f euros\n",L->prix);
        printf("NUMERO DE SALLE : %2d\n", L->numSalle);
        printf("PLACES RESTANTES : %3d\n\n", L->placesDispo);
        }
        i++;
        L = L->next;
    }       
  
    

    
}

//gestion chaine carac
void replaceChar(char *str, char old, char new)
{
    int i;
    int slenght = strlen(str);
    for ( i = 0; i < slenght; i++)
        if(str[i] == old)
            str[i] = new;
}

void decalChar(char *str)
{
    int slength = strlen(str), nbEspaces = 0, i;
    for ( i = 0; i < slength; i++)
    {
        if (str[i] == ' ')
        {
            nbEspaces++;
        }else{
            break;
        }
    }
    for ( i = 0; i < slength; i++)
    {
        str[i] = str[i+nbEspaces];
    }
}


//interractions utilisateur - programme


void introduction(Salle *s, Client *cl, ListeFilms *f, FILE *clts)
{
    int choix;
    printf("Que voulez vous faire? \n1) Voir les films diffuses en ce moment\n2) Voir les avants-premieres qui auront lieues prochainement\n3) Voir les evenements qui auront lieus prochainement\n4) voir les salles disponnibles\n5) Retour\n\n");
    printf("entrez le numero de votre choix :\n");
    scanf("%d",&choix);
    while (choix < 0 || choix > 5)
    {
        printf("ce choix n'existe pas.\n");
        printf("entrez le numero de votre choix :\n");
        scanf("%d",&choix);
    }
    while (choix != 5)
    {
        switch (choix)
        {
        case 1: printFilmsListe(f,1);
                ventePlace(f, cl,clts);
            break;
        case 2: printFilmsListe(f,2);
                
            break;
        case 3: printFilmsListe(f,3);
                
            break;
        case 4:
            tailleDesSalles(s);
            break;
        default:
            break;
        }
        printf("Que voulez vous faire? \n1) Voir les films diffuses en ce moment\n2) Voir les avants-premieres qui auront lieues prochainement\n3) Voir les arts et essais qui auront lieus prochainement\n4) voir les salles disponnibles\n5) Retour\n\n");
        printf("entrez le numero de votre choix :\n");
        scanf("%d",&choix);
        while (choix < 0 || choix > 5)
        {
            printf("ce choix n'existe pas.\n");
            printf("entrez le numero de votre choix :\n");
            scanf("%d",&choix);
        }

    }
    
}



//gestions salles------------------------------------
void tailleDesSalles(Salle *s)
{
    int i, choix;
    printf("1) Afficher toutes les salles\n2) Afficher les salles des petite capacite (130 places)\n3) Afficher les salles Moyennes (200 places)\n4) Afficher les grandes salles (350 places)\n5) Quitter\n");
    scanf("%d",&choix);
    while (choix < 0 || choix > 5)
    {
        printf("Valeur incorrecte, veuillez entrer votre choix (entre 1 et 5)\n");
        scanf("%d",&choix);
    }
    
    while (choix != 5)
    {
            
            switch (choix)
            {
            case 1:
                for (i = 1; i <= NB_SALLES; i++)
                {
                    printf("Numero : %2d Nombre de places : %3d\n",s[i].num,s[i].placesDispo);
                }
                
            break;

            case 2:
                for ( i = 1; i <= NB_SALLES; i++)
                {
                    if (s[i].placesDispo < 200)
                    {
                        printf("Numero : %2d Nombre de places : %3d\n",s[i].num,s[i].placesDispo);
                    }
                }
                
                break;
            
            case 3:
                for ( i = 1; i <= NB_SALLES; i++)
                {
                    if (s[i].placesDispo < 350 && s[i].placesDispo > 130)
                    {
                        printf("Numero : %2d Nombre de places : %3d\n",s[i].num,s[i].placesDispo);
                    }
                }
                
                break;

            case 4:
                for ( i = 1; i <= NB_SALLES; i++)
                {
                    if (s[i].placesDispo >= 350)
                    {
                        printf("Numero : %2d Nombre de places : %3d\n",s[i].num,s[i].placesDispo);
                    }
                }
                
                break;

            default:
                break;
            }
            printf("1) Afficher toutes les salles\n2) Afficher les salles des petite capacité (130 places)\n3) Afficher les salles Moyennes (200 places)\n4) Afficher les grandes salles (350 places)\n5) Quitter\n");
            scanf("%d",&choix);
            while (choix < 0 || choix > 5)
            {
                printf("Valeur incorrecte, veuillez entrer votre choix (entre 1 et 5)\n");
                scanf("%d",&choix);
            }
    }

}



void ventePlace(Film *L, Client *cl,FILE *f)
{
    long nbPl, choixFilm;
    char nom[MAX_STRING_LENGTH], prenom[MAX_STRING_LENGTH], ouiNon;
    printf("Souhaitez vous acheter une/des place(s) pour un film? (Y/N)\n", nbPl);
    scanf(" %c",&ouiNon);
    if (ouiNon == 'Y' || ouiNon =='y')
    {
        printf("Quel film voulez-vous regarder ? (entrez le numero du film)\n");
        scanf("%d", &choixFilm);
        printf("Combien de places souhaitez-vous? (max 5 places)\n");
        scanf("%d", &nbPl);
        printf("confirmez-vous l'achat de %1d place(s) pour %5.2f euros? (Y/N)\n", nbPl, nbPl*getPrixFilmAt(L,choixFilm));
        scanf(" %c",&ouiNon);
        if (ouiNon == 'Y' || ouiNon =='y')
        {
            printf("Votre nom : \n");
            scanf(" %[^\n]s", &nom);
            printf("Votre prenom : \n");
            scanf(" %[^\n]s", &prenom);
            acheterPlace(L, cl, nbPl, choixFilm,nom, prenom);
        }else{
            return;
        }
    }else{
        return;
    }
}

void acheterPlace(Film *L, Client *cl, int nbPl, int choixFilm, char *nom, char *prenom)
{
    choixFilm -=1;
    long i = 0;
    while(L != NULL)
    {
        if (choixFilm == i)
        {
            cl = addClientAt(cl,nom,prenom,nbPl, choixFilm,lenClientsList(cl)-1);
            L->placesDispo -= nbPl;
            return;
        }
        i++;
        L = L->next;
    }
}

float calculerPrix(int duree, int is3d)
{
    if (is3d == 0)
    {
        return (float)duree/10.0;
    }else{
        return (float) (duree/10.0)*PRIX_3D;
    }
    
    
}   


void introAdmin(ListeClients *cl, ListeFilms *f, Salle *s)
{
    int choix;
    printf("***********************************************\n**************Menu administrateur**************\n***********************************************\n\n");
    printf("1) Gerer les films\n2) Gerer les clients\n3) Gerer les salles\n4) Retour\n\n");
    printf("Votre choix? (1-4)\n");
    scanf("%d",&choix);
    while (choix > 0 && choix < 4)
    {
        switch (choix)
        {
        case 1:
                printf("-------------- Films ------------------\n");
                printFilmsListe(f,1);
                printf("-------------- Avant-premieres ------------------\n");
                printFilmsListe(f,2);
                printf("-------------- arts et essais ------------------\n");
                printFilmsListe(f,3);
                break;
        case 2 : 
                printClients(cl);
                break;
        case 3 :
                tailleDesSalles(s);
                break;
        default:
            break;
        }



        printf("1) Gerer les films\n2) Gerer les clients\n3) Gerer les salles\n4) Retour\n\n");
        printf("Votre choix? (1-4)\n");
        scanf("%d",&choix);
    }
    


}

void printClients(ListeClients *L)
{
    int i = 0, choix, choixCl;
    printf("NUM          PRENOM          NOM             ID. FILM            NB. PLACES\n");
    printf("---------------------------------------------------------------------------\n");
    while (L != NULL)
    {   
        i++;
        printf("%d      %-21s   %-21s   %d               %d\n", i ,L->prenom, L->nom, L->numFilm, L->nbPlaces);

        L = L->next;
    }
    //--------
    printf("\n1) Supprimer un client\n2) supprimer toute la liste\n3) retour\n\n");
    printf("\nVotre choix ?\n");
    scanf("%d", &choix);
    if (choix <= 2){
        switch (choix)
        {
        case 1:
            printf("Entrez le numero du client que vous voulez supprimer : \n");
            scanf("%d", &choixCl);
            choixCl--;
            L = freeClientAt(L,choixCl);
            printf("Le client %d a bien ete supprime de la liste\n",choixCl+1);
            break;
        case 2 :
            L = freeClientsList(L);
            printf("Toute la liste de client a ete supprimée\n\n");
            break;
        default:
            break;
        }
    }

}

Client *freeClientsList(ListeClients *L)
{
    ListeClients *tmp = NULL;
    while (L != NULL)
    {
        tmp = L->next;
        free(L);
        L = tmp;
    }
    return L;
}

float getPrixFilmAt(ListeFilms *L, int pos)
{
    pos--;
    int i = 0; 
    if(isEmptyFilmList(L))
    {
        return -1.0;
    }
    while (i < pos)
    {
        i++;
        L = L->next;
    }
    
    return L->prix;
}

Client *freeClientAt(ListeClients *L, int pos)
{
    Client *prec = L;
    Client *cur = L;
    int i;
    if(isEmptyClientsList(L))
    {
        return NULL;    
    }
    if(pos == 0)
    {
        L = L->next;
        free(cur);
        return L;
    }
    i = 0;
    while (i < pos)
    {
        i++;
        prec = cur;
        cur = cur->next;
    }
    prec->next = cur->next;
    free(cur);
    return L;
}

