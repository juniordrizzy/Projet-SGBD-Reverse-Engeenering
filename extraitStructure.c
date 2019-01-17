/* extraitStructure.c
 
 
 * Utilisation du programme: <nom_programme> <nom_fichier>.
 *
 * Le programme affiche les balises ouvrantes d'un fichier XML/HTML.
 * Il vérifie aussi si le fichier est bien formé. Si le fichier est
 * mal formé, le programme affiche un message d'erreur et arrête le
 * traitement du fichier.
 *
 * Pour le moment, voici les vérifications faites par le programme:
 * - Le fichier xml ne commence pas par une balise FIN.
 * - Le fichier ne contient pas de balises fermées dans le mauvais ordre.
 * - Toutes les balises DEBUT sont fermées à la fin du fichier.
 * - Le programme s'assure que le fichier contient au moins une balise
 *   ouvrante. S'il n'y a pas au moins une balise, ce n'est pas un fichier
 *   XML ou HTML.
 *
 *  Pour le moment, on suppose que ces situations ne se produisent pas dans
 *  les fichiers XML:
 *  - Il n'y a pas d'espace au début de la balise: par exemple toutes les
 *    balises sont du format <texte..., et non < texte... Même chose pour les
 *    balises de type DÉBUTFIN, il n'y a pas d'espace avant la fin de la 
 *    balise, donc du format .../>, et non .../ >.
 *  - Les noms et attributs sont biens formés, selon les spécifications XML.
 *    Ils ne contiennent pas de caractères illégaux.
 *  - Les fichiers XML ne sont pas extrèmement longs. Donc, la pile fournie
 *    ne sera jamais pleine, car nous n'avons pas de façon de vérifier si
 *    la pile est pleine.
 *
 *  Le programme a comme comportement, lors d'une erreur d'allocation dynamique
 *  de mémoire, d'afficher un message d'erreur sur stderr et d'arrêter le 
 *  traitement.
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "pile.h"
#include "fichierBalises.h"

#define IMBRICATION "  "
#define ERR_ARG 1	    //Erreur avec les arguments.
#define ERR_XML 2       //Erreur dans le fichier XML/HTML.
#define ERR_FICHIER 3   //Erreur lors de l'ouverture du fichier.
#define ERR_MEM 4       //Erreur d'allocation de mémoire
#define ERR_MESS_XML "Ficiher XML ou HTML mal formé.\n"
#define ERR_MESS_FICHIER "Erreur lors de l'ouverture du fichier.\n"
#define ERR_MESS_MEM "Erreur lors de l'allocation de mémoire.\n"

//afficherErreur:
//Affiche un message d'erreur approprié sur stderr pour l'erreur noErreur.
void afficherErreur(int noErreur, const char *nomFichier);

//afficherNomBalise:
//Affiche le nom de la balise avec le niveau d'imbrication approprié.
//Si l'allocation de mémoire dynamique échoue, la fonction retourne 0.
int afficherBalise(int imbrication, const Balise balise);

//obtenirNomBalise:
//Retourne une chaine char * du nom de la balise. Retourne NULL si
//l'allocation de mémoire dynamique échoue.
char *obtenirNomBalise(Balise balise);

//Libère l'allocation de mémoire pour une structure Info.
void libereInfo(Info info);

//Libère la mémoire de la pile lors d'une erreur dans le fichier, si
//nécessaire. La pile ne contient que des structures Info de type
//Balise, donc c'est le seul cas traité dans la fonction.
void viderPile(void);

int main(int argc, char *argv[]) {
   
    int erreur = 0,             //Indique si une erreur a été détectée.
        uneBalise = 0,          //Booléen: vrai si le fichier a au moins une
                                //balise ouvrante.
        resultatCompare;        //Résultat de strcmp.
    fichierBalises fichier;
    Info texte,                 //Prochaine partie du texte
         depile;                //Structure dépilée.
    int imbrication = 0;        
    TypeBalise typeBalise;
    char *compare1, *compare2;  //Pour comparer les noms des balises

    if(argc != 2){
        afficherErreur(ERR_ARG,  argv[0]);
        return ERR_ARG;
    }

    fichier = fichierBalisesOuvre(argv[1]);

    if (fichier == NULL) {
		afficherErreur(ERR_FICHIER, argv[0]);
		return ERR_FICHIER;
	}

	while ((texte = fichierBalisesLit(fichier)) != NULL) {
	    if (texte->type == BALISE) {
	        typeBalise = baliseLitType(texte->contenu.balise);
            uneBalise = 1;
	        if (typeBalise == DEBUT || typeBalise == DEBUTFIN) {
	            if(!(afficherBalise(imbrication, texte->contenu.balise))) {
                    erreur = ERR_MEM;
                    break;
                }
                if(typeBalise == DEBUT) {
                    pileEmpiler(texte);
                    imbrication++;
                } else {
                    libereInfo(texte);
                }
	        } else if (typeBalise == FIN) {
                //Vérifier si la pile est vide avant de dépiler. Si elle est
                //vide, c'est une erreur.
                if(pileTaille() == 0) {
                    libereInfo(texte);
                    erreur = ERR_XML;
                    break; 
                }
                depile = (Info)pileDepiler();

                compare1 = obtenirNomBalise(depile->contenu.balise);
                compare2 = obtenirNomBalise(texte->contenu.balise);
                libereInfo(texte);
                libereInfo(depile);
                if(!compare1 && !compare2) {
                    erreur = ERR_MEM;
                    break;
                }
                resultatCompare = strcmp(compare1, compare2);
                free(compare1);
                free(compare2);
                if(resultatCompare) {
                    //Erreur - les noms des deux balises sont
                    //différents
                    erreur = ERR_XML;
                    break;
                } else {
                    //pas d'erreur
                    imbrication--;
                }
	        }
	    } else {
            //Info de type texte, on ignore
            libereInfo(texte);
	    }
    }
    erreur = (erreur?erreur:(uneBalise?0:ERR_XML));
    fichierBalisesFerme(fichier);
    if(pileTaille() > 0  || erreur) {
        viderPile();
        afficherErreur(erreur, argv[0]);
    }
    return erreur;

}

void afficherErreur(int noErreur, const char *nomFichier) {

    printf("\n");
	fprintf(stderr, "%s: Erreur %d\n", nomFichier, noErreur);
	switch(noErreur) {
	    case ERR_ARG:     fprintf(stderr, "Mauvais arguments.\n");
                          fprintf(stderr, "Usage: %s <fichier>\n",
                              nomFichier);
                          break;
        case ERR_FICHIER: fprintf(stderr, "%s", ERR_MESS_FICHIER);
                          break;
        case ERR_XML:     fprintf(stderr, "%s", ERR_MESS_XML);
                          break;
        case ERR_MEM:     fprintf(stderr, "%s",ERR_MESS_MEM);
                          break;
	    default:	  fprintf(stderr, "Erreur inconnue.\n");
			          break;
	}
    return;
}

int afficherBalise (int imbrication, const Balise balise) {
    char * temp1; //Pour eviter des fuites de mémoire
    Chaine temp2;
    int i;
    for (i = 0;  i < imbrication; i++)  {
        printf("%s", IMBRICATION);
    }
    

    if(!(temp2 = baliseLitNom(balise))) {
        //Erreur - retourne 0
        return 0;
    } else {
        if(!(temp1 = chaineValeur(temp2))) {
            return 0;
        } else {
            printf("%s\n", temp1);
            chaineSupprimme(temp2);
            free(temp1);
        }
    }
    return 1;
}

char * obtenirNomBalise(Balise balise) {
    char * retour;
    Chaine nom = baliseLitNom(balise);
    if(!nom) {
        return NULL;
    }
    retour = chaineValeur(nom);
    chaineSupprimme(nom);
    return retour;
}

void libereInfo(Info info) {
    if(info->type == TEXTE) {
        chaineSupprimme(info->contenu.texte);
        free(info);
    } else {
        baliseSupprimme(info->contenu.balise);
        free(info);
    }

    return;
}

void viderPile(void) {

    Info info;
    while(pileTaille() != 0) {
        info = (Info)pileDepiler();
        baliseSupprimme(info->contenu.balise);
        free(info);
    }
    return;
}
