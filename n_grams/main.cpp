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
#include <string.h>

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
	int souradnice[] = {0, 0, 0, 0, 0};
	char string[255];
	int y = 0;
	Rstrom Strom;

	int a = scanf("%d", &souradnice[0]);
	int b = scanf("%d", &souradnice[1]);
	printf("ukladam do stromu: %d, %d\n", souradnice[0], souradnice[1]);
	Strom.VlozPrvniZaznam(souradnice, 2);

	fgets(string, sizeof(string)-1, stdin);
	while (fgets(string, sizeof(string)-1, stdin)){
		strtok(string, "\n");
		printf("ukladam do stromu: %s\n", string);
		Strom.VlozZaznam(string);
	}

	Strom.Vypis();
	Strom.UkazStrom();

	return 0;
}