

//Chaine des caractères interdits dans le nom d'une balise xml
//selon Wikipedia
#define VERIF_NOM "!\"#$%&'()*+,/;<=>?@[\\]^`{|}~, "
#define MESS_ERR_VERIF "Erreur lors de la vérification d'un champ\n."
#define MESS_ERR_CHAMP "Erreur: nom de balise mal formé.\n"

#include "balise.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct balise {
    Chaine nom;
    Chaine attribut;
    TypeBalise type;
};

//obtenirType:
//Retourne le type de balise de la chaine, selon l'énumération TypeBalise.
TypeBalise obtenirType(char *nom);

//verifierChamp:
//Verifie qu'un champ d'une balise contient pas de caractères illégaux. 
//Retourne faux s'il y une erreur, vrai sinon. Si l'allocation de mémoire
//ne fonctionne pas, retourne -1.
int verifierChamp(Chaine chaine, char *carInterdits);

//baliseCree:
//La fonction obtient le champ balise->type, et les champs balise->nom et
//balise->attribut si nécéssaire, selon le type de balise. Elle utilise
//la fonction strtok pour obtenir le nom et les attributs. Si l'allocation
//de mémoire ne fonctionne pas à une certain moment de la fonction, elle
//retourne alors NULL.
Balise baliseCree(Chaine nom) {

    Balise balise;
    char * jeton;                           //pour strtok
    char * attributs;                       //pour les attributs, si nécessaire
    char * valeurNom;
    int champ;

    if(!(balise = (Balise)malloc(sizeof(struct balise)))) {
        return NULL;
    }
    if(!(valeurNom = chaineValeur(nom))) {
        free(balise);
        return NULL;
    }

    //Vérification qu'il s'agit bien d'une balise et nom d'une chaine
    //quelconque.
    if(valeurNom[0] != '<' || valeurNom[chaineLongueur(nom)-1] != '>') {
        free(valeurNom);
        free(balise);
        return NULL;
    }

    balise->nom = balise->attribut = NULL;
    balise->type = obtenirType(valeurNom);
   
    if (balise->type == FIN) {
        //Trouver le nom à l'aide de jetons.
        jeton = strtok(&valeurNom[2], ">");
        if(!(balise->nom = chaineCreeCopie(jeton, strlen(jeton)))) {
            free(balise);
            free(valeurNom);
            return NULL;
        }
    } else if (balise->type == DEBUT || balise->type == DEBUTFIN) {
        jeton = strtok(&valeurNom[1], " >");
        if(!(balise->nom = chaineCreeCopie(jeton, strlen(jeton)))) {
            free(balise);
            free(valeurNom);
            return NULL;
        }
         
        //Traitement des attributs, si nécessaire.
        if((jeton = strtok(NULL, " >")) != NULL) {

            attributs = (char *)malloc(chaineLongueur(nom)*sizeof(char));
            if(!attributs) {
                baliseSupprimme(balise);
                return NULL;
            }
            attributs[0] = '\0';
            while (jeton != NULL) {
                strcat(attributs, jeton);
                strcat(attributs, " \0");
                jeton = strtok(NULL, " >");
            }
            if(balise->type == DEBUTFIN) {
                //Pour enlever le '/' d'attributs
                attributs[strlen(attributs)-2] = '\0';
            }
            free(attributs);
            if(!(balise->attribut = chaineCreeCopie(attributs, strlen(attributs)))) {
                baliseSupprimme(balise);
                return NULL;
            }
        }    
    } 
    
    //Vérifie que le nom est bien formé, si nécessaire, et qu'il
    //n'est pas NULL
    if(balise->type != COMMENTAIRES && balise->type != DIRECTIVE) {

        assert(balise->nom != NULL && "Nom de balise NULL");
        champ = verifierChamp(balise->nom, VERIF_NOM);
        if(champ < 0) {
            fprintf(stderr, "%s",MESS_ERR_VERIF);
        } else if (champ == 0) {
            fprintf(stderr, "%s",MESS_ERR_CHAMP);
        }
    }

    free(valeurNom);
    return balise;
} //fin baliseCree


Chaine baliseLitNom(Balise balise) {

    assert(balise != NULL && "balise est NULL.");
    assert(baliseLitType(balise) != DIRECTIVE && baliseLitType(balise) 
            != COMMENTAIRES && "Mauvais type de balise.");
    char * temp = chaineValeur(balise->nom);
    Chaine nomBalise = chaineCreeCopie(temp, chaineLongueur(balise->nom));
    free(temp);
    if(nomBalise == NULL) {
        return NULL;
    }
    return nomBalise;
}

TypeBalise baliseLitType(Balise balise) {
    
    assert(balise != NULL && "balise est NULL.");

    return balise->type;
}
 
Chaine baliseLitAttributs(Balise balise) {

    assert(balise != NULL && "La balise est nulle.");
    assert(baliseLitType(balise) != DIRECTIVE && baliseLitType(balise) != 
            COMMENTAIRES && "Mauvais type de balise.");

    Chaine attribut;
    char *temp;  //Pour libérer l'allocation

    //If sert ici à éviter de passer ue chaine NULL à chaineValeur.
    if(balise->attribut == NULL) {
        return NULL;
    } else {
        temp = chaineValeur(balise->attribut);
        if(!temp) {
            return NULL;
        }
        attribut = chaineCreeCopie(temp,
                    chaineLongueur(balise->attribut));
        free(temp);
        
        if(!attribut) {
            return NULL;
        }
    }
    return attribut;
}

void baliseSupprimme(Balise balise) {

    assert(balise != NULL && "La balise est nulle.");

    if(balise->nom) {
        chaineSupprimme(balise->nom);
    }
    if(balise->attribut) {
        chaineSupprimme(balise->attribut);
    }
    free(balise);
}

TypeBalise obtenirType(char * nom) {

    TypeBalise type;

    if(nom[1] == '?') {
        type = DIRECTIVE;
    } else if (nom[1] == '!') {
        type = COMMENTAIRES;
    } else if (nom[1] == '/') {
        type = FIN;
    } else if(nom[strlen(nom)-2] == '/') {
        type = DEBUTFIN;
    } else {
        type = DEBUT;
    }
    return type;
}

int verifierChamp(Chaine chaine, char *carInterdits) {

    char * valeurChamp = chaineValeur(chaine);
    int i, longueurChamp = chaineLongueur(chaine);
    if(!valeurChamp) {
        return -1;
    }

    for(i = 0; i < longueurChamp; i++) {
        if(strchr(carInterdits, valeurChamp[i]) != NULL) {
                free(valeurChamp);
                return 0;
        }
    }
    free(valeurChamp);
    return 1;

}
