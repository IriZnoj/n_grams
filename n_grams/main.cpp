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
 *
 *		pro pouziti Bstromu je treba odkomentovat radek s pripojenou knihovnou a sekci
 *		oznacenou: //--------------------Bstrom--------------------
 *
 *		pro pouziti Rstromu je treba odkomentovat radek s pripojenou knihovnou a sekci
 *		oznacenou: //--------------------Rstrom--------------------
 *
 *		v main:	void Vypis():
 *								vypis vsech polozech ulozenych ve stromu	
 *						void UkazStrom():						
 *								vypis polozek tak jak jsou ulozeny ve stromu vcetne jeho struktury
 *						void VlozZaznam(char vstup[]):
 *								vlozeni zaznamu do stromu 
 *						bool Vyhledej(char retezec[]):
 *								vyhledani zaznamu ve stromu
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//#include "Bstrom.h"
#include "Rstrom.h"
using namespace std;

int main(){

	char string[255];

	//--------------------Bstrom--------------------
	/*
	char vstup[MAX_SLOVO];
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
		//printf("ukladam do stromu: %s\n", vstup);
		Strom.VlozZaznam(vstup); 
		znak = getchar();
	}

	Strom.Vypis();
	Strom.UkazStrom();
	
	*/
	//--------------------Bstrom--------------------

	//--------------------Rstrom--------------------
	
	//delkarace promennych
	int souradnice[] = {0, 0, 0, 0, 0};
	
	int y = 0;
	Rstrom Strom;

	//jina moznost jak ukladat zaznamy do Rstromu
	//int a = scanf("%d", &souradnice[0]);
	//int b = scanf("%d", &souradnice[1]);
	//printf("ukladam do stromu: %d, %d\n", souradnice[0], souradnice[1]);
	//Strom.VlozZaznam(souradnice, 2);
	
	//int PocetPolozek = 0;
	//fgets(string, sizeof(string)-1, stdin);
	while (fgets(string, sizeof(string)-1, stdin)){
		strtok(string, "\n");
		//printf("ukladam do stromu: %s\n", string);
		Strom.VlozZaznam(string);
		//PocetPolozek++;
		//Strom.UkazStrom();
	}

	Strom.Vypis();

	//Strom.UkazStrom();
	//Strom.VypisPlus();
	//printf("Pocet vlozenych zaznamu: %d\n", PocetPolozek);

	//--------------------Rstrom--------------------
	
	/*
	char str[4];
	for (int i = 0; i < 1000; i++){
		sprintf(str, "%d", i);
		if (!Strom.Vyhledej(str)){
			printf("nenalezeno: %d\n", i);
		}
	}
	*/

	printf("Vyhledej: ");
	while (fgets(string, sizeof(string)-1, stdin)){
		strtok(string, "\n");
		if (Strom.Vyhledej(string)){
			printf("Nalezeno\n");
		}
		else{
			printf("-NENalezeno\n");
		}
	}

	
	return 0;
}