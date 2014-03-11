#include <stdlib.h>
#include <stdio.h>
#include "Rstrom.h"

//kazdy zaznam je ve tvaru (mbr, oid) kde
//mbr = minimalni ohranicujici obdelnik ktery obsahuje objekt, 
//oid je id objektu - pouzivam dvojici (x, y) jako jednoznacny identifikator

Rzaznam::Rzaznam(int x, int y){
	this->x = x;
	this->y = y;
	
	this->mbr = NULL;
	//Soused = NULL;
}

Rstrom::Rstrom(){
	Koren = this;
	//po vytvoreni stromu timto zpusobem je vzdy volana
	//metoda VlozPrvniZaznam(int x, int y); az tehdy se cely strom inicializuje
}

Rstrom::Rstrom(Rzaznam *z){
	VytvoreniStromu(z);
}

Rstrom::Rstrom(int x, int y){
	Rzaznam *z = new Rzaznam(x, y);
	VytvoreniStromu(z);
}

Rstrom::Rstrom(Rstrom *LPotomek, Rstrom *RPotomek){
	Potomci[0] = LPotomek;
	//uz jem nastavil pred volanim teto metody
	//Potomci[0]->Rodic = this;
	Potomci[1] = RPotomek;
	Potomci[1]->Rodic = this;
	//hranice rodice
	if (LPotomek->l < RPotomek->l){
		l = LPotomek->l;
	}
	else{
		l = RPotomek->l;
	}
	if (LPotomek->r > RPotomek->r){
		r = LPotomek->l;
	}
	else{
		r = RPotomek->r;
	}
	if (LPotomek->d < RPotomek->d){
		d = LPotomek->d;
	}
	else{
		d = RPotomek->d;
	}
	if (LPotomek->u > RPotomek->u){
		u = LPotomek->u;
	}
	else{
		u = RPotomek->u;
	}

	mbr = VypocitejObsah(l, d, r, u);
	
}

void Rstrom::inicializace(){
	Rodic = NULL;
	for (int i = 0; i < K; i++){
		Potomci[i] = NULL;
		Zaznamy[i] = NULL;
	}
	//potomku je o 1 vic nez zaznamu
	Potomci[K] = NULL;
}

void Rstrom::VytvoreniStromu(Rzaznam *z){
	inicializace();
	Zaznamy[0] = z;
	pocetZaznamu = 1;
	//pro jeden bod je hranice mbr prave tento bod
	this->l = z->x;
	this->u = z->y;
	this->r = z->x;
	this->d = z->y;
	this->mbr = 0;

}

void Rstrom::VlozPrvniZaznam(int x, int y){
	Rzaznam *z = new Rzaznam(x, y);
	VytvoreniStromu(z);
}

void Rstrom::VlozZaznam(Rzaznam *zaznam){
	if (Potomci[0] != NULL){
		//nejsem v listu
		VlozDoUzlu(zaznam);
	}
	else{
		VlozDoListu(zaznam);
	}
}

void Rstrom::VlozZaznam(int x, int y){
	Rzaznam *zaznam = new Rzaznam(x, y);
	Koren->VlozZaznam(zaznam);
}

void Rstrom::VlozUzelDoRodice(Rstrom *RPotomek){
	int porovnani;
	if (pocetZaznamu == K){
		//je potreba list rozdelit na dva
		RozdelUzel(RPotomek);
		return;
	}
	Potomci[pocetZaznamu] = RPotomek;
	if (RPotomek != NULL){
		RPotomek->Rodic = this;
	}
	//uprava hranic pokud je treba
	ZkontrolujHranici(RPotomek->l, RPotomek->r, RPotomek->u, RPotomek->d);
}

/* TODO */
void Rstrom::VlozDoUzlu(Rzaznam *zaznam){
	int porovnani;

	/* TO DO */
	//najdi zaznam F tak, ze ObdelnikIndexovehoZaznamuF potrebuje nejmensi rozsireni, 
	//aby OIZNovehoZaznamu byl podmnizinou OIZF. Prednost ma zaznam s mensim obdelnikem
	//do N uloz idF
}

void Rstrom::VlozDoListu(Rzaznam *zaznam){
	if (pocetZaznamu == K){
		//je potreba list rozdelit na dva
		RozdelList(zaznam);
		return;
	}
	//pokud je misto, tak to tam vlozim
	Zaznamy[pocetZaznamu] = zaznam;
	pocetZaznamu++;
	//je potreba upravit hranice mbr
	ZkontrolujHranici(zaznam->x, zaznam->y);
}

void Rstrom::ZkontrolujHranici(int x, int y){
	ZkontrolujHranici(x, x, y, y);
}

void Rstrom::ZkontrolujHranici(int l, int r, int u, int d){
	bool zmena = false;

	if (this->l > l){
		PosunHranici(vlevo, l);
		zmena = true;
	}
	if (this->r < r){
		PosunHranici(vpravo, r);
		zmena = true;
	}
	if (this->u < u){
		PosunHranici(nahoru, u);
		zmena = true;
	}
	if (this->d > d){
		PosunHranici(dolu, d);
		zmena = true;
	}

	//pokud zmena, tak musim prepocitat obsah mbr
	if (zmena){
		mbr = VypocitejObsah(this->l, this->d, this->r, this->u);
		//kdyz posouvam hranici, tak musim zkontrolovat jestli netreba posunout i rodice
		if (Rodic != NULL){
			this->Rodic->ZkontrolujHranici(this->l, this->r, this->u, this->d);
		}
	}
}

void Rstrom::PosunHranici(strana s, int hodnota){
	switch (s){
		case vlevo:
			this->l = hodnota;
			break;
		case vpravo:
			this->r = hodnota;
			break;
		case nahoru:
			this->u = hodnota;
			break;
		case dolu:
			this->d = hodnota;
			break;
	}
}

/* TODO prvek se pridava hned a pak se teprve rozdeluje... */
void Rstrom::RozdelList(Rzaznam *zaznam){
	//Pro kazdy podstrom vybereme jeden zaznam tak, aby byly co nejvice odlisne
	int levy = 0;
	int pravy = 0;
	//KvadratickySplit
	VyberDvaZaznamy(levy, pravy);
	//levy zustava, pravy presunu

	Rstrom *novyList = new Rstrom(Zaznamy[pravy]);

	//zjistim na kterou stranu budou jednotlive zaznamy lepe pasovat
	int ll = Zaznamy[levy]->x;
	int lr = Zaznamy[levy]->x;
	int lu = Zaznamy[levy]->y;
	int ld = Zaznamy[levy]->y;
	int mbrl = 0;
	int zmenal[K];
	int obsahl = 0;
	int pocetl = 1;
	int lx = 0;
	int ly = 0;

	int rl = Zaznamy[pravy]->x;
	int rr = Zaznamy[pravy]->x;
	int ru = Zaznamy[pravy]->y;
	int rd = Zaznamy[pravy]->y;
	int mbrr = 0;
	int zmenar[K];
	int obsahr = 0;
	int pocetr = 1;
	int rx = 0;
	int ry = 0;

	//je treba vypocitat jak moc zalezi do ktereho listu ktery zaznam umistim.
	//ty, u kterych je rozdil nejvyssi, pak budu prirazovat do prislusnych listu nejdrive
	int rozdily[K];
	rozdily[levy] = 0;
	rozdily[pravy] = 0;

	for (int i = 0; i < K; i++){
		//prochazim jen zaznamy ktere jsem jeste nepriradil
		if (i != levy && i != pravy){
			//posunuji vlevo
			if (Zaznamy[i]->x < ll){
				lx = ll;
			}
			//posunuji vpravo nebo vubec
			else{
				lx = lr;
			}
			//posunuji dolu
			if (Zaznamy[i]->y < ld){
				ly = ld;
			}
			//posunuji nahoru nebo vubec
			else{
				ly = lu;
			}
			//-------------------------------------------------
			//posunuji vlevo
			if (Zaznamy[i]->x < rl){
				rx = rl;
			}
			//posunuji vpravo nebo vubec
			else{
				rx = rr;
			}
			//posunuji dolu
			if (Zaznamy[i]->y < rd){
				ry = rd;
			}
			//posunuji nahoru nebo vubec
			else{
				ry = ru;
			}
			//-------------------------------------------------
			obsahl = VypocitejObsah(lx, ly, Zaznamy[i]->x, Zaznamy[i]->y);
			zmenal[i] = obsahl - mbr;
			if (zmenal[i] < 0){
				zmenal[i] = -1 * zmenal[i];
			}
			obsahr = VypocitejObsah(rx, ry, Zaznamy[i]->x, Zaznamy[i]->y);
			zmenar[i] = obsahr - mbrr;
			if (zmenar < 0){
				zmenar[i] = -1 * zmenar[i];
			}

			rozdily[i] = zmenal[i] - zmenar[i];
			//absolutni hodnota
			if (rozdily[i] < 0){
				rozdily[i] = -1 * rozdily[i];
			}
		} //konec if (i != levy && i != pravy)
	} //konec for (int i = 0; i < K; i++)

	int poradi[K];
	int nejvetsi = rozdily[0];
	//urci poradi pro kazdy zaznam v zavislosti na dulezitosti vyberu listu
	//do ktereho dany zaznam bude patrit
	for (int i = 0; i < K; i++){
		for (int j = 0; j < K; j++){
			if (rozdily[j] > nejvetsi){
				poradi[i] = j;
				rozdily[j] = 0;
			}
		}
	}
	//podle dulezitosti prirazuji prvky
	for (int p = 0; p < K; p++){
		int i = poradi[p];
		//preskocim 2 prvky ktery uz v jednom z listu jsou
		if ((i == levy) || (i == pravy)){
			continue;
		}
		//aby byla dodrzena podminka minimalniho poctu zaznamu v kazdem listu...
		if (p - pocetl == m){
			//zbytek musi prijit vlevo
			pocetl++;
		}
		else if (p - pocetr == m){
			//zbytek musi prijit vpravo
			novyList->VlozZaznam(Zaznamy[i]);
			//je treba jej zleva odstranit
			Zaznamy[i] = NULL;
			pocetr++;
		}
		//porovnani obsahu a prirazeni zaznamu
		//vkladam vlevo
		if (zmenal < zmenar){
			//netreba vkladat vlevo - zaznam zde jiz je
			pocetl++;
		}
		//vkladam vpravo
		else if (zmenal > zmenar){
			novyList->VlozZaznam(Zaznamy[i]);
			//je treba jej zleva odstranit
			Zaznamy[i] = NULL;
			pocetr++;
		}
		//zmena bude stejna - vyberu tridu s mensim obdelnikem
		else{
			//vkladam vlevo
			if (obsahl < obsahr){
				//netreba vkladat vlevo - zaznam zde jiz je
				pocetl++;
			}
			//vkladam vpravo
			else if (obsahl > obsahr){
				novyList->VlozZaznam(Zaznamy[i]);
				//je treba jej zleva odstranit
				Zaznamy[i] = NULL;
				pocetr++;
			}
			//pokud i obsahy jsou stejne, pak vkladam do toho listu, kde je mene zaznamu
			else{
				//vkladam vpravo
				if (this->pocetZaznamu > novyList->pocetZaznamu){
					novyList->VlozZaznam(Zaznamy[i]);
					//je treba jej zleva odstranit
					Zaznamy[i] = NULL;
					pocetr++;
				}
				//netreba vkladat vlevo - zaznam zde jiz je
				pocetl++;
			}
		}
	}
	PreskladejZaznamy();
	//vysledny obdelnik bude mit po preskladani velikost toho leveho
	pocetZaznamu = pocetl;
	mbr = mbrl;

	//TO DO je potreba nahlasit rodicovi ze je tu o potomka vic
	if (Rodic != NULL){
		novyList->Rodic = Rodic;
		Rodic->VlozUzelDoRodice(novyList);
		Rodic->ZkontrolujHranici(this->l, this->r, this->u, this->d);
		Rodic->ZkontrolujHranici(novyList->l, novyList->r, novyList->u, novyList->d);
	}
	else{
		//delim koren
		Rodic = new Rstrom(this, novyList);
		Koren = Rodic;
	}
}

void Rstrom::PreskladejZaznamy(){
	bool zmena = false;
	for (int i = pocetZaznamu-1; i >= 0; i--){
		if (Zaznamy[i] != NULL){
			for (int j = 0; j < i; j++){
				if (Zaznamy[j] == NULL){
					Zaznamy[j] = Zaznamy[i];
					Zaznamy[i] = NULL;
					zmena = true;
				}
			}
			//vse je jiz preskladano
			if (!zmena){
				return;
			}
		}
	}
}

int Rstrom::VypocitejObsah(int ax, int ay, int bx, int by){
	int stranaA = ax - bx;
	int stranaB = ay - by;
	int obsah = stranaA * stranaB;
	//je treba vzit absolutni hodnotu z obsahu
	if (obsah < 0){
		obsah = -1 * obsah;
	}
	return obsah;
}

void Rstrom::VyberDvaZaznamy(int &l, int &r){
	int poleObdelniku[K][K];
	int nejvetsiObsah = 0;
	int obsah = 0;
	for (int i = 0; i < K; i++){
		for (int j = 0; j < K; j++){
			obsah = VypocitejObsah(Zaznamy[i]->x, Zaznamy[i]->y, Zaznamy[j]->x, Zaznamy[j]->y);
			
			if (obsah > nejvetsiObsah){
				nejvetsiObsah = obsah;
				l = i;
				r = j;
			}
		}
	}
}

/* TODO */
void Rstrom::RozdelUzel(Rstrom *RPotomek){

}

bool Rstrom::Vyhledej(int x, int y){
	Rzaznam *z = new Rzaznam(x, y);
	/*if (Vyhledej(z)){
		printf("Nalezeno %s\n", z->text);
		return true;
	}
	else{
		printf("NENalezeno %s\n", z->text);
		return false;
	}
	*/
	return Koren->Vyhledej(z);
}

/* TODO */
bool Rstrom::Vyhledej(Rzaznam *z){
	if (JeStromList()){
		for (int i = 0; i < pocetZaznamu; i++){

		}
		//printf("NENalezeno %s (> %s)\n", z->text, Zaznamy[pocetZaznamu-1]);
		return false;
	}
	else{
		for (int i = 0; i < pocetZaznamu; i++){

		}
		//printf("%s < %s\n", Zaznamy[pocetZaznamu-1], z->text);
		return Potomci[pocetZaznamu]->Vyhledej(z);
	}
}

bool Rstrom::JeStromList(){
	if (Potomci[0] == NULL){
		return true;
	}
	return false;
}

void Rstrom::VypisPolozky(Rstrom *strom){
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
			printf("%d, %d\n", strom->Zaznamy[i]->x, strom->Zaznamy[i]->y);
		}
		printf("MBR: %d", strom->mbr);
	}
}

void Rstrom::Vypis(){
	VypisPolozky(Koren);
}

/* TODO */
void Rstrom::UkazStrom(){
	printf("Koren: \n");
	Koren->VypisZaznamySPotomky();
	if (Koren->Potomci[0] != NULL){
		for (int i = 0; i < Koren->pocetZaznamu + 1; i++){
			Koren->Potomci[i]->VypisZaznamySPotomky();
		}
	}
}

/* TODO */
void Rstrom::VypisZaznamySPotomky(){
	for (int i = 0; i < pocetZaznamu; i++){
		printf("Cislo zaznamu: %d, zaznam: %d, %d\n", i, Zaznamy[i]->x, Zaznamy[i]->y);
		/*if (Zaznamy[i]->Soused != NULL){
		printf(" - Soused: %s\n", Zaznamy[i]->Soused->text);
		}*/
	}
	for (int i = 0; i < K + 1; i++){
		if (Potomci[i] != NULL){
			for (int j = 0; j < Potomci[i]->pocetZaznamu; j++){
				printf("Potomek %d: %d, %d\n", i, Potomci[i]->Zaznamy[j]->x, Potomci[i]->Zaznamy[j]->y);
			}
		}
	}
	printf("\n");
}