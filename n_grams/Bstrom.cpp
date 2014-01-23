#include "Bstrom.h";

class Bstrom{
	//jako klíè budu používat int
	static const int MAX_INT = 2147483647;
	//K udava pocet zaznamu ve strankach/uzlech/podstromech
	//Napr. pro K=4 budou v kazdem podstromu 2-4 zaznamy a az 5 potomku
	static const int K = 4;

	int pocetZaznamu = 0;
	Bzaznam *Zaznamy[K];
	Bstrom *Potomci[K + 1];
	
	Bstrom(char *text){
		Zaznamy[0] = new Bzaznam(text);
		pocetZaznamu = 1;
	}

	void VlozZaznam(Bzaznam *zaznam){
		int porovnani;
		for (int i = 0; i < K; i++){
			porovnani = JePrvniVetsi(Zaznamy[i], zaznam);
			if(porovnani == -1){
				//jde vlevo
				VlozVlevo(zaznam, i);
			}
			else{
				//if(porovnani == 0)
					//zaznam zde jiz existuje, lze dodat informace do zaznamu
			}
		}
		//jde vpravo
		VlozVpravo(zaznam);
	}

	/*
	return -1 prvni je mensi
	return 0 jsou stejne
	return 1 prvni je vetsi
	*/
	int JePrvniVetsi(Bzaznam *z1, Bzaznam *z2){
		for (int i = 0; i < MAX_SLOVO; i++){
			if (z1->text[i] < z2->text[i]){
				//z1 je mensi
				return -1;
			}
			else if (z1->text[i] > z2->text[i]){
				//z1 je vetsi
				return 1;
			}
		}
		//zaznamy jsou stejne
		return 0;
	}

	void VlozVlevo(Bzaznam *z, int index){
		if (pocetZaznamu == K - 1){
			//je potreba list rozdelit na dva
		}
		else{
			PresunZaznamyVpravo(index);
			Zaznamy[index] = z;
			pocetZaznamu++;
			Zaznamy[index]->Soused = Zaznamy[index + 1];
		}
	}

	void VlozVpravo(Bzaznam *z){
		if (pocetZaznamu == K-1){
			//je potreba list rozdelit na dva a !zvednou vysku ve vsech listech!
		}
		else{
			Zaznamy[pocetZaznamu] = z;
			Zaznamy[pocetZaznamu - 1]->Soused = Zaznamy[pocetZaznamu++];

		}
	}

	void PresunZaznamyVpravo(int index){
		//vim, ze pocet zaznamu je < K-1
		for (int i = pocetZaznamu; i > index; i--){
			Zaznamy[i] = Zaznamy[i - 1];
		}
	}

};