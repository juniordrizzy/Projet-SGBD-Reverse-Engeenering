

#define BALISE_DEBUT '<'
#define BALISE_FIN '>'
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include "fichierBalises.h"

struct fichierBalises {
    FILE *fic;
    int position; //dernière position lue dans le fichier.
};


//fichierBalisesOuvre:
//retourne NULL dans deux cas: soit il est impossible d'ouvrir le
//fichier nom_fichier, soit l'allocation de mémoire a échoué.
fichierBalises fichierBalisesOuvre(char * nom_fichier) {

    fichierBalises fib = (fichierBalises)malloc(sizeof(struct fichierBalises));
    if(!fib) {
        return NULL;
    }

    if(!(fib->fic = fopen(nom_fichier, "r"))) {
        free(fib);
        return NULL;
    } else {
        fib->position = 0;
        return fib;
    }
}

void fichierBalisesFerme(fichierBalises fichier) {

    assert(fichier != NULL && "fichier est NULL");

    fclose(fichier->fic);
    free(fichier);
    return;
}   


//fichierBalisesLit:
//retourne une structure TypeInfo contenant soit la prochaine balise,
//soit le prochain texte. Si la balise est de type DIRECTIVE ou COMMENTAIRES,
//on lit le prochain extrait du fichier. Pour ce faire, j'utilise un appel
//récursif à fichieBalisesLit. La fonction retourne NULL si une
//allocation de mémoire échoue.
Info fichierBalisesLit(fichierBalises fichier) {

    assert(fichier !=NULL && "fichier est NULL");
    Info info;
    Chaine prochaine;   
    int prochainCar,    
        verif;          //Caractère de fin de texte ou balise.

    if(!(info = (Info)malloc(sizeof(struct info)))) {
        return NULL;
    }

    //On s'assure que le fichier est placé à la dernière position lue.
    fseek(fichier->fic, fichier->position*sizeof(char), SEEK_SET);
        
    //On obtient le prochain caractère qui n'est pas un blanc.
    do {
        prochainCar = fgetc(fichier->fic);
        fichier->position++;
    } while (isspace(prochainCar));

    if(prochainCar == EOF) {
        free(info);
        return NULL;
    }else if (prochainCar == BALISE_DEBUT) {
        verif = BALISE_FIN;
        info->type = BALISE;
    } else {
        verif = BALISE_DEBUT;
        info->type = TEXTE;
    }

    if(!(prochaine = chaineCreeVide())) {
        free(info);
        return NULL;
    }

    chaineAjoute(prochaine, (unsigned char)prochainCar);
        
    //On rempli la chaine. Le seul moment où on n'ajoute pas à la chaine est
    //lors de la lecture d'un '<' avec un TypeInfo TEXTE.
    do {    
        prochainCar = fgetc(fichier->fic);
        if((prochainCar == verif && info->type == BALISE) || prochainCar != verif) {
            if(!chaineAjoute(prochaine,(unsigned char)prochainCar)){
                //problème d'ajout à la chaine.
                free(prochaine);
                free(info);
                return NULL;
            }
            fichier->position++;
        }
    } while(prochainCar != verif && prochainCar != EOF);
        
    if(info->type == TEXTE) {
        info->contenu.texte = prochaine;
    } else {
        //analyse de la balise
        info->contenu.balise = baliseCree(prochaine);
        chaineSupprimme(prochaine);
    }

    if(info->type == BALISE && (baliseLitType(info->contenu.balise) == COMMENTAIRES ||                       baliseLitType(info->contenu.balise) == DIRECTIVE)) {

        baliseSupprimme(info->contenu.balise);
        free(info);
        info = fichierBalisesLit(fichier);                    
    }
    return info;
}

