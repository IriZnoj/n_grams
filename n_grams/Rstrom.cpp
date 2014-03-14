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
	if (pocetZaznamu == K){
		//je potreba uzel rozdelit na dva
		RozdelUzel(RPotomek);
		return;
	}
	Potomci[pocetZaznamu++] = RPotomek;
	if (RPotomek != NULL){
		RPotomek->Rodic = this;
	}
	//uprava hranic pokud je treba
	ZkontrolujHranici(RPotomek->l, RPotomek->r, RPotomek->u, RPotomek->d);
}

void Rstrom::VlozDoUzlu(Rzaznam *zaznam){

	//"najdi zaznam F tak, ze ObdelnikIndexovehoZaznamuF potrebuje nejmensi rozsireni, 
	//aby OIZNovehoZaznamu byl podmnozinou OIZF. Prednost ma zaznam s mensim obdelnikem
	//do N uloz idF"
	//pro vsechny potomky vypocitam zmenu
	bool zmena = false;
	int leva = this->l;
	int prava = this->r;
	int horni = this->u;
	int dolni = this->d;
	int Zmeny[K];
	//Pro vsechny potomky vypocitam velikost zmeny potrebne pro pridani prvku "zaznam"
	for (int i = 0; i < K; i++){
		if (leva > zaznam->x){
			leva = zaznam->x;
			zmena = true;
		}
		else if (prava < zaznam->x){
			prava = zaznam->x;
			zmena = true;
		}
		if (horni < zaznam->y){
			horni < zaznam->y;
			zmena = true;
		}
		else if (dolni > zaznam->y){
			dolni > zaznam->y;
			zmena = true;
		}
		if (zmena){
			Zmeny[i] = VypocitejObsah(leva, dolni, prava, horni) - Potomci[i]->mbr;
		}
		else{
			Zmeny[i] = 0;
		}
	}
	int nejmensiRozdil = Zmeny[0];
	int IndexNejmensihoPotomka = 0;
	//Vyberu potomka, ktery bude muset nejmene menit obdelnik
	for (int i = 1; i < K; i++){
		if (Zmeny[i] < nejmensiRozdil){
			nejmensiRozdil = Zmeny[i];
			IndexNejmensihoPotomka = i;
		}
		//remizi resim vyberem zaznamu s mensim obdelnikem
		else if (Zmeny[i] == nejmensiRozdil){
			if (Potomci[i]->mbr < Potomci[IndexNejmensihoPotomka]->mbr){
				IndexNejmensihoPotomka = i;
			}
		}
	}
	//mozna bych pak v budoucnu nemusel znovu pocitat hranice a obsah,
	//ale nenapada me ted jak to efektivne vyresit
	Potomci[IndexNejmensihoPotomka]->VlozZaznam(zaznam);
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
	bool zmena = PorovnejAZmen(l, r, u, d);
	//pokud zmena, tak musim prepocitat obsah mbr
	if (zmena){
		mbr = VypocitejObsah(this->l, this->d, this->r, this->u);
		//kdyz posouvam hranici, tak musim zkontrolovat jestli netreba posunout i rodice
		if (Rodic != NULL){
			this->Rodic->ZkontrolujHranici(this->l, this->r, this->u, this->d);
		}
	}
}

bool Rstrom::PorovnejAZmen(int l, int r, int u, int d){
	bool zmena = false;
	if (this->l > l){
		this->l = l;
		zmena = true;
	}
	if (this->r < r){
		this->r = r;
		zmena = true;
	}
	if (this->u < u){
		this->u = u;
		zmena = true;
	}
	if (this->d > d){
		this->u = u;
		zmena = true;
	}
	return zmena;
}

void Rstrom::RozdelList(Rzaznam *zaznam){
	//Pro kazdy podstrom vybereme jeden zaznam tak, aby byly co nejvice odlisne
	int levy = 0;
	int pravy = 0;
	//KvadratickySplit-------------------------------------------------------------------
	//zaznamy si ulozim do pomocneho pole, smazu a z tohoto pomocneho pole budu zaznamy
	//znovu pridavat do obou vetvi. 
			//Druha moznost by byla presunout zaznamy co maji byt
			//vpravo tak doprava, pak leve zaznamy preskladat aby mezi nimi nebylo praznde misto
			//upravit pocet prvku, zmensit hranice MBR a prepocitat obsah - tedy mbr
	for (int i = 0; i < K; i++){
		PomocnyPole[i] = this->Zaznamy[i];
		this->Zaznamy[i] = NULL;
	}
	PomocnyPole[K] = zaznam;
	VyberDvaZaznamy(levy, pravy);
	//je potreba otestovat jestli by levy ci pravy zaznam nemel byt nove vkladany prvek
	//levy zustava, pravy presunu

	Rstrom *novyList = new Rstrom(PomocnyPole[pravy]);

	//zjistim na kterou stranu budou jednotlive zaznamy lepe pasovat
	int ll = this->l = PomocnyPole[levy]->x;
	int lr = this->r = PomocnyPole[levy]->x;
	int lu = this->u = PomocnyPole[levy]->y;
	int ld = this->d = PomocnyPole[levy]->y;
	int mbrl = this->mbr = 0;
	int pocetl = this->pocetZaznamu = 1; 
	int zmenal[K + 1];
	int obsahl = 0;
	int lx = 0;
	int ly = 0;

	int rl = PomocnyPole[pravy]->x;
	int rr = PomocnyPole[pravy]->x;
	int ru = PomocnyPole[pravy]->y;
	int rd = PomocnyPole[pravy]->y;
	int mbrr = 0;
	int pocetr = 1;
	int zmenar[K+1];
	int obsahr = 0;
	int rx = 0;
	int ry = 0;

	//je treba vypocitat jak moc zalezi do ktereho listu ktery zaznam umistim.
	//ty, u kterych je rozdil nejvyssi, pak budu prirazovat do prislusnych listu nejdrive
	int rozdily[K+1];
	rozdily[levy] = 0;
	rozdily[pravy] = 0;

	for (int i = 0; i < K+1; i++){
		//prochazim jen zaznamy ktere jsem jeste nepriradil
		if (i != levy && i != pravy){
			//posunuji vlevo
			if (PomocnyPole[i]->x < ll){
				lx = ll;
			}
			//posunuji vpravo nebo vubec
			else{
				lx = lr;
			}
			//posunuji dolu
			if (PomocnyPole[i]->y < ld){
				ly = ld;
			}
			//posunuji nahoru nebo vubec
			else{
				ly = lu;
			}
			//-------------------------------------------------
			//posunuji vlevo
			if (PomocnyPole[i]->x < rl){
				rx = rl;
			}
			//posunuji vpravo nebo vubec
			else{
				rx = rr;
			}
			//posunuji dolu
			if (PomocnyPole[i]->y < rd){
				ry = rd;
			}
			//posunuji nahoru nebo vubec
			else{
				ry = ru;
			}
			//-------------------------------------------------
			obsahl = VypocitejObsah(lx, ly, PomocnyPole[i]->x, PomocnyPole[i]->y);
			zmenal[i] = obsahl - mbr;
			if (zmenal[i] < 0){
				zmenal[i] = -1 * zmenal[i];
			}
			obsahr = VypocitejObsah(rx, ry, PomocnyPole[i]->x, PomocnyPole[i]->y);
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

	int poradi[K+1];
	int nejvetsi = rozdily[0];
	//urci poradi pro kazdy zaznam v zavislosti na dulezitosti vyberu listu
	//do ktereho dany zaznam bude patrit
	for (int i = 0; i < K+1; i++){
		for (int j = 0; j < K+1; j++){
			if (rozdily[j] > nejvetsi){
				poradi[i] = j;
				rozdily[j] = 0;
			}
		}
	}
	//podle dulezitosti prirazuji prvky
	for (int p = 0; p < K+1; p++){
		int i = poradi[p];
		//preskocim 2 prvky ktery uz v jednom z listu jsou
		if ((i == levy) || (i == pravy)){
			continue;
		}
		//aby byla dodrzena podminka minimalniho poctu zaznamu v kazdem listu...
		if (p - pocetl == m){
			//zbytek musi prijit vlevo
			VlozZaznam(PomocnyPole[i]);
			pocetl++;
		}
		else if (p - pocetr == m){
			//zbytek musi prijit vpravo
			novyList->VlozZaznam(PomocnyPole[i]);
			pocetr++;
		}
		//porovnani obsahu a prirazeni zaznamu
		//vkladam vlevo
		if (zmenal < zmenar){
			VlozZaznam(PomocnyPole[i]);
			pocetl++;
		}
		//vkladam vpravo
		else if (zmenal > zmenar){
			novyList->VlozZaznam(PomocnyPole[i]);
			pocetr++;
		}
		//zmena bude stejna - vyberu tridu s mensim obdelnikem
		else{
			//vkladam vlevo
			if (obsahl < obsahr){
				VlozZaznam(PomocnyPole[i]);
				pocetl++;
			}
			//vkladam vpravo
			else if (obsahl > obsahr){
				novyList->VlozZaznam(PomocnyPole[i]);
				pocetr++;
			}
			//pokud i obsahy jsou stejne, pak vkladam do toho listu, kde je mene zaznamu
			else{
				//vkladam vpravo
				if (this->pocetZaznamu > novyList->pocetZaznamu){
					novyList->VlozZaznam(PomocnyPole[i]);
					pocetr++;
				}
				VlozZaznam(PomocnyPole[i]);
				pocetl++;
			}
		}
	}

	if (Rodic != NULL){
		Rodic->VlozUzelDoRodice(novyList);
	}
	else{
		//delim koren
		Rodic = new Rstrom(this, novyList);
		Koren = Rodic;
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
	int nejvetsiObsah = 0;
	int obsah = 0;
	for (int i = 0; i < K+1; i++){
		for (int j = 0; j < K+1; j++){
			//PomocnyPole obsahuje veskere Zaznamy + prave vkladany prvek
			obsah = VypocitejObsah(PomocnyPole[i]->x, PomocnyPole[i]->y, PomocnyPole[j]->x, PomocnyPole[j]->y);
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