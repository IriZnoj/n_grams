#include "Bzaznam.h"

class Bzaznam{

	//text je klic - chtelo by to asi hash funkci...
	char text[MAX_SLOVO];
	Bzaznam *Soused;
	Bstrom *LPotomek;
	Bstrom *RPotomek;

	Bzaznam(char *text){
		for (int i = 0; i < MAX_SLOVO; i++){
			this->text[i] = text[i];
		}
		Soused = NULL;
		LPotomek = NULL;
		RPotomek = NULL;
	}

};