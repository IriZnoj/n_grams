/*
 *	Predmet:
 *		Bc prace - Implementace efektivn�ch datov�ch struktur
 *	Autor:
 *		Jiri Znoj
 *	Zadani:
 *		Naimplementujte vhodn� datov� struktury pou�iteln� pro efektivn�
 *		vyhled�v�n� v datech. Tato data mohou b�t nap��klad n-gramy extrahovan�
 *		z text�, nebo sekvence DNA. N�sledn� tyto datov� struktury otestujte na
 *		vhodn� datov� kolekci a srovnejte v�sledky. Pro implementaci pou�ijte
 *		programovac� jazyk C, nebo C++.
 */


/* Pouziti:
 *		./n_grams < vstupni_soubor
 *		./n_grams
 */



#include <stdio.h>
#include <stdlib.h>

#include "Bstrom.h"

using namespace std;

int main(){
	//deklarace promennych
	char vstup[MAX_SLOVO];
	Bstrom Strom;
	/* jednotlive zaznamy oddeleny bilym znakem
	scanf("%s", vstup);
	//printf("ukladam do stromu: %s\n", vstup);
	Strom.VlozPrvniZaznam(vstup);

	while (int a = scanf("%s", vstup) >= 0){
		printf("ukladam do stromu: %s\n", vstup);
		Strom.VlozZaznam(vstup);
	}
	*/

	//zaznamy oddeleny koncem radku
	int znak = getchar();
	int pocet_pismen;
	while (znak != EOF){
		pocet_pismen = 0;

		while (znak != '\n' && znak != EOF){
			vstup[pocet_pismen++] = znak;
			znak = getchar();
		}
		vstup[pocet_pismen++] = '\0';
		//printf("ukladam do stromu: %s\n", vstup);
		Strom.VlozZaznam(vstup); 
		znak = getchar();
	}

	Strom.Vypis();
	Strom.UkazStrom();

	Strom.Vyhledej("1000");

	return 0;
}