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

Rstrom::Rstrom(Rstrom *Potomek){
	Potomci[0] = Potomek;
	Potomci[0]->Rodic = this;
	this->l = Potomci[0]->l;
	this->r = Potomci[0]->r;
	this->d = Potomci[0]->d;
	this->u = Potomci[0]->u;
	mbr = 0;
}

Rstrom::Rstrom(Rstrom *LPotomek, Rstrom *RPotomek){
	//Toto je novy koren
	Rodic = NULL;
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
	pocetZaznamu = 2;
}

void Rstrom::inicializace(){
	Rodic = NULL;
	for (int i = 0; i < K; i++){
		Potomci[i] = NULL;
		Zaznamy[i] = NULL;
	}
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

void Rstrom::VlozUzel(Rstrom *RPotomek){
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
	pocetZaznamu++;
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
	for (int i = 0; i < pocetZaznamu; i++){
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

		zmena = false;
		leva = this->l;
		prava = this->r;
		horni = this->u;
		dolni = this->d;
	}
	int nejmensiRozdil = Zmeny[0];
	int IndexNejmensihoPotomka = 0;
	//Vyberu potomka, ktery bude muset nejmene menit obdelnik
	for (int i = 1; i < pocetZaznamu; i++){
		if (Zmeny[i] < nejmensiRozdil){
			nejmensiRozdil = Zmeny[i];
			IndexNejmensihoPotomka = i;
		}
		//remizu resim vyberem zaznamu s mensim obdelnikem
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
		this->d = d;
		zmena = true;
	}
	return zmena;
}

void Rstrom::VypoctiNoveHranice(int &l, int &r, int &u, int &d, int x, int y){
	if (x < l){
		l = x;
	}
	//posunuji vpravo nebo vubec
	else if(x > r){
		r = x;
	}
	//posunuji dolu
	if (y > u){
		u = y;
	}
	//posunuji nahoru nebo vubec
	else if(y < d){
		d = y;
	}
}

int Rstrom::DalsiPrvek(Rstrom *strom, bool prepocitatLevy, bool prepocitatPravy, int zmenal[], int zmenar[]){

	//ulozeni hranic do promennych 
	int ll = this->l;
	int lr = this->r;
	int lu = this->u;
	int ld = this->d;

	int rl = strom->l;
	int rr = strom->r;
	int ru = strom->u;
	int rd = strom->d;

	//je treba vypocitat jak moc zalezi do ktereho listu ktery zaznam umistim.
	//ty, u kterych je rozdil nejvyssi, pak budu prirazovat do prislusnych listu nejdrive
	int rozdily[K - 1];

	//i < K-1 proto, protoze K-1 je prvni levy prvek a K je prvni pravy prvek
	for (int i = 0; i < K - 1; i++){
		//pokud je prvek jiz do nejakeho pole prirazen, ma x-ovou souradnici rovnu -1
		if (zmenal[i] != -1){
			//kdyz bych posunul vlevo
			if (prepocitatLevy){
				VypoctiNoveHranice(ll, lr, lu, ld, PomocnyPole[i]->x, PomocnyPole[i]->y);
				zmenal[i] = VypocitejObsah(ll, lu, lr, ld) - this->mbr;
			}
			//kdyz vpravo
			if (prepocitatPravy){
				VypoctiNoveHranice(rl, rr, ru, rd, PomocnyPole[i]->x, PomocnyPole[i]->y);
				zmenar[i] = VypocitejObsah(rl, ru, rr, rd) - strom->mbr;
			}
			//-------------------------------------------------
			//tady lze poznat jestli pujde prvek vlevo ci vpravo, nechci ale pouzivat dalsi parametr kvuli prehlednosti
			rozdily[i] = zmenal[i] - zmenar[i]; 
			//absolutni hodnota
			if (rozdily[i] < 0){
				rozdily[i] = -1 * rozdily[i];
			}
		}
		else{
			rozdily[i] = -1;
		}
	} //konec for (int i = 0; i < K-1; i++)

	int max = rozdily[0];
	int maxIndex = 0;
	for (int i = 1; i < K - 1; i++){
		if (rozdily[i] > max){
			max = rozdily[i];
			maxIndex = i;
 		}
	}
	return maxIndex;
}

void Rstrom::RozdelList(Rzaznam *zaznam){
	//Pro kazdy podstrom vybereme jeden zaznam tak, aby byly co nejvice odlisne
	int levy = 0;
	int pravy = 0;
	//KvadratickySplit-------------------------------------------------------------------
	//zaznamy si ulozim do pomocneho pole, smazu a z tohoto pomocneho pole budu zaznamy
	//znovu pridavat do obou vetvi. 
			//Druha moznost by byla presunout zaznamy co maji zustat tak doleva
			//upravit pocet prvku, zmensit hranice MBR a prepocitat obsah - tedy mbr
	for (int i = 0; i < K; i++){
		PomocnyPole[i] = this->Zaznamy[i];
		this->Zaznamy[i] = NULL;
	}
	//nasledujici vypocty budou probihat i snove vkladanym zaznamem
	PomocnyPole[K] = zaznam;

	VyberDvaZaznamy(levy, pravy);
	//tyto dva prvky presunu do pole vpravo
	Rzaznam *pom = PomocnyPole[levy];
	PomocnyPole[levy] = PomocnyPole[K - 1];
	PomocnyPole[K - 1] = pom;
	levy = K - 1; //!!!

	pom = PomocnyPole[pravy];
	PomocnyPole[pravy] = PomocnyPole[K];
	PomocnyPole[K] = pom;
	pravy = K; //!!!

	//novy hranice tohoto listu
	this->l = this->r = PomocnyPole[K - 1]->x;
	this->u = this->d = PomocnyPole[K - 1]->y;
	this->pocetZaznamu = 1;

	//levy zustava v tomto listu, pravy presunu do noveho
	Zaznamy[0] = PomocnyPole[levy];
	Rstrom *novyList = new Rstrom(PomocnyPole[pravy]);

	//zjistim na kterou stranu budou jednotlive zaznamy lepe pasovat
	int zmenal[K - 1];
	int pocetl = 1;
	bool prepocitatLevy = true;
	int zmenar[K - 1];
	int pocetr = 1;
	bool prepocitatPravy = true;

	int maxIndex = 0;
	//podle dulezitosti prirazuji prvky
	for (int p = 0; p < K-1; p++){
		//aby byla dodrzena podminka minimalniho poctu zaznamu v kazdem listu...
		maxIndex = DalsiPrvek(novyList, prepocitatLevy, prepocitatPravy, zmenal, zmenar);
		prepocitatLevy = false;
		prepocitatPravy = false;
		if (p - pocetl == m-1){
			//zbytek musi prijit vlevo
			VlozZaznam(PomocnyPole[maxIndex]);
			pocetl++;
			prepocitatLevy = true;
			continue;
		}
		else if (p - pocetr == m-1){
			//zbytek musi prijit vpravo
			novyList->VlozZaznam(PomocnyPole[maxIndex]);
			pocetr++;
			prepocitatPravy = true;
			continue;
		}

		//porovnani obsahu a prirazeni zaznamu
		//vkladam vlevo
		if (zmenal[maxIndex] < zmenar[maxIndex]){
			VlozZaznam(PomocnyPole[maxIndex]);
			pocetl++;
			prepocitatLevy = true;
		}
		//vkladam vpravo
		else if (zmenal[maxIndex] > zmenar[maxIndex]){
			novyList->VlozZaznam(PomocnyPole[maxIndex]);
			pocetr++;
			prepocitatPravy = true;
		}
		//zmena bude stejna - vyberu tridu s mensim obdelnikem
		else{
			//vkladam vlevo
			if (mbr < novyList->mbr){
				VlozZaznam(PomocnyPole[maxIndex]);
				pocetl++;
				prepocitatLevy = true;
			}
			//vkladam vpravo
			else if (mbr > novyList->mbr){
				novyList->VlozZaznam(PomocnyPole[maxIndex]);
				pocetr++;
				prepocitatPravy = true;
			}
			//pokud i obsahy jsou stejne, pak vkladam do toho listu, kde je mene zaznamu
			else{
				//vkladam vpravo
				if (this->pocetZaznamu > novyList->pocetZaznamu){
					novyList->VlozZaznam(PomocnyPole[maxIndex]);
					pocetr++;
					prepocitatPravy = true;
				}
				VlozZaznam(PomocnyPole[maxIndex]);
				pocetl++;
				prepocitatLevy = true;
			}
		}
		//tento zaznam jiz nebude vybran
		zmenal[maxIndex] = -1; 
		zmenar[maxIndex] = -1;
	}

	if (Rodic != NULL){
		Rodic->VlozUzel(novyList);
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

void Rstrom::VyberDvaUzly(int &l, int &r){
	int nejvetsiObsah = 0;
	int obsah = 0;
	for (int i = 0; i < K + 1; i++){
		for (int j = 0; j < K + 1; j++){
			//PomocnyPole obsahuje veskere Potomky + prave vkladany uzel
			obsah = VypocitejObsah(PomocnyPoleUzlu[i]->l, PomocnyPoleUzlu[i]->d, PomocnyPoleUzlu[j]->r, PomocnyPoleUzlu[j]->u);
			if (obsah > nejvetsiObsah){
				nejvetsiObsah = obsah;
				l = i;
				r = j;
			}
		}
	}
}

void Rstrom::RozdelUzel(Rstrom *RPotomek){
	//Pro kazdy podstrom vybereme jeden uzel tak, aby byly co nejvice odlisne
	int levy = 0;
	int pravy = 0;
	//KvadratickySplit-------------------------------------------------------------------
		//upravit pocet prvku, zmensit hranice MBR a prepocitat obsah - tedy mbr
	for (int i = 0; i < K; i++){
		PomocnyPoleUzlu[i] = this->Potomci[i];
		this->Potomci[i] = NULL;
	}
	PomocnyPoleUzlu[K] = RPotomek;
	VyberDvaUzly(levy, pravy);
	//levy zustava, pravy presunu

	Rstrom *novyUzel = new Rstrom(PomocnyPoleUzlu[pravy]);

	//zjistim na kterou stranu budou jednotlive zaznamy lepe pasovat
	int ll = this->l = PomocnyPoleUzlu[levy]->l;
	int lr = this->r = PomocnyPoleUzlu[levy]->r;
	int lu = this->u = PomocnyPoleUzlu[levy]->u;
	int ld = this->d = PomocnyPoleUzlu[levy]->d;
	int mbrl = this->mbr = 0;
	int pocetl = this->pocetZaznamu = 1;
	int zmenal[K + 1];
	int obsahl = 0;

	int rl = PomocnyPoleUzlu[pravy]->l;
	int rr = PomocnyPoleUzlu[pravy]->r;
	int ru = PomocnyPoleUzlu[pravy]->u;
	int rd = PomocnyPoleUzlu[pravy]->d;
	int mbrr = 0;
	int pocetr = 1;
	int zmenar[K + 1];
	int obsahr = 0;

	//je treba vypocitat jak moc zalezi do ktereho listu ktery zaznam umistim.
	//ty, u kterych je rozdil nejvyssi, pak budu prirazovat do prislusnych listu nejdrive
	int rozdily[K + 1];
	rozdily[levy] = 0;
	rozdily[pravy] = 0;

	for (int i = 0; i < K + 1; i++){
		//prochazim jen zaznamy ktere jsem jeste nepriradil
		if (i != levy && i != pravy){
			//posunuji vlevo
			if (PomocnyPoleUzlu[i]->l < ll){
				ll = PomocnyPoleUzlu[i]->l;
			}
			//posunuji vpravo
			if (PomocnyPoleUzlu[i]->r > lr){
				lr = PomocnyPoleUzlu[i]->r;
			}
			//posunuji dolu
			if (PomocnyPoleUzlu[i]->d < ld){
				ld = PomocnyPoleUzlu[i]->d;
			}
			//posunuji nahoru
			if(PomocnyPoleUzlu[i]->u > lu){
				lu = PomocnyPoleUzlu[i]->u;
			}
			//-------------------------------------------------
			//posunuji vlevo
			if (PomocnyPoleUzlu[i]->l < rl){
				rl = PomocnyPoleUzlu[i]->l;
			}
			//posunuji vpravo
			if (PomocnyPoleUzlu[i]->r > rr){
				rr = PomocnyPoleUzlu[i]->r;
			}
			//posunuji dolu
			if (PomocnyPoleUzlu[i]->d < rd){
				rd = PomocnyPoleUzlu[i]->d;
			}
			//posunuji nahoru
			if (PomocnyPoleUzlu[i]->u > ru){
				ru = PomocnyPoleUzlu[i]->u;
			}
			//-------------------------------------------------
			obsahl = VypocitejObsah(ll, ld, lr, lu);
			zmenal[i] = obsahl - mbr;
			if (zmenal[i] < 0){
				zmenal[i] = -1 * zmenal[i];
			}
			obsahr = VypocitejObsah(rl, rd, rr, ru);
			zmenar[i] = obsahr - mbrr;
			if (zmenar[i] < 0){
				zmenar[i] = -1 * zmenar[i];
			}

			rozdily[i] = zmenal[i] - zmenar[i];
			//absolutni hodnota
			if (rozdily[i] < 0){
				rozdily[i] = -1 * rozdily[i];
			}

			//znovu je treba obnovit minimalni hranice ktere jsou potreba
			int ll = PomocnyPoleUzlu[levy]->l;
			int lr = PomocnyPoleUzlu[levy]->r;
			int lu = PomocnyPoleUzlu[levy]->u;
			int ld = PomocnyPoleUzlu[levy]->d;

			int rl = PomocnyPoleUzlu[pravy]->l;
			int rr = PomocnyPoleUzlu[pravy]->r;
			int ru = PomocnyPoleUzlu[pravy]->u;
			int rd = PomocnyPoleUzlu[pravy]->d;
		} //konec if (i != levy && i != pravy)
	} //konec for (int i = 0; i < K; i++)

	int poradi[K + 1];
	int nejvetsi = rozdily[0];
	//urci poradi pro kazdy zaznam v zavislosti na dulezitosti vyberu listu
	//do ktereho dany zaznam bude patrit
	for (int i = 0; i < K + 1; i++){
		for (int j = 0; j < K + 1; j++){
			if (rozdily[j] > nejvetsi){
				poradi[i] = j;
				rozdily[j] = 0;
			}
		}
	}
	//podle dulezitosti prirazuji prvky
	for (int p = 0; p < K + 1; p++){
		int i = poradi[p];
		//preskocim 2 prvky ktery uz v jednom z listu jsou
		if ((i == levy) || (i == pravy)){
			continue;
		}
		//aby byla dodrzena podminka minimalniho poctu zaznamu v kazdem listu...
		if (p - pocetl == m){
			//zbytek musi prijit vlevo
			VlozUzel(PomocnyPoleUzlu[i]);
			pocetl++;
		}
		else if (p - pocetr == m){
			//zbytek musi prijit vpravo
			novyUzel->VlozUzel(PomocnyPoleUzlu[i]);
			pocetr++;
		}
		//porovnani obsahu a prirazeni zaznamu
		//vkladam vlevo
		if (zmenal[i] < zmenar[i]){
			VlozUzel(PomocnyPoleUzlu[i]);
			pocetl++;
		}
		//vkladam vpravo
		else if (zmenal[i] > zmenar[i]){
			novyUzel->VlozUzel(PomocnyPoleUzlu[i]);
			pocetr++;
		}
		//zmena bude stejna - vyberu tridu s mensim obdelnikem
		else{
			//vkladam vlevo
			if (obsahl < obsahr){
				VlozUzel(PomocnyPoleUzlu[i]);
				pocetl++;
			}
			//vkladam vpravo
			else if (obsahl > obsahr){
				novyUzel->VlozUzel(PomocnyPoleUzlu[i]);
				pocetr++;
			}
			//pokud i obsahy jsou stejne, pak vkladam do toho listu, kde je mene zaznamu
			else{
				//vkladam vpravo
				if (this->pocetZaznamu > novyUzel->pocetZaznamu){
					novyUzel->VlozUzel(PomocnyPoleUzlu[i]);
					pocetr++;
				}
				VlozUzel(PomocnyPoleUzlu[i]);
				pocetl++;
			}
		}
	}

	if (Rodic != NULL){
		Rodic->VlozUzel(novyUzel);
	}
	else{
		//delim koren
		Rodic = new Rstrom(this, novyUzel);
		Koren = Rodic;
	}
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

bool Rstrom::Vyhledej(Rzaznam *z){
	if (JeStromList()){
		for (int i = 0; i < pocetZaznamu; i++){
			if (Zaznamy[i]->x == z->x && Zaznamy[i]->y == z->y){
				printf("Nalezeno");
				return true;
			}
		}
		printf("NENalezeno\n");
		return false;
	}
	else{
		bool result = false;
		for (int i = 0; i < pocetZaznamu; i++){
			//pokud je hledany zaznam uvnitr obdelniku tohoto podstromu, volam v danem podstromu metodu Vyhledej
			if (Potomci[i]->d < z->y && Potomci[i]->u < z->y && Potomci[i]->l < z->x && Potomci[i]->r > z->x){
				result = result || Potomci[i]->Vyhledej(z);
			}
		}
		return result;
	}
}

bool Rstrom::JeStromList(){
	if (Potomci[0] == NULL){
		return true;
	}
	return false;
}

void Rstrom::VypisPolozky(){
	if (this->Potomci[0] != NULL){
		for (int i = 0; i < this->pocetZaznamu; i++){
			//podminka pro nejpravejsi potomky...
			if (this->Potomci[i] != NULL){
				this->Potomci[i]->VypisPolozky();
			}
		}
	}
	else{
		for (int i = 0; i < this->pocetZaznamu; i++){
			printf("x: %d, y: %d\n", this->Zaznamy[i]->x, this->Zaznamy[i]->y);
		}
		printf("l: %d, r: %d, u: %d, d: %d\n", this->l, this->r, this->u, this->d);
	}
}

void Rstrom::Vypis(){
	Koren->VypisPolozky();
}

void Rstrom::UkazStrom(){
	printf("Koren: \n");
	Koren->VypisZaznamySPotomky(0);
}

void Rstrom::VypisZaznamySPotomky(int hloubka){
	if (Potomci[0] != NULL){
		//na zacatku pocet mezer urcuje zanoreni
		for (int i = 0; i < hloubka; i++){
			printf(" ");
		}
		printf("%d) l: %d, r: %d, u: %d, d: %d\n", hloubka, l, r, u, d);
		for (int i = 0; i < pocetZaznamu; i++){
			if (Potomci[i] != NULL){
				Potomci[i]->VypisZaznamySPotomky(hloubka+1);
			}
		}
	}
	else{
		for (int i = 0; i < pocetZaznamu; i++){
			printf("Zaznam: %d -> x: %d, y: %d\n", i, Zaznamy[i]->x, Zaznamy[i]->y);
		}
	}
}