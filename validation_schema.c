#define LIBXML_SCHEMA_ENABLED
#include<libxml/xmlschemastypes.h>
#include<stdio.h>
#include<stdlib.h>
int main()
{
xmlDocPtr doc;
xmlSchemaPtr schema = NULL;
xmlSchemaParserCtxtPtr ctxt;
char *XMLFileName = "diagramme.xml";
char *XSDFileName = "validation.xsd";

xmlLineNumbersDefault(1);

ctxt = xmlSchemaNewParserCtxt(XSDFileName);

xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc)fprintf, (xmlSchemaValidityWarningFunc)fprintf, stderr);
schema = xmlSchemaParse(ctxt);
xmlSchemaFreeParserCtxt(ctxt);
//xmlSchemaDump(stdout, schema); //To print schema dump

doc = xmlReadFile(XMLFileName, NULL, 0);

if (doc == NULL)
 {
   fprintf(stderr, "Could not parse %s\n", XMLFileName);
 } 
else 
 {
  
   xmlSchemaValidCtxtPtr ctxt;
   int ret;

   ctxt = xmlSchemaNewValidCtxt(schema);
   xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc)fprintf, (xmlSchemaValidityWarningFunc)fprintf, stderr);
   ret = xmlSchemaValidateDoc(ctxt, doc);
   if (ret == 0)
    {
       printf("%s validates\n", XMLFileName);
    }
    else if (ret > 0)
    {
       printf("%s fails to validate\n", XMLFileName);
    }
    else
    {
       printf("%s validation generated an internal error\n", XMLFileName);
    }
    xmlSchemaFreeValidCtxt(ctxt);
    xmlFreeDoc(doc);
 }
 //Free the resource
 if (schema != NULL)
	xmlSchemaFree(schema);

 xmlSchemaCleanupTypes();
 xmlCleanupParser();
 xmlMemoryDump();

 return (0);
}
