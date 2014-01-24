#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "Bzaznam.h"

//vlikost MAX_SLOVO je nastavena v Bzaznam.h

class Bstrom{
	public:
		Bstrom(char *text);
		Bstrom(Bzaznam z);
};