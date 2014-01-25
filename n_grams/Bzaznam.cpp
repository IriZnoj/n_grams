#include <stdlib.h>
#include <stdio.h>
#include "Bzaznam.h"

Bzaznam::Bzaznam(char *text){
	for (int i = 0; i < MAX_SLOVO; i++){
		this->text[i] = text[i];
	}
	Soused = NULL;
}