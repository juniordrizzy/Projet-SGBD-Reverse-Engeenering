

#define TEXTE_PAS 10    
#include "chaine.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//Longueur indique le nombre de caractères dans la chaine, en comprenant le \0.
struct chaine {
    char *texte;
    unsigned longueur;
    unsigned longueurMax;
};


Chaine chaineCreeVide() {

    Chaine chaine = (Chaine)malloc(sizeof(struct chaine));
    if(!chaine) {
        return NULL;
    }

    chaine->texte = (char *)malloc(TEXTE_PAS*sizeof(char));
    if(!chaine->texte) {
        free(chaine);
        return NULL;
    }

    //Insère \0 pour la fin de la chaine.
    chaine->texte[0] = '\0';
    chaine->longueur = 1;
    chaine->longueurMax = TEXTE_PAS;

    return chaine;
}

Chaine chaineCreeCopie(char * ch, unsigned n) {
    
    assert(n <= (strlen(ch)) && "n plus grand que la longeur de ch");
    
    Chaine chaine = (Chaine)malloc(sizeof(struct chaine));
    if(!chaine)
        return NULL;

    chaine->texte = (char *)malloc((n+1)*sizeof(char)); //+1 pour \0
    if(!chaine->texte) {
        free(chaine);
        return NULL;
    }

    strncpy(chaine->texte, ch, n);

    //On s'assure que la chaine se termine par \0
    chaine->texte[n] = '\0';

    chaine->longueur = chaine->longueurMax = n+1;

    return chaine;
}

//J'utilise une chaine temporaire pour éviter de perdre la chaine passée en
//argument si le realloc ne fonctionne pas. Le seul cas où la fonction
//retourne faux est si la Chaine est pleine et que realloc échoue.
int chaineAjoute(Chaine chaine, unsigned char c) {

    assert(chaine != NULL  && "Erreur: chaine NULL");

    char * temp; //Pour réalloc: on ne perd pas de données en cas d'échec

    if(chaine->longueur >= chaine->longueurMax) {
        temp = (char *)realloc(chaine->texte, 
                (chaine->longueurMax + TEXTE_PAS)*sizeof(char));
        
        if(!temp)
            return 0; //Problème avec realloc

        chaine->texte = temp;
        chaine->longueurMax += TEXTE_PAS;
    }
    chaine->texte[chaine->longueur-1] = c;
    chaine->texte[chaine->longueur++] = '\0';
    return 1;
}

//La fonction retourne NULL si l'allocation de mémoire a échoué.
char * chaineValeur(Chaine chaine) {

    assert(chaine != NULL  && "Erreur: chaine NULL");

    char * retour = (char*)malloc((chaine->longueur)*sizeof(char));
    
    if(!retour) {
        return NULL;
    }

    strcpy(retour, chaine->texte);

    return retour;
}

//Retourne le nombre de caractères de la chaine, sans compter le
//caractère de fin de chaine.
unsigned chaineLongueur(Chaine chaine) {
    assert(chaine != NULL  && "Erreur: chaine NULL");
    return chaine->longueur-1;
}

void chaineSupprimme(Chaine chaine) {
    assert(chaine != NULL  && "Erreur: chaine NULL");

    free(chaine->texte);
    free(chaine);
    return;
}

