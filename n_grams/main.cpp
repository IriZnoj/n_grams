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
 *		./n_grams vstupni_soubor
 *		./n_grams vstupni_soubor soubor_vyhledavani
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
#include <errno.h>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <map>
#include <ctype.h>

#include "Bstrom.h"
//#include "Rstrom.h"
using namespace std;
long long int VelKolekce = 0;

//funkce nacita ze vstupniho souboru po znacich vstup ktery uklada do promenne vstup. 
//Jednotliva slova - tj. nebile znaky oddelene bilymi - uklada do kolekce a prirazuje jim cislo. 
//Pokud jiz v kolekci slovo existuje, pak je cislo vraceno a tim nevznikaji redundance.
//Jednotliva slova jsou ulozena tedy v tomto slovniku a N-gram, tvoreny temito souradnicemi bude po skonceni funkce v promenne souradnice.
//Navratova hodnota je pocet souradnic v poli souradnice
int nacitaniR(char vstup[], int souradnice[], FILE *vstupni_soubor, int &znak, int &pocet_pismen, map<string, int> &Kolekce, int &pocitadlo){
	int dim = 0;
	int index;
	do{
		znak = fgetc(vstupni_soubor);
		if (isspace(znak)){
			if (pocet_pismen != 0){
				vstup[pocet_pismen] = '\0';
				string str(vstup);

				index = Kolekce[str];
				if (index == 0){
					if (pocitadlo == -1){
						do{
							znak = fgetc(vstupni_soubor);
						} while (znak != '\r' && znak != '\n' && znak != EOF);
					} 
					else{
						Kolekce[str] = pocitadlo;
						index = pocitadlo++;
					}
				}
				souradnice[dim++] = index;
				pocet_pismen = 0;
			}
		}
		else{
			vstup[pocet_pismen++] = znak;
		}
	} while (znak != '\r' && znak != '\n' && znak != EOF);
	return dim;
}

int main(int argc, char *argv[]){

	char retezec[255];
	clock_t t;
	FILE *vstupni_soubor;
	FILE *soubor_vyhledavani;

	if (argc > 1){
		if ((vstupni_soubor = fopen(argv[1], "rb")) == NULL){
			fprintf(stderr, "Chyba cteni vstupniho souboru: %s\n", argv[1]);
			return errno;
		}
		if (argc > 2){
			if ((soubor_vyhledavani = fopen(argv[2], "rb")) == NULL){
				fprintf(stderr, "Chyba cteni souboru s daty k vyhledavani: %s\n", argv[2]);
				return errno;
			}
		}
		else{
			soubor_vyhledavani = stdin;
		}
	}
	else{
		vstupni_soubor = stdin;
		soubor_vyhledavani = stdin;
	}


	//--------------------Bstrom--------------------
	
	char vstup[MAX_SLOVO];
	Bstrom Strom;
	
	//zaznamy oddeleny koncem radku
	int znak = fgetc(vstupni_soubor);
	int pocet_pismen;
	t = clock();
	while (znak != EOF){
		pocet_pismen = 0;
		while (znak != '\r' && znak != '\n' && znak != EOF){
			vstup[pocet_pismen++] = znak;
			znak = fgetc(vstupni_soubor);
			//printf(" - %c - \n", znak);
		}
		vstup[pocet_pismen++] = '\0';
		//printf("ukladam do stromu: %s\n", vstup);
		Strom.VlozZaznam(vstup); 
		znak = fgetc(vstupni_soubor);
	}
	t = clock() - t;
	//Strom.Vypis();
	Strom.VypisVelikost();
	//Strom.UkazStrom();
	printf("%d tiku (%f vterin).\n", t, ((float)t) / CLOCKS_PER_SEC);

	
	printf("Vyhledej: \n");
	t = clock();
	znak = fgetc(soubor_vyhledavani);
	while (znak != EOF){
		pocet_pismen = 0;
		while (znak != '\r' && znak != '\n' && znak != EOF){
			vstup[pocet_pismen++] = znak;
			znak = fgetc(soubor_vyhledavani);
		}
		vstup[pocet_pismen++] = '\0';
		//printf("hledam: %s\n", vstup);
		if (Strom.Vyhledej(vstup)){
			//printf("%s Nalezeno\n", vstup);
		}
		else{
			//printf("%s NENalezeno\n", vstup);
		}
		znak = fgetc(soubor_vyhledavani);
		while (znak == '\r' || znak == '\n'){
			znak = fgetc(soubor_vyhledavani);
		}
	}
	t = clock() - t;
	printf(" s casem: %d tiku (%f vterin).\n", t, ((float)t) / CLOCKS_PER_SEC);
	
	//--------------------Bstrom--------------------

	//--------------------Rstrom-------------------- s Kolekci
	/*
	//delkarace promennych
	int souradnice[] = {0, 0, 0, 0, 0};
	int znak, dim = 0;
	char vstup[MAX_SLOVO];
	Rstrom Strom;
	map<string, int> Kolekce;
	int pocitadlo = 1;

	//zaznamy oddeleny koncem radku
	int pocet_pismen;
	t = clock();
	do{
		pocet_pismen = 0;		
		nacitaniR(vstup, souradnice, vstupni_soubor, znak, pocet_pismen, Kolekce, pocitadlo);
		//Strom.VlozZaznam(souradnice, nacitaniR(vstup, souradnice, vstupni_soubor, znak, pocet_pismen, Kolekce, pocitadlo));
	} while (znak != EOF);
	t = clock() - t;
	printf("vse vlozeno s casem: %d tiku (%f vterin).\n", t, ((float)t) / CLOCKS_PER_SEC);
	printf("velikost kolekce: %LLd\n", (VelKolekce += sizeof(Kolekce))/1000);
	//Strom.Vypis();
	Strom.VypisVelikost();
	//Strom.UkazStrom();
	//Strom.VypisPlus();
	//printf("Pocet vlozenych zaznamu: %d\n", PocetPolozek);
	//printf("Vyhledej: \n");
	t = clock();
	bool nal = true;
	do{
		pocet_pismen = 0;
		dim = nacitaniR(vstup, souradnice, soubor_vyhledavani, znak, pocet_pismen, Kolekce, pocitadlo);
		if (znak == EOF){
			break;
		}
		//printf("'%d' - ", souradnice[0]);
		//printf("%d: ", dim);
		
		if (dim != 0){
			if (nal && Strom.Vyhledej(souradnice, dim)){
				//printf("... %s Nalezeno\n", vstup);
			}
			else{
				//printf("'... %s' NENalezeno\n", vstup);
			}
		}
		nal = true;
	} while (1);
	t = clock() - t;
	printf("vyhledavani s casem: %d tiku (%f vterin).\n", t, ((float)t) / CLOCKS_PER_SEC);
	*/
	
	//--------------------Rstrom-------------------- s Kolekci

	//--------------------Rstrom--------------------
	/*
	Rstrom Strom;
	char vstup[MAX_SLOVO];
	int znak, pocet_pismen;

	t = clock();
	while (fgets(vstup, sizeof(vstup)-1, vstupni_soubor) != NULL){
		strtok(vstup, "\n");
		//printf("ukladam do stromu: %s\n", vstup);
		Strom.VlozZaznam(vstup);
		//Strom.UkazStrom();
	}
	t = clock() - t;
	printf("vse vlozeno s casem: %d tiku (%f vterin).\n", t, ((float)t) / CLOCKS_PER_SEC);
	//Strom.Vypis();
	Strom.VypisVelikost();
	//Strom.UkazStrom();
	//Strom.VypisPlus();

	//printf("Vyhledej: \n");
	t = clock();
	while (fgets(vstup, sizeof(vstup)-1, soubor_vyhledavani)){
		strtok(vstup, "\r\n");
		//printf("%s\n", vstup);
		if (Strom.Vyhledej(vstup)){
			//printf("%s Nalezeno\n", vstup);
		}
		else{
			//printf("%s NENalezeno\n", vstup);
		}
	}
	t = clock() - t;
	printf(" s casem: %d tiku (%f vterin).\n", t, ((float)t) / CLOCKS_PER_SEC);
	*/
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
	
	return 0;
}