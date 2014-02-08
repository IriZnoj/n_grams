/*
 *	Predmet:
 *		Bc prace - Implementace efektivních datových struktur
 *	Autor:
 *		Jiri Znoj
 *	Zadani:
 *		Naimplementujte vhodné datové struktury použitelné pro efektivní
 *		vyhledávání v datech. Tato data mohou být napøíklad n-gramy extrahované
 *		z textù, nebo sekvence DNA. Následnì tyto datové struktury otestujte na
 *		vhodné datové kolekci a srovnejte výsledky. Pro implementaci použijte
 *		programovací jazyk C, nebo C++.
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