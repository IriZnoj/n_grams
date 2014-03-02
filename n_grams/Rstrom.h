#pragma once

//vychazim z mnou napsaneho Bstromu

//jako klic budu pouzivat int
static const int MAX_INT = 2147483647;
//K udava pocet zaznamu ve strankach/uzlech/podstromech
//Napr. pro K=4 budou v kazdem podstromu 2-4 zaznamy a az 5 potomku
static const int K = 4;
//maximalni velikost zaznamu
static const int MAX_SLOVO = 75;

class Rstrom;
static Rstrom *Koren;

class Rzaznam{

public:
	int x;
	int y;
	//Rzaznam *Soused;

	Rzaznam(int x, int y);
};

class Rstrom{
private:
	int pocetZaznamu;
	Rzaznam *Zaznamy[K];
	Rstrom *Rodic;
	Rstrom *Potomci[K + 1];

	void inicializace();
	void VytvoreniStromu(Rzaznam *z);
	void VlozZaznam(Rzaznam *zaznam);
	void VlozZaznamDoRodice(Rzaznam *zaznam, Rstrom *RPotomek);
	void VlozDoPotomka(Rzaznam *zaznam);
	void VlozDoListu(Rzaznam *zaznam);
	int JePrvniVetsi(Rzaznam *z1, Rzaznam *z2);
	void VlozVlevo(Rzaznam *z, int index);
	void VlozVpravo(Rzaznam *z);
	void PresunZaznamyVpravo(int index);
	void PresunPotomkyVpravo(int index);
	void RozdelList(Rzaznam *zaznam);
	void RozdelUzel(Rzaznam *zaznam, Rstrom *RPotomek);
	bool Vyhledej(Rzaznam *zaznam);
	bool JeStromList();

public:
	Rstrom();
	Rstrom(int x, int y);
	Rstrom(Rzaznam *z);
	Rstrom(Rzaznam *z, Rstrom *LPotomek, Rstrom *RPotomek);
	void VlozZaznam(int x, int y);
	void VlozPrvniZaznam(int x, int y);
	bool Vyhledej(int x, int y);
	void VypisPolozky(Rstrom *strom);
	void Vypis();
	void UkazStrom();
	void VypisZaznamySPotomky();
};