#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>

void xmlTrace(char *xml) {
    char *filename;
    xmlDoc         *document;
    xmlNode        *root, *first_child, *node;
    xmlChar        *val;
    filename = xml;
    xmlNode *cur_node = NULL;
    document = xmlReadFile(filename, NULL, 0);
    //document = xmlParseFile(filename);
    root = xmlDocGetRootElement(document);
    fprintf(stdout, "La racine est de :  <%s> (%i)\n", root->name, root->type);
    first_child = root->children;
    printf("La liste entites et les relations: \n");
    for (node = first_child; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            val= xmlNodeGetContent(cur_node);
            fprintf(stdout, "\t<%s> (%i)\n", node->name, node->type);
        }
    }
    fprintf(stdout, "...\n");
    xmlFreeDoc(document);

}
void jsonTrace() {

}
