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

typedef enum{
	vlevo, vpravo, nahoru, dolu
} strana;


class Rstrom;
static Rstrom *Koren;

class Rzaznam{

public:
	int x;
	int y;
	Rstrom *mbr;
	//Rzaznam *Soused;
	Rzaznam(int x, int y);
};

//staticka pomocna promenna
static Rzaznam *PomocnyPole[K + 1];
static Rstrom *PomocnyPoleUzlu[K + 1];

class Rstrom{
private:
	//hranice mbr/stromu
	int l; //left
	int u; //up
	int r; //right
	int d; //down

	int mbr; //obsah

	int pocetZaznamu; //v listu se jedna o pocet zaznamu, v nelistovym uzlu jde o pocet potomku
	Rzaznam *Zaznamy[K];
	Rstrom *Rodic;
	Rstrom *Potomci[K];

	void inicializace();
	void VytvoreniStromu(Rzaznam *z);
	void VlozZaznam(Rzaznam *zaznam);
	void VlozUzel(Rstrom *RPotomek);
	void VlozDoUzlu(Rzaznam *zaznam);
	void ZkontrolujHranici(int x, int y);
	void ZkontrolujHranici(int l, int r, int u, int d);
	bool PorovnejAZmen(int l, int r, int u, int d);
	void VypoctiNoveHranice(int &l, int &r, int &u, int &d, int x, int y);
	int DalsiPrvek(Rstrom *strom, bool prepocitatLevy, bool prepocitatPravy, int zmenal[], int zmenar[]);
	int VypocitejObsah(int ax, int ay, int bx, int by);
	void VlozDoListu(Rzaznam *zaznam);
	void RozdelList(Rzaznam *zaznam);
	void VyberDvaZaznamy(int &l, int &r);
	void VyberDvaUzly(int &l, int &r);
	void RozdelUzel(Rstrom *RPotomek);
	bool Vyhledej(Rzaznam *zaznam);
	bool JeStromList();
	void VypisPolozky();
	void VypisZaznamySPotomky(int hloubka);

public:
	Rstrom();
	Rstrom(int x, int y);
	Rstrom(Rzaznam *z);
	Rstrom(Rstrom *Potomek);
	Rstrom(Rstrom *LPotomek, Rstrom *RPotomek);
	void VlozZaznam(int x, int y);
	void VlozPrvniZaznam(int x, int y);
	bool Vyhledej(int x, int y);
	void Vypis();
	void UkazStrom();
};