#pragma once

#include <stdlib.h>
#include <stdio.h>

//maximalni velikost zaznamu
static const int MAX_SLOVO = 30;

class Bzaznam{
	public:
		char text[MAX_SLOVO];
		Bzaznam *Soused;

		Bzaznam(char *text);
};