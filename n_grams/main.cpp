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
	char vstup[30];
	Bstrom Strom;

	scanf("%s", vstup);
	//printf("ukladam do stromu: %s\n", vstup);
	Strom.VlozPrvniZaznam(vstup);

	while (int a = scanf("%s", vstup) >= 0){
		//printf("ukladam do stromu: %s\n", vstup);
		Strom.VlozZaznam(vstup);
	}

	//TO DO - kdyz delim list, tak zustava nejpravejsi potomek levehopotomka i nejlevejsi potomek praveho potomka
	//je to zamerne, ten nejpravejsi lze smazat, ale nicemu tam nevadi, pouze je pak ve vypisu 2x a to by se mohlo opravit
	Strom.Vypis();
	Strom.UkazStrom();

	return 0;
}