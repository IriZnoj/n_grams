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

//#include "Bstrom.h"
#include "Rstrom.h"
using namespace std;

int main(){
	/* jednotlive zaznamy oddeleny bilym znakem
	scanf("%s", vstup);
	//printf("ukladam do stromu: %s\n", vstup);
	Strom.VlozPrvniZaznam(vstup);

	while (int a = scanf("%s", vstup) >= 0){
		printf("ukladam do stromu: %s\n", vstup);
		Strom.VlozZaznam(vstup);
	}
	*/

	//--------------------Bstrom--------------------
/*	char vstup[MAX_SLOVO];
	Bstrom Strom;
	
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
		printf("ukladam do stromu: %s\n", vstup);
		Strom.VlozZaznam(vstup); 
		znak = getchar();
	}

	Strom.Vypis();
	Strom.UkazStrom();

	Strom.Vyhledej("1000");
*/

	//--------------------Rstrom--------------------
	//delkarace promennych
	int x = 0;
	int y = 0;
	Rstrom Strom;

	int a = scanf("%d", &x);
	int b = scanf("%d", &y);
	printf("ukladam do stromu: %d, %d\n", x, y);
	Strom.VlozPrvniZaznam(x, y);


	a = scanf("%d", &x);
	if (a <= 0){
		b = -1;
	}
	else{
		b = scanf("%d", &y);
	}

	while (b > 0){
		printf("ukladam do stromu: %d, %d\n", x, y);
		Strom.VlozZaznam(x, y);
		a = scanf("%d", &x);
		if (a <= 0){
			break;
		}

		b = scanf("%d", &y);
	}

	Strom.Vypis();
	Strom.UkazStrom();

	return 0;
}