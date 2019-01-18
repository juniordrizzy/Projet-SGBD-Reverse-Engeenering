#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <sys/stat.h>



// main
int main(int argc, char** argv)
{
// Syntaxe : XJ_Convertor [-i xml/json] [-t ][-h url_FluxHTTP] [-f FichierInput] -o nomFichier.svg
// verif commande
if(argc!=8&&argc!=7) {
	printf("erreur, arguments en surplus ou manquants \n");
	return(1) ;
}
		//se documenter sur strcmp
if(argc==8) { //l'option -t est presente
	 if ( strcmp(argv[1],"-i")!=0 ) { printf("erreur definition input \n") ; return(1) ;}
	 if ( strcmp(argv[3],"-t")!=0 ) { printf("erreur trace \n"); return(1) ;} 
	 if ( strcmp(argv[4],"-f")!=0 && strcmp(argv[4],"-h")!=0 ) { printf("erreur fichierinput \n"); return(1) ;}
	 if ( strcmp(argv[6],"-o")!=0 ) { printf("erreur output \n"); return(1) ; }
	 if ( strcmp(argv[2],"xml")!=0 && strcmp(argv[2],"json")!=0 ) { printf("erreur definition input : xml ou json \n") ; return(1) ;} 
}

if(argc==7) { //l'option -t est absemte
	 if ( strcmp(argv[1],"-i")!=0 ) { printf("erreur definition input \n"); return(1) ; }
	 if ( strcmp(argv[3],"-f")!=0 && strcmp(argv[3],"-h")!=0 ) { printf("erreur fichierinput \n"); return(1) ;}
	 if ( strcmp(argv[5],"-o")!=0 )  { printf("erreur output \n"); return(1) ;}
	 if ( strcmp(argv[2],"xml")!=0 && strcmp(argv[2],"json")!=0 ) { printf("erreur definition input : xml ou json \n") ; return(1) ;} 
}	
// if ( strcmp(argv[7],"t")==0) { //afichage trace }

//verifier si http= fichier rek pour combiner if
//json pars
if(strcmp(argv[2],"json")==0 ) {
	if(argc==8) { //l'option -t est presente
		if ( strcmp(argv[4],"-f")==0 ) { //fichier en input
			char commande1[256] , commande2[256];
			snprintf(commande1, 256, "valid-extract_json/./valid2_json %s > generation_json", argv[5]); //envoie les données dans une chaine de caractères
			int status = system(commande1); if(status!=0) return(1);
			//affichage trace
			snprintf(commande2, 256, "valid-extract_json/./valid2_json %s", argv[5]);
			system(commande2); 
		}
		if ( strcmp(argv[4],"-h")==0 ) { //fichier en input
			char commande1[256] , commande2[256];
			snprintf(commande1, 256, "valid-extract_json/./valid2_json %s > generation_json", argv[5]);
			int status = system(commande1); if(status!=0) return(1);
			//affichage trace
			snprintf(commande2, 256, "valid-extract_json/./valid2_json %s", argv[5]);
			system(commande2); 
		}
	}
	if(argc==7) { //l'option -t est absente
		if ( strcmp(argv[3],"-f")==0 ) { //fichier en input
			char commande1[256] ;
			snprintf(commande1, 256, "valid-extract_json/./valid2_json %s > generation_json", argv[4]);		
			int status = system(commande1); if(status!=0) return(1);
		}
		if ( strcmp(argv[3],"-h")==0 ) { //url en input		
			char commande1[256] ;
			snprintf(commande1, 256, "valid-extract_json/./valid2_json %s > generation_json", argv[4]);		
			int status = system(commande1);	if(status!=0) return(1);		
		} 
	}	 
}
//xml
// printf("%d",status);  on remarque que status prend 0 en cas de non erreur, gerer les if plus tard
if(strcmp(argv[2],"xml")==0 ) {  
	if(argc==8) { //l'option -t est presente
		if ( strcmp(argv[4],"-f")==0 ) { //fichier en input
			char commande1[256] , commande2[256];
			snprintf(commande1, 256, "valid-extract_xml/./valid_xml %s > generation_xml", argv[5]);		
			int status = system(commande1); if(status!=0) return(1);
			//affichage trace
			snprintf(commande2, 256, "valid-extract_xml/./valid_xml %s", argv[5]);
			system(commande2); 
			
		}
		if ( strcmp(argv[4],"-h")==0 ) { //url en input		
			char commande1[256] , commande2[256];
			snprintf(commande1, 256, "valid-extract_xml/./valid_xml %s > generation_xml", argv[5]);		
			int status = system(commande1); if(status!=0) return(1);
			//affichage trace
			snprintf(commande2, 256, "valid-extract_xml/./valid_xml %s", argv[5]);
			system(commande2);
		} 
	}
	if(argc==7) { //l'option -t est absente
		if ( strcmp(argv[3],"-f")==0 ) { //fichier en input
			char commande1[256];
			snprintf(commande1, 256, "valid-extract_xml/./valid_xml %s > generation_xml", argv[4]);			
			int status = system(commande1); if(status!=0) return(1);
		}
		if ( strcmp(argv[3],"-h")==0 ) { //url en input		
			char commande1[256];
			snprintf(commande1, 256, "valid-extract_xml/./valid_xml %s > generation_xml", argv[4]);			
			int status = system(commande1);	if(status!=0) return(1);	
		} 
	}
}

// svg


}// fin main
