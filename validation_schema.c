#include <stdio.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>
 
enum {
    ERROR_OCCURED = -1, // Une erreur est survenue pendant la validation
    NOT_VALID = 0,      // Le document n'est pas valide
    VALID = 1           // Le document est valide
};
 
/**
 * Fonction de validation d'un arbre DOM à l'aide d'un XML Schema
 **/
int validation_schema(xmlDocPtr doc, const char *xml_schema, int afficher_erreurs) {
    int ret;
    xmlSchemaPtr schema;
    xmlSchemaValidCtxtPtr vctxt;
    xmlSchemaParserCtxtPtr pctxt;

    // Ouverture du fichier XML Schema
    if ((pctxt = xmlSchemaNewParserCtxt(xml_schema)) == NULL) {
        return ERROR_OCCURED;
    }
    // Chargement du contenu du XML Schema
    schema = xmlSchemaParse(pctxt);
    xmlSchemaFreeParserCtxt(pctxt);
    if (schema == NULL) {
        return ERROR_OCCURED;
    }
    // Création du contexte de validation
    if ((vctxt = xmlSchemaNewValidCtxt(schema)) == NULL) {
        xmlSchemaFree(schema);
        return ERROR_OCCURED;
    }
    // Traitement des erreurs de validation
    if (afficher_erreurs) {
        xmlSchemaSetValidErrors(vctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
    }
    // Validation
    ret = (xmlSchemaValidateDoc(vctxt, doc) == 0 ? VALID : NOT_VALID);
    // Libération de la mémoire
    xmlSchemaFree(schema);
    xmlSchemaFreeValidCtxt(vctxt);

    return ret;
}

int main(int argc, char** argv) {
int ll;

validation_schema(argv[0],argv[1],ll);
return ll;
}
