#pragma once

//vychazim z mnou napsaneho Bstromu

//jako klic budu pouzivat int
static const int MAX_INT = 2147483647;
//K udava pocet zaznamu ve strankach/uzlech/podstromech
//Napr. pro K=4 budou v kazdem podstromu 2-4 zaznamy a az 4 potomci
static const int K = 4;
//m je nejmensi pocet zaznamu v listu
static const int m = 2;
//maximalni velikost zaznamu--------------------------------------!!!
static const int MAX_SLOVO = 75;
//maximalni Dimenze pro zaznam Rstromu (pocet cisel oddelenych mezerou ve vstupnim retezci)
static const int D = 5;

class Rstrom;
static Rstrom *Koren;

class Rzaznam{

public:
	int souradnice[D];
	//pocet cisel v poli
	int dimenze;
	int x;
	int y;
	Rstrom *mbr;
	//Rzaznam *Soused;
	Rzaznam(int souradnice[], int velikost);
	Rzaznam(char *retezec);
	void RetezecNaCisla(char *retezec);
};

//staticka pomocna promenna
static Rzaznam *PomocnyPole[K + 1];
static Rstrom *PomocnyPoleUzlu[K + 1];

class Rstrom{
private:
	//hranice mbr/stromu, D = dimenze, 2 => min(0) && max(1)
	int hranice[D][2];
	//dimenze = max(dimenze jakehokoli potomka)
	int dimenze;

	int mbr; //obsah

	int pocetZaznamu; //v listu se jedna o pocet zaznamu, v nelistovym uzlu jde o pocet potomku
	Rzaznam *Zaznamy[K];
	Rstrom *Rodic;
	Rstrom *Potomci[K];

	void inicializace();
	void VytvoreniStromu(Rzaznam *z);
	void VlozZaznam(Rzaznam *zaznam);
	void VlozUzel(Rstrom *potomek);
	void VlozDoUzlu(Rzaznam *zaznam);
	int Max(int a, int b);
	int Min(int a, int b);
	int VetsiSouradnice(Rstrom *LPotomek, Rstrom *RPotomek, int dimenze);
	int MensiSouradnice(Rstrom *lPotomek, Rstrom *RPotomek, int dimenze);
	void ZkontrolujHranici(int hranice[], int dimenze);
	void ZkontrolujHranici(int hranice[][2], int dimenze);
	bool PorovnejAZmen(int hranice[], int dimenze);
	bool PorovnejAZmen(int hranice[][2], int dimenze);
	void PrepocitejObsah(); 
	void VypoctiHranice(int &l, int &r, int &u, int &d, int x, int y);
	void VypoctiHraniceUzlu(int &l, int &r, int &u, int &d, Rstrom *strom);
	int DalsiPrvek(Rstrom *strom, bool prepocitatLevy, bool prepocitatPravy, int zmenal[], int zmenar[]);
	int VypocitejObsah(int hranice[][2], int dimenze);
	void VlozDoListu(Rzaznam *zaznam);
	void RozdelList(Rzaznam *zaznam);
	void VyberDvaZaznamy(int &l, int &r);
	void VyberDvaUzly(int &l, int &r);
	void RozdelUzel(Rstrom *podstrom);
	bool Vyhledej(Rzaznam *zaznam);
	bool JeStromList();
	void VypisPolozky();
	void VypisZaznamySPotomky(int hloubka);

public:
	Rstrom();
	Rstrom(char *souradnice);
	Rstrom(Rzaznam *z);
	Rstrom(Rstrom *potomek);
	Rstrom(Rstrom *LPotomek, Rstrom *RPotomek);
	void VlozZaznam(char *souradnice);
	void VlozZaznam(int souradnice[], int velikost);
	void VlozPrvniZaznam(char *souradnice);
	void VlozPrvniZaznam(int souradnice[], int velikost);
	bool Vyhledej(char *souradnice);
	void Vypis();
	void UkazStrom();
};