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

#include"strom.h"

using namespace std;

int main(){

	//deklarace promennych
	strom x;

	char vstup[30];

	while (int a = scanf("%s", vstup) >= 0){
		printf("%s\n", vstup);
	}

	return 0;
}