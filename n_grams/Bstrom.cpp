#include <stdlib.h>
#include <stdio.h>
#include "Bstrom.h"


Bzaznam::Bzaznam(char *text){
	for (int i = 0; i < MAX_SLOVO; i++){
		this->text[i] = text[i];
	}
	Soused = NULL;
}

Bstrom::Bstrom(){
	inicializace();
	pocetZaznamu = 0;
}

Bstrom::Bstrom(Bzaznam *z){
	inicializace();
	Zaznamy[0] = z;
	pocetZaznamu = 1;
}

Bstrom::Bstrom(char *text){
	Bzaznam *z = new Bzaznam(text);
	//Bstrom::Bstrom(z);
}

Bstrom::Bstrom(Bzaznam *z, Bstrom *LPotomek, Bstrom *RPotomek){
	//Bstrom::Bstrom(z);
	Potomci[0] = LPotomek;
	Potomci[0]->Rodic = this;
	Potomci[1] = RPotomek;
	Potomci[1]->Rodic = this;
}

void Bstrom::inicializace(){
	Rodic = NULL;
	for (int i = 0; i < K + 1; i++){
		Potomci[i] = NULL;
	}
}

void Bstrom::VlozZaznam(Bzaznam *zaznam){
	if (Potomci[0] != NULL){
		//nejsem v listu
		VlozDoPotomka(zaznam);
	}
	else{
		VlozDoListu(zaznam);
	}
}

void Bstrom::VlozZaznam(char *text){
	Bzaznam *zaznam = new Bzaznam(text);
	VlozZaznam(zaznam);
}

void Bstrom::VlozZaznamDoRodice(Bzaznam *zaznam, Bstrom *RPotomek){
	int porovnani;
	if (pocetZaznamu == K - 1){
		//je potreba list rozdelit na dva
		RozdelUzel(zaznam, RPotomek);
	}
	for (int i = 0; i < pocetZaznamu; i++){
		porovnani = JePrvniVetsi(Zaznamy[i], zaznam);
		if (porovnani == -1){
			//jde vlevo a na i-ty index ulozi tento prvek
			VlozVlevo(zaznam, i);
			PresunPotomkyVpravo(i+1);
			Potomci[i + 1] = RPotomek;
			return;
		}
		else{
			//if (porovnani == 0){
				//zaznam zde jiz existuje, lze dodat informace do zaznamu
			//}
		}
	}
	//jde vpravo
	VlozVpravo(zaznam);
	//pocet zaznamu je jiz aktualizovany, takze je ted stejny pocet zaznamu jako potomku
	Potomci[pocetZaznamu] = RPotomek;
}

void Bstrom::VlozPrvniZaznam(char *text){
	Zaznamy[0] = new Bzaznam(text);
	pocetZaznamu = 1;
}

void Bstrom::VlozDoPotomka(Bzaznam *zaznam){
	int porovnani;
	for (int i = 0; i < pocetZaznamu; i++){
		porovnani = JePrvniVetsi(Zaznamy[i], zaznam);
		if (porovnani == -1){
			//jde vlevo
			Potomci[i]->VlozZaznam(zaznam);
		}
		else{
			//if(porovnani == 0)
			//zaznam zde jiz existuje, lze dodat informace do zaznamu
		}
	}
	//jde vpravo (pocetZaznamu - 1 /spravnej index/ + 1 /vpravo od tohoto indexu v poli Potomci/
	Potomci[pocetZaznamu]->VlozZaznam(zaznam);
}

void Bstrom::VlozDoListu(Bzaznam *zaznam){
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
			return;
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
int Bstrom::JePrvniVetsi(Bzaznam *z1, Bzaznam *z2){
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

void Bstrom::VlozVlevo(Bzaznam *z, int index){
	PresunZaznamyVpravo(index);
	Zaznamy[index] = z;
	Zaznamy[index]->Soused = Zaznamy[index + 1];
}

void Bstrom::VlozVpravo(Bzaznam *z){
	Zaznamy[pocetZaznamu] = z;
	Zaznamy[pocetZaznamu - 1]->Soused = Zaznamy[pocetZaznamu++];
}

void Bstrom::PresunZaznamyVpravo(int index){
	//vim, ze pocet zaznamu je < K-1
	for (int i = pocetZaznamu; i > index; i--){
		Zaznamy[i] = Zaznamy[i - 1];
	}
	pocetZaznamu++;
}

void Bstrom::PresunPotomkyVpravo(int index){
	for (int i = pocetZaznamu+1; i > index; i--){
		Potomci[i] = Potomci[i - 1];
	}
}

void Bstrom::RozdelList(Bzaznam *zaznam){
	//levy zustava, pokud je K liche cislo, pak je vlevo mensi pulka
	//pocetZaznamu/2-1 znamena, vezmu polovinu prvku, dostanu prostredni prvek a -1 dava spravny index tohoto prvku
	int stredniIndex = pocetZaznamu / 2 - 1;

	Bzaznam *prostredniPrvek = Zaznamy[stredniIndex];
	//prvni prvek kterej presunu do novyho uzlu je ten prvni vpravo od prostredniho...
	Bstrom *novyList = new Bstrom(Zaznamy[stredniIndex+1]);
	Zaznamy[stredniIndex + 1] = NULL;
	for (int i = stredniIndex + 2; i < pocetZaznamu; i++){
		novyList->VlozZaznam(Zaznamy[i]);
		Zaznamy[i] = NULL;
	}
	//aktualizuju promennou pocetZaznamu, protoze jsem jich ted pulku presunul do jinyho listu
	pocetZaznamu = stredniIndex + 1;

	if (Rodic != NULL){
		Rodic->VlozZaznamDoRodice(Zaznamy[stredniIndex], novyList);
	}
	else{
		//delim koren
		Rodic = new Bstrom(Zaznamy[stredniIndex], this, novyList);
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
		novyList->VlozZaznam(zaznam);
	}
}

void Bstrom::RozdelUzel(Bzaznam *zaznam, Bstrom *RPotomek){
	int stredniIndex = pocetZaznamu / 2 - 1;

	Bzaznam *prostredniPrvek = Zaznamy[stredniIndex];
	//prvni prvek kterej presunu do novyho uzlu je ten prvni vpravo od prostredniho...
	Bstrom *novyUzel = new Bstrom(Zaznamy[stredniIndex + 1]);
	Zaznamy[stredniIndex + 1] = NULL;
	//jelikoz je rodicem zaznam s indexem stredniIndex+1, tak jiz vetsi zaznam jak tento sem neprijde
	//vzdy prijde do stromu novyUzel, proto je odkaz presunut tam
	novyUzel->Potomci[0] = Potomci[stredniIndex + 2];
	Potomci[stredniIndex + 2] = NULL;
	for (int i = stredniIndex + 2; i < pocetZaznamu; i++){
		novyUzel->VlozZaznamDoRodice(Zaznamy[i], Potomci[i+1]);
		Zaznamy[i] = NULL;
		Potomci[i + 1] = NULL;
	}

	//aktualizuju promennou pocetZaznamu, protoze jsem jich ted pulku presunul do jinyho uzlu
	pocetZaznamu = stredniIndex + 1;

	if (Rodic != NULL){
		Rodic->VlozZaznamDoRodice(Zaznamy[stredniIndex], novyUzel);
	}
	else{
		//delim koren
		Rodic = new Bstrom(Zaznamy[stredniIndex], this, novyUzel);
	}

	//zjistim do ktere stranky bude patrit nove vkladany prvek
	int porovnani = JePrvniVetsi(zaznam, Zaznamy[stredniIndex]);
	if (porovnani == -1){
		//jde vlevo
		VlozZaznamDoRodice(zaznam, RPotomek);
	}
	else{
		//if(porovnani == 0) zaznam zde jiz existuje, lze dodat informace do zaznamu
		//jde vpravo
		novyUzel->VlozZaznamDoRodice(zaznam, RPotomek);
	}
}

void Bstrom::VypisPolozky(){
	for (int i = 0; i < pocetZaznamu; i++){
		printf("Cislo zaznamu: %d, zaznam: %s\n", i, Zaznamy[i]->text);
		if (Zaznamy[i]->Soused != NULL){
			printf(" - Soused: %s\n", Zaznamy[i]->Soused->text);
		}
		for (int i = 0; i < K + 1; i++){
			printf("Potomek: %s\n\n", Potomci[i]->Zaznamy[0]->text);
		}
	}
}