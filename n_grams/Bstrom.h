#pragma once
#include "Bzaznam.h"

//static const int MAX_SLOVO = 30;
//jako klíè budu používat int
static const int MAX_INT = 2147483647;
//K udava pocet zaznamu ve strankach/uzlech/podstromech
//Napr. pro K=4 budou v kazdem podstromu 2-4 zaznamy a az 5 potomku
static const int K = 4;

class Bstrom{
	private:
		int pocetZaznamu;

		void inicializace();
		void VlozDoPotomka(Bzaznam *zaznam);
		void VlozDoListu(Bzaznam *zaznam);
		int JePrvniVetsi(Bzaznam *z1, Bzaznam *z2);
		void VlozVlevo(Bzaznam *z, int index);
		void VlozVpravo(Bzaznam *z);
		void PresunZaznamyVpravo(int index);
		void RozdelList(Bzaznam *zaznam);

	public:
		Bzaznam *Zaznamy[K];
		Bstrom *Rodic;
		Bstrom *Potomci[K + 1];

		Bstrom();
		Bstrom(char *text);
		Bstrom(Bzaznam *z);
		Bstrom(Bzaznam *z, Bstrom *LPotomek, Bstrom *RPotomek);
		void VlozZaznam(Bzaznam *zaznam);
		void VlozZaznam(char *text);
		int VlozZaznamDoRodice(Bzaznam *zaznam);
		void VlozPrvniZaznam(char *text);
		void VypisPolozky();
};