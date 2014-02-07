#pragma once

//jako klíè budu používat int
static const int MAX_INT = 2147483647;
//K udava pocet zaznamu ve strankach/uzlech/podstromech
//Napr. pro K=4 budou v kazdem podstromu 2-4 zaznamy a az 5 potomku
static const int K = 4;
//maximalni velikost zaznamu
static const int MAX_SLOVO = 75;

class Bstrom;
static Bstrom *Koren;

class Bzaznam{

public:
	char text[MAX_SLOVO];
	//Bzaznam *Soused;

	Bzaznam(char *text);
};

class Bstrom{
	private:
		int pocetZaznamu;
		Bzaznam *Zaznamy[K];
		Bstrom *Rodic;
		Bstrom *Potomci[K + 1];

		void inicializace();
		void VytvoreniStromu(Bzaznam *z);
		void VlozZaznam(Bzaznam *zaznam);
		void VlozZaznamDoRodice(Bzaznam *zaznam, Bstrom *RPotomek);
		void VlozDoPotomka(Bzaznam *zaznam);
		void VlozDoListu(Bzaznam *zaznam);
		int JePrvniVetsi(Bzaznam *z1, Bzaznam *z2);
		void VlozVlevo(Bzaznam *z, int index);
		void VlozVpravo(Bzaznam *z);
		void PresunZaznamyVpravo(int index);
		void PresunPotomkyVpravo(int index);
		void RozdelList(Bzaznam *zaznam);
		void RozdelUzel(Bzaznam *zaznam, Bstrom *RPotomek);

	public:
		Bstrom();
		Bstrom(char *text); 
		Bstrom(Bzaznam *z);
		Bstrom(Bzaznam *z, Bstrom *LPotomek, Bstrom *RPotomek);
		void VlozZaznam(char *text);
		void VlozPrvniZaznam(char *text);
		void VypisPolozky(Bstrom *strom);
		void Vypis();
		void UkazStrom();
		void VypisZaznamySPotomky();
};