#include "Bstrom.h";

class Bstrom{
	//jako klíè budu používat int
	static const int MAX_INT = 2147483647;
	//K udava pocet zaznamu ve strankach/uzlech/podstromech
	//Napr. pro K=4 budou v kazdem podstromu 2-4 zaznamy a az 5 potomku
	static const int K = 4;
	//pocet zaznamu v teto strance
	int pocetZaznamu = 0;

	Bzaznam *Zaznamy[K];
	Bstrom *Rodic;
	//Bstrom *Potomci[K + 1];
	
	Bstrom(char *text){
		Zaznamy[0] = new Bzaznam(text);
		pocetZaznamu = 1;
		Rodic = NULL;
	}

	Bstrom(Bzaznam *z){
		Zaznamy[0] = z;
		pocetZaznamu = 1;
		Rodic = NULL;
	}

	Bstrom(Bzaznam *z, Bstrom *LPotomek, Bstrom *RPotomek){
		Zaznamy[0] = z;
		pocetZaznamu = 1;
		Rodic = NULL;
		z->LPotomek = LPotomek;
		z->RPotomek = RPotomek;
	}

	void VlozZaznam(Bzaznam *zaznam){
		if (Zaznamy[0]->LPotomek != NULL){
			//nejsem v listu
			VlozDoPotomka(zaznam);
		}
		else{
			VlozDoListu(zaznam);
		}
	}

	void VlozDoPotomka(Bzaznam *zaznam){
		int porovnani;
		for (int i = 0; i < pocetZaznamu; i++){
			porovnani = JePrvniVetsi(Zaznamy[i], zaznam);
			if (porovnani == -1){
				//jde vlevo
				Zaznamy[i]->LPotomek->VlozZaznam(zaznam);
			}
			else{
				//if(porovnani == 0)
				//zaznam zde jiz existuje, lze dodat informace do zaznamu
			}
		}
		//jde vpravo
		Zaznamy[pocetZaznamu - 1]->RPotomek->VlozZaznam(zaznam);
	}

	void VlozDoListu(Bzaznam *zaznam){
		int porovnani;
		if (pocetZaznamu == K - 1){
			//je potreba list rozdelit na dva
			RozdelList(zaznam);
		}
		for (int i = 0; i < pocetZaznamu; i++){
			porovnani = JePrvniVetsi(Zaznamy[i], zaznam);
			if (porovnani == -1){
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
		PresunZaznamyVpravo(index);
		Zaznamy[index] = z;
		pocetZaznamu++;
		Zaznamy[index]->Soused = Zaznamy[index + 1];
	}

	void VlozVpravo(Bzaznam *z){
		Zaznamy[pocetZaznamu] = z;
		Zaznamy[pocetZaznamu - 1]->Soused = Zaznamy[pocetZaznamu++];
	}

	void PresunZaznamyVpravo(int index){
		//vim, ze pocet zaznamu je < K-1
		for (int i = pocetZaznamu; i > index; i--){
			Zaznamy[i] = Zaznamy[i - 1];
		}
	}

	void RozdelList(Bzaznam *zaznam){
		//levy zustava, pokud je K liche cislo, pak je vlevo mensi pulka
		//pocetZaznamu/2-1 znamena, vezmu polovinu prvku, dostanu prostredni prvek a -1 dava spravny index tohoto prvku
		int stredniIndex = pocetZaznamu / 2 - 1;

		Bzaznam *prostredniPrvek = Zaznamy[stredniIndex];
		//prvni prvek kterej presunu do novyho uzlu je ten prvni vpravo od prostredniho...
		Bstrom *novyUzel = new Bstrom(Zaznamy[stredniIndex+1]);
		Zaznamy[stredniIndex + 1] = NULL;
		for (int i = stredniIndex + 2; i < pocetZaznamu; i++){
			novyUzel->VlozZaznam(Zaznamy[i]);
			Zaznamy[i] = NULL;
		}
		//aktualizuju promennou pocetZaznamu, protoze jsem jich ted pulku presunul do jinyho uzlu
		pocetZaznamu = stredniIndex + 1;


		if (Rodic != NULL){
			Rodic->VlozZaznam(Zaznamy[stredniIndex]);
			Zaznamy[stredniIndex]->LPotomek = this;
			Zaznamy[stredniIndex]->RPotomek = novyUzel;
		}
		else{
			//delim koren
			Rodic = new Bstrom(Zaznamy[stredniIndex], this, novyUzel);
			/*
			tohle je varianta kdy jsou potomci v rodici - musel bych vzdy pri vkladani prvku potomky preskladat a to se mi nelibi
			Rodic->Potomci[0] = this;
			Rodic->Potomci[1] = novyUzel;
			*/
		}

		//zjistim do ktere stranky bude patrit nove vkladany prvek
		int porovnani = JePrvniVetsi(zaznam, Zaznamy[stredniIndex]);
		if (porovnani == -1){
			//jde vlevo
			VlozZaznam(zaznam);
		}
		else{
			//if(porovnani == 0) zaznam zde jiz existuje, lze dodat informace do zaznamu
			//jde vpravo
			novyUzel->VlozZaznam(zaznam);
		}
	}
};