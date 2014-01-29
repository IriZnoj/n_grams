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