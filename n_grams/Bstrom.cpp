#include <stdlib.h>
#include <stdio.h>
#include "Bstrom.h"


Bzaznam::Bzaznam(char *text){
	for (int i = 0; i < MAX_SLOVO; i++){
		this->text[i] = text[i];
		if (text[i] == '\0'){
			return;
		}

	}
	//Soused = NULL;
}

Bstrom::Bstrom(){
	inicializace();
	pocetZaznamu = 0;
	Koren = this;
}

Bstrom::Bstrom(Bzaznam *z){
	VytvoreniStromu(z);
}

Bstrom::Bstrom(char *text){
	Bzaznam *z = new Bzaznam(text);
	VytvoreniStromu(z);
}

Bstrom::Bstrom(Bzaznam *z, Bstrom *LPotomek, Bstrom *RPotomek){
	VytvoreniStromu(z);
	Potomci[0] = LPotomek;
	Potomci[0]->Rodic = this;
	Potomci[1] = RPotomek;
	Potomci[1]->Rodic = this;
}

void Bstrom::inicializace(){
	Rodic = NULL;
	for (int i = 0; i < K ; i++){
		Potomci[i] = NULL;
		Zaznamy[i] = NULL;
	}
	//potomku je o 1 vic nez zaznamu
	Potomci[K] = NULL;
}

void Bstrom::VytvoreniStromu(Bzaznam *z){
	inicializace();
	Zaznamy[0] = z;
	pocetZaznamu = 1;
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
	if (Koren->Potomci[0] != NULL){
		//nejsem v listu
		Koren->VlozDoPotomka(zaznam);
	}
	else{
		Koren->VlozDoListu(zaznam);
	}
}

void Bstrom::VlozZaznamDoRodice(Bzaznam *zaznam, Bstrom *RPotomek){
	int porovnani;
	if (pocetZaznamu == K){
		//je potreba list rozdelit na dva
		RozdelUzel(zaznam, RPotomek);
		return;
	}
	for (int i = 0; i < pocetZaznamu; i++){
		porovnani = JePrvniVetsi(Zaznamy[i], zaznam);
		if (porovnani == 1){
			//jde vlevo a na i-ty index ulozi tento prvek
			VlozVlevo(zaznam, i);
			PresunPotomkyVpravo(i+1);
			Potomci[i + 1] = RPotomek;
			//potomek ma novyho rodice
			RPotomek->Rodic = this;
			return;
		}
		else if (porovnani == 0){
			//zaznam zde jiz existuje, lze dodat informace do zaznamu
			return;
		}
	}
	//jde vpravo
	VlozVpravo(zaznam);
	//pocet zaznamu je jiz aktualizovany, takze je ted stejny pocet zaznamu jako potomku
	Potomci[pocetZaznamu] = RPotomek;
	//potomek ma novyho rodice pokud potomek neni null (plati pro prave potomky nejpravejsich
	//zaznamu, jelikoz tyto nejpravejsi zaznamy maji stejnou hodnotu jako rodic - proto
	//nemuze byt nic vetsi - tedy nemuze byt nic vpravo
	if (RPotomek != NULL){
		RPotomek->Rodic = this;
	}
}

void Bstrom::VlozPrvniZaznam(char *text){
	Zaznamy[0] = new Bzaznam(text);
	pocetZaznamu = 1;
}

void Bstrom::VlozDoPotomka(Bzaznam *zaznam){
	int porovnani;
	for (int i = 0; i < pocetZaznamu; i++){
		porovnani = JePrvniVetsi(Zaznamy[i], zaznam);
		if (porovnani == 1){
			//jde vlevo
			Potomci[i]->VlozZaznam(zaznam);
			return;
		}
		else if (porovnani == 0){
			//zaznam zde jiz existuje, lze dodat informace do zaznamu
			return;
		}
	}
	//jde vpravo (pocetZaznamu - 1 /spravnej index/ + 1 /vpravo od tohoto indexu v poli Potomci/
	Potomci[pocetZaznamu]->VlozZaznam(zaznam);
}

void Bstrom::VlozDoListu(Bzaznam *zaznam){
	int porovnani;
	if (pocetZaznamu == K){
		//je potreba list rozdelit na dva
		RozdelList(zaznam);
		return;
	}
	for (int i = 0; i < pocetZaznamu; i++){
		porovnani = JePrvniVetsi(Zaznamy[i], zaznam);
		if (porovnani == 1){
			//jde vlevo
			VlozVlevo(zaznam, i);
			return;
		}
		else if (porovnani == 0){
			//zaznam zde jiz existuje, lze dodat informace do zaznamu
			return;
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
	//Zaznamy[index]->Soused = Zaznamy[index + 1];
}

void Bstrom::VlozVpravo(Bzaznam *z){
	Zaznamy[pocetZaznamu] = z;
	//Zaznamy[pocetZaznamu - 1]->Soused = Zaznamy[pocetZaznamu++];
	pocetZaznamu++;
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
	//nejdrive zkontroluji, jestli se zde zaznam nevyskytuje
	int porovnani;
	for (int i = 0; i < pocetZaznamu; i++){
		porovnani = JePrvniVetsi(Zaznamy[i], zaznam);
		if (porovnani == 0){
			//zaznam zde jiz existuje, lze dodat informace do zaznamu
			return;
		}
	}

	//levy zustava, pokud je K liche cislo, pak je vlevo mensi pulka
	//pocetZaznamu/2-1 znamena, vezmu polovinu prvku, dostanu prostredni prvek a -1 dava spravny index tohoto prvku
	//lze udelat jako konstantu - vysledek je zde vzdy stejny
	int stredniIndex = pocetZaznamu / 2 - 1;

	//Bzaznam *prostredniPrvek = Zaznamy[stredniIndex];
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
		novyList->Rodic = Rodic;
	}
	else{
		//delim koren
		Rodic = new Bstrom(Zaznamy[stredniIndex], this, novyList);
		Koren = Rodic;
		//printf("Novy koren - zaznam: %s\n", Koren->Zaznamy[0]->text);
	}

	//zjistim do ktere stranky bude patrit nove vkladany prvek
	porovnani = JePrvniVetsi(zaznam, Zaznamy[stredniIndex]);
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
	//lze udelat jako konstantu - vysledek je zde vzdy stejny
	int stredniIndex = pocetZaznamu / 2 - 1;

	Bzaznam *prostredniPrvek = Zaznamy[stredniIndex];
	//prvni prvek kterej presunu do novyho uzlu je ten prvni vpravo od prostredniho...
	Bstrom *novyUzel = new Bstrom(Zaznamy[stredniIndex + 1]);
	Zaznamy[stredniIndex + 1] = NULL;
	//jelikoz je rodicem zaznam s indexem stredniIndex+1, tak jiz vetsi zaznam jak tento sem neprijde
	//vzdy prijde do stromu novyUzel, proto je odkaz presunut tam
	novyUzel->Potomci[0] = Potomci[stredniIndex + 1];
	novyUzel->Potomci[0]->Rodic = novyUzel;
	novyUzel->Potomci[1] = Potomci[stredniIndex + 2];
	novyUzel->Potomci[1]->Rodic = novyUzel;
	Potomci[stredniIndex + 2] = NULL;
	Potomci[stredniIndex + 1] = NULL;

	for (int i = stredniIndex + 2; i < pocetZaznamu; i++){
		novyUzel->VlozZaznamDoRodice(Zaznamy[i], Potomci[i+1]);
		Zaznamy[i] = NULL;
		Potomci[i + 1] = NULL;
	}

	/*	osetreno pro vkladani uzlu
		for (int i = 0; i < novyUzel->pocetZaznamu + 1; i++){
		novyUzel->Potomci[i]->Rodic = novyUzel;
	}*/

	//aktualizuju promennou pocetZaznamu, protoze jsem jich ted pulku presunul do jinyho uzlu
	pocetZaznamu = stredniIndex + 1;

	if (Rodic != NULL){
		Rodic->VlozZaznamDoRodice(Zaznamy[stredniIndex], novyUzel);
	}
	else{
		//delim koren
		Rodic = new Bstrom(Zaznamy[stredniIndex], this, novyUzel);
		Koren = Rodic;
		//printf("Novy koren - zaznam: %s\n", Koren->Zaznamy[0]->text);
	}

	novyUzel->Rodic = Rodic;

	//zjistim do ktere stranky bude patrit nove vkladany uzel
	int porovnani = JePrvniVetsi(zaznam, Zaznamy[stredniIndex]);
	if (porovnani == -1){
		//jde vlevo
		VlozZaznamDoRodice(zaznam, RPotomek);
	}
	else{
		//jde vpravo
		novyUzel->VlozZaznamDoRodice(zaznam, RPotomek);
	}
}

void Bstrom::VypisPolozky(Bstrom *strom){
	if (strom->Potomci[0] != NULL){
		for (int i = 0; i < strom->pocetZaznamu + 1; i++){
			//podminka pro nejpravejsi potomky...
			if (strom->Potomci[i] != NULL){
				strom->Potomci[i]->VypisPolozky(strom->Potomci[i]);
			}
		}
	}
	else{
		for (int i = 0; i < strom->pocetZaznamu; i++){
			printf("%s\n", strom->Zaznamy[i]->text);
		}
	}
}

void Bstrom::Vypis(){
	VypisPolozky(Koren);
}

void Bstrom::UkazStrom(){
	printf("Koren: \n");
	Koren->VypisZaznamySPotomky();
	if (Koren->Potomci[0] != NULL){
		for (int i = 0; i < Koren->pocetZaznamu + 1; i++){
			Koren->Potomci[i]->VypisZaznamySPotomky();
		}
	}
}

void Bstrom::VypisZaznamySPotomky(){
	for (int i = 0; i < pocetZaznamu; i++){
		printf("Cislo zaznamu: %d, zaznam: %s\n", i, Zaznamy[i]->text);
		/*if (Zaznamy[i]->Soused != NULL){
		printf(" - Soused: %s\n", Zaznamy[i]->Soused->text);
		}*/
	}
	for (int i = 0; i < K + 1; i++){
		if (Potomci[i] != NULL){
			for (int j = 0; j < Potomci[i]->pocetZaznamu; j++){
				printf("Potomek %d: %s\n", i, Potomci[i]->Zaznamy[j]->text);
			}
		}
	}
	printf("\n");
}