/* pile.c

   Module gérant une pile de pointeurs de type void *, implementee
   sous la forme d'une machine abstraite.

   Emmanuel Chieze, UQAM, INF3135, A12, TP2
*/

#include <assert.h>
#include "pile.h"

#define MAX_ELEMS 10000

static unsigned nbElements = 0;
static void * elems[MAX_ELEMS];

void pileEmpiler(void * x) {
	assert (nbElements < MAX_ELEMS && "pile pleine");

	elems[nbElements++] = x;
}

void * pileDepiler(void) {
	assert (nbElements > 0 && "pile vide");

	return elems[--nbElements];
}

unsigned pileTaille(void) {
	return nbElements;
}
