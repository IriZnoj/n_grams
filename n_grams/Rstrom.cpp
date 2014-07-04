#include <stdlib.h>
#include <stdio.h>
#include "Rstrom.h"

//kazdy zaznam je ve tvaru (mbr, souradnice) kde
//mbr = obsah, objem, nebo neco takovyho pro vice-dimenzionalni hranice
//zjisteni velikosti --> velikost = sizeof(souradnice) / sizeof(souradnice[0])
Rzaznam::Rzaznam(int souradnice[], int velikost){
	this->mbr = NULL;
	this->dimenze = velikost;
	for (int i = 0; i < velikost; i++){
		this->souradnice[i] = souradnice[i];
	}
	//Soused = NULL;
}

Rzaznam::Rzaznam(char * retezec){
	this->mbr = NULL;
	//dimenze bude nastavena pri prevodu
	this->dimenze = 0;
	if (retezec[0] == '\n'){
		//printf("omylem stisknuty enter\n");
		return;
	}
	//prevod retezce na souradnice
	RetezecNaCisla(retezec);
	//printf("dimenze: %d\n", dimenze);
	//Soused = NULL;
}

void Rzaznam::RetezecNaCisla(char *retezec){
	int offset, cislo = 0;
	//kdyz bude predano neco jinyho nez cisla oddelena mezerami, pak se nacte jen ta cast
	//od zacatku retezce ktera je vporadku
	while (sscanf(retezec, "%d%n", &cislo, &offset) == 1){
		this->souradnice[this->dimenze++] = cislo;
		retezec += offset;
		if (dimenze >= D){
			//printf("dimenze %d >= D\n", dimenze);
			return;
		}
		//printf("cislo: %d\n", cislo);
	}
}

Rstrom::Rstrom(){
	Koren = this;
	prvni = true;
}

Rstrom::Rstrom(Rzaznam *z){
	VytvoreniStromu(z);
}

Rstrom::Rstrom(char *souradnice){
	Rzaznam *z = new Rzaznam(souradnice);
	VytvoreniStromu(z);
}

Rstrom::Rstrom(Rstrom *potomek){
	Potomci[0] = potomek;
	Potomci[0]->Rodic = this;
	this->Rodic = NULL;
	//strom navazu po ukonceni rozdeleni uzlu
	pocetZaznamu = 1;
	//hranice v prave vytvarenem stromu jsou stejne jako v potomkovi
	dimenze = potomek->dimenze;
	for (int i = 0; i < dimenze; i++){
		hranice[i][0] = potomek->hranice[i][0]; //min
		hranice[i][1] = potomek->hranice[i][1]; //max
	}
	mbr = potomek->mbr;
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
	dimenze = Max(LPotomek->dimenze, RPotomek->dimenze);
	for (int i = 0; i < dimenze; i++){
		//prirazena je bud vetsi souradnice, nebo ta co zapada do velikosti dimenze daneho potomka
		hranice[i][0] = MensiSouradnice(LPotomek, RPotomek, i);
		hranice[i][1] = VetsiSouradnice(LPotomek, RPotomek, i);
	}

	mbr = VypocitejObsah(hranice, Min(LPotomek->dimenze, RPotomek->dimenze));
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
	//hranice v prave vytvarenem stromu jsou souradnice prvniho zaznamu
	dimenze = z->dimenze;
	for (int i = 0; i < dimenze; i++){
		hranice[i][0] = z->souradnice[i];
		hranice[i][1] = z->souradnice[i];
	}
	this->mbr = 0;
	prvni = false;
}

void Rstrom::VlozPrvniZaznam(int souradnice[], int velikost){
	Rzaznam *z = new Rzaznam(souradnice, velikost);
	
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

void Rstrom::VlozZaznam(char *souradnice){
	//TO DO chybi kontrola stringu

	Rzaznam *zaznam = new Rzaznam(souradnice);
	if (prvni){
		VytvoreniStromu(zaznam);
	}
	else{
		if (zaznam->dimenze != 0){
			if (Koren->Vyhledej(zaznam)){
				//printf("duplicita\n");
			}
			else{
				Koren->VlozZaznam(zaznam);
			}
		}
	}
}

void Rstrom::VlozZaznam(int souradnice[], int velikost){
	Rzaznam *zaznam = new Rzaznam(souradnice, velikost);
	if (prvni){
		VytvoreniStromu(zaznam);
	}
	else{
		if (zaznam->dimenze != 0){
			if (Koren->Vyhledej(zaznam)){
				//printf("duplicita\n");
			}
			else{
				Koren->VlozZaznam(zaznam);
			}
		}
	}
}

void Rstrom::VlozUzel(Rstrom *potomek){
	potomek->Rodic = this;
	if (pocetZaznamu == K){
		//je potreba uzel rozdelit na dva
		RozdelUzel(potomek);
		return;
	}
	Potomci[pocetZaznamu] = potomek;
	pocetZaznamu++;
	//uprava hranic pokud je treba
	ZkontrolujHranici(potomek->hranice, potomek->dimenze);
}

void Rstrom::VlozDoUzlu(Rzaznam *zaznam){
	//"najdi zaznam F tak, ze ObdelnikIndexovehoZaznamuF potrebuje nejmensi rozsireni, 
	//aby OIZNovehoZaznamu byl podmnozinou OIZF. Prednost ma zaznam s mensim obdelnikem
	//do N uloz idF"
	//pro vsechny potomky vypocitam zmenu
	bool zmena = false;
	int pom_souradnice[D][2];
	 long long Zmeny[K];
	//Pro vsechny potomky vypocitam velikost zmeny potrebne pro pridani prvku "zaznam"
	for (int i = 0; i < pocetZaznamu; i++){
		for (int j = 0; j < Min(Potomci[i]->dimenze, zaznam->dimenze); j++){
			//inicializace prom. na hodnotu aktualnich hranic mbr uzlu
			pom_souradnice[j][0] = Potomci[i]->hranice[j][0];
			pom_souradnice[j][1] = Potomci[i]->hranice[j][1];
			//pokud jsou souradnice nove pridavaneho prvku mimo rozsah hranic aktualniho uzlu
			//pak v pripade pridani je treba hranice rozsirit
			if (pom_souradnice[j][0] > zaznam->souradnice[j]){
				pom_souradnice[j][0] = zaznam->souradnice[j];
				zmena = true;
			}
			else if (pom_souradnice[j][1] < zaznam->souradnice[j]){
				pom_souradnice[j][1] = zaznam->souradnice[j];
				zmena = true;
			}
		}
		//pokud ma vkladany vetsi dimenzi...
		if (zaznam->dimenze > Potomci[i]->dimenze){
			for (int j = dimenze; j < zaznam->dimenze; j++){
				pom_souradnice[j][0] = zaznam->souradnice[j];
				pom_souradnice[j][1] = zaznam->souradnice[j];
			}
			//pokud bude jen vetsi dimenze s nulovymi souradnicemi, tak se sice nejedna o 
			//zmenu, ale znamena to pouze jedno vypocitani obsahu navic
			zmena = true;
		}
		//inicializace zbyvajicich souradnic
		else{
			for (int j = zaznam->dimenze; j < Potomci[i]->dimenze; j++){
				pom_souradnice[j][0] = Potomci[i]->hranice[j][0];
				pom_souradnice[j][1] = Potomci[i]->hranice[j][1];
			}
		}
		//vynulovani zbyvajicich souradnic
		for (int j = Max(Potomci[i]->dimenze, zaznam->dimenze); j < D; j++){
			pom_souradnice[j][0] = 0;
			pom_souradnice[j][1] = 0;
		}
		
		if (zmena){
			Zmeny[i] = VypocitejObsah(pom_souradnice, Max(Potomci[i]->dimenze, zaznam->dimenze)) - Potomci[i]->mbr;
		}
		else{
			Zmeny[i] = 0;
		}

		zmena = false;
	}
	 long long nejmensiRozdil = Zmeny[0];
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
	//vytvoreni novyho konstruktoru s predanim dimenze a hranic by zabranilo
	//novymu vypoctu hranic pro vkladani zaznamu
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
	ZkontrolujHranici(zaznam->souradnice, zaznam->dimenze);
}

int Rstrom::Max(int a, int b){
	if (a >= b){
		return a;
	}
	return b;
}

int Rstrom::Min(int a, int b){
	if (a <= b){
		return a;
	}
	return b;
}

int Rstrom::VetsiSouradnice(Rstrom *LPotomek, Rstrom *RPotomek, int dimenze){
	//vrati vetsi souradnici pokud obe spadaji do dimenze, pokud ne, tak vrati tu co spada
	if (dimenze < LPotomek->dimenze){
		if (dimenze < RPotomek->dimenze){
			return Max(LPotomek->hranice[dimenze][1], RPotomek->hranice[dimenze][1]);
		}
		return LPotomek->hranice[dimenze][1];
	}
	return RPotomek->hranice[dimenze][1];
}

int Rstrom::MensiSouradnice(Rstrom *LPotomek, Rstrom *RPotomek, int dimenze){
	if (dimenze < LPotomek->dimenze){
		if (dimenze < RPotomek->dimenze){
			return Min(LPotomek->hranice[dimenze][0], RPotomek->hranice[dimenze][0]);
		}
		return LPotomek->hranice[dimenze][0];
	}
	return RPotomek->hranice[dimenze][0];
}

void Rstrom::ZkontrolujHranici(int hranice[], int dimenze){
	if (PorovnejAZmen(hranice, dimenze)){
		PrepocitejObsah();
	}
}

void Rstrom::ZkontrolujHranici(int hranice[][2], int dimenze){
	//pokud zmena, tak musim prepocitat obsah mbr
	if (PorovnejAZmen(hranice, dimenze)){
		PrepocitejObsah();
	}
}

void Rstrom::PrepocitejObsah(){
	mbr = VypocitejObsah(hranice, dimenze);
	//kdyz posouvam hranici, tak musim zkontrolovat jestli netreba posunout i rodice
	if (Rodic != NULL){
		this->Rodic->ZkontrolujHranici(hranice, dimenze);
	}
}

bool Rstrom::PorovnejAZmen(int hranice[], int dimenze){
	bool zmena = false;
	for (int i = 0; i < dimenze; i++){
		//pokud je nova hranice i-teho rozmeru mensi nez soucasna spodni hranice, pak
		//je treba oznamit zmenu a hranici prepsat. Stejnym zpusobem menime i horni hranici.
		if (i >= this->dimenze){
			this->hranice[i][0] = hranice[i];
			this->hranice[i][1] = hranice[i];
			this->dimenze++;
			zmena = true;
		}
		else{
			if (hranice[i] < this->hranice[i][0]){
				this->hranice[i][0] = hranice[i];
				zmena = true;
			}
			if (hranice[i] > this->hranice[i][1]){
				this->hranice[i][1] = hranice[i];
				zmena = true;
			}
		}
	}
	return zmena;
}

bool Rstrom::PorovnejAZmen(int hranice[][2], int dimenze){
	bool zmena = false;
	for (int i = 0; i < dimenze; i++){
		if (i >= this->dimenze){
			this->hranice[i][0] = hranice[i][0];
			this->hranice[i][1] = hranice[i][1];
			this->dimenze++;
			zmena = true;
		}
		else{
			if (hranice[i][0] < this->hranice[i][0]){
				this->hranice[i][0] = hranice[i][0];
				zmena = true;
			}
			if (hranice[i][1] > this->hranice[i][1]){
				this->hranice[i][1] = hranice[i][1];
				zmena = true;
			}
		}
	}
	return zmena;
}

//puvodni = hranice, nova = prvek ktery muze (puvodni) hranice rozsirit
//vraci dimenzi rozsirene hranice
int Rstrom::VypoctiHranice(int puvodni[][2], int dim_puvodni, int nova[][2], int dim_nova){
	//upravim jen hodnoty dimenzi u kterych to jde
	int min_dimenze = Min(dim_puvodni, dim_nova);
	for (int i = 0; i < min_dimenze; i++){
		if (puvodni[i][0] > nova[i][0]){
			puvodni[i][0] = nova[i][0];
		}
		if (puvodni[i][1] < nova[i][1]){
			puvodni[i][1] = nova[i][1];
		}
	}
	//pokud je dimenze rozsirujiciho prvku vetsi nez dimenze puvodni pak se puvodni rozsiri
	if (dim_puvodni < dim_nova){
		for (int i = dim_puvodni; i < dim_nova; i++){
			puvodni[i][0] = nova[i][0];
		}
		return dim_nova;
	}
	return dim_puvodni;
}

//hranice kdy do 2-rozmerneho pole vkladam obycejne pole slozene z "dim_nova" souradnic
int Rstrom::VypoctiHranice(int puvodni[][2], int dim_puvodni, int nova[], int dim_nova){
	//upravim jen hodnoty dimenzi u kterych to jde
	int min_dimenze = Min(dim_puvodni, dim_nova);
	for (int i = 0; i < min_dimenze; i++){
		if (puvodni[i][0] > nova[i]){
			puvodni[i][0] = nova[i];
		}
		if (puvodni[i][1] < nova[i]){
			puvodni[i][1] = nova[i];
		}
	}
	//pokud je dimenze rozsirujiciho prvku vetsi nez dimenze puvodni pak se puvodni rozsiri
	if (dim_puvodni < dim_nova){
		for (int i = dim_puvodni; i < dim_nova; i++){
			puvodni[i][0] = nova[i];
			puvodni[i][1] = nova[i];
		}
		return dim_nova;
	}
	return dim_puvodni;
}

int Rstrom::VypoctiHraniceUzlu(int puvodni[][2], int dim_puvodni, Rstrom *strom){
	return VypoctiHranice(puvodni, dim_puvodni, strom->hranice, strom->dimenze);
}

int Rstrom::DalsiPrvek(Rstrom *strom, bool prepocitatLevy, bool prepocitatPravy,  long long zmenal[],  long long zmenar[]){

	//ulozeni hranic do promennych 
	int l_hranice[D][2];
	int l_dim;

	int r_hranice[D][2];
	int r_dim;

	//je treba vypocitat jak moc zalezi do ktereho listu ktery zaznam umistim.
	//ty, u kterych je rozdil nejvyssi, pak budu prirazovat do prislusnych listu nejdrive
	 long long rozdily[K - 1];

	//i < K-1 proto, protoze K-1 je prvni levy prvek a K je prvni pravy prvek
	for (int i = 0; i < K - 1; i++){
		//pokud je prvek jiz do nejakeho pole prirazen, ma x-ovou souradnici rovnu -1
		if (zmenal[i] != -1){

			//obnoveni hranic
			l_hranice[D][2];
			for (int j = 0; j < dimenze; j++){
				l_hranice[j][0] = hranice[j][0];
				l_hranice[j][1] = hranice[j][1];
			}
			for (int j = dimenze; j < D; j++){
				l_hranice[j][0] = 0;
				l_hranice[j][1] = 0;
			}
			l_dim = dimenze;

			r_hranice[D][2];
			for (int j = 0; j < strom->dimenze; j++){
				r_hranice[j][0] = strom->hranice[j][0];
				r_hranice[j][1] = strom->hranice[j][1];
			}
			for (int j = strom->dimenze; j < D; j++){
				r_hranice[j][0] = 0;
				r_hranice[j][1] = 0;
			}
			r_dim = strom->dimenze;

			//kdyz bych posunul vlevo
			if (prepocitatLevy){
				l_dim = dimenze;
				if (strom->JeStromList()){
					l_dim = VypoctiHranice(l_hranice, l_dim, PomocnyPole[i]->souradnice, PomocnyPole[i]->dimenze);
				}
				else{
					VypoctiHraniceUzlu(l_hranice, l_dim, PomocnyPoleUzlu[i]);
				}
				zmenal[i] = VypocitejObsah(l_hranice, l_dim) - this->mbr;
			}
			//kdyz vpravo
			if (prepocitatPravy){
				r_dim = strom->dimenze;
				if (strom->JeStromList()){
					r_dim = VypoctiHranice(r_hranice, r_dim, PomocnyPole[i]->souradnice, PomocnyPole[i]->dimenze);
				}
				else{
					VypoctiHraniceUzlu(r_hranice, r_dim, PomocnyPoleUzlu[i]);
				}
				zmenar[i] = VypocitejObsah(r_hranice, r_dim) - strom->mbr;
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
			//pokud byl prvek prirazen, tak ma i rozdil -1
			rozdily[i] = -1;
		}
	} //konec for (int i = 0; i < K-1; i++)

	long long max = rozdily[0];
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
	//nasledujici vypocty budou probihat i s nove vkladanym zaznamem
	PomocnyPole[K] = zaznam;

	VyberDvaZaznamy(levy, pravy);
	//tyto dva prvky presunu do pole vpravo (pod 2 nejvyssi indexy)
	Rzaznam *pom = PomocnyPole[levy];
	PomocnyPole[levy] = PomocnyPole[K - 1];
	PomocnyPole[K - 1] = pom;
	levy = K - 1; //!!!

	pom = PomocnyPole[pravy];
	PomocnyPole[pravy] = PomocnyPole[K];
	PomocnyPole[K] = pom;
	pravy = K; //!!!

	//novy hranice tohoto listu
	dimenze = PomocnyPole[levy]->dimenze;
	for (int i = 0; i < this->dimenze; i++){
		hranice[i][0] = PomocnyPole[levy]->souradnice[i];
		hranice[i][1] = PomocnyPole[levy]->souradnice[i];
	}
	pocetZaznamu = 1;
	mbr = 0;
	Zaznamy[0] = PomocnyPole[levy];

	Rstrom *novyList = new Rstrom(PomocnyPole[pravy]);

	//zjistim na kterou stranu budou jednotlive zaznamy lepe pasovat
	 long long zmenal[K - 1];
	int pocetl = 1;
	bool prepocitatLevy = true;
	 long long zmenar[K - 1];
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

 long long Rstrom::VypocitejObsah(int hranice[][2], int dimenze){
	 long long obsah = 0;
	int strana = 0;
	for (int i = 0; i < dimenze; i++){
		strana = hranice[i][1] - hranice[i][0];
		//pokud by byla strana 0, pak by vynulovala cely obsah/objem
		//pokud je n-ta dimenze u vsech zaznamu stejna, pak se do objemu pochopitelne nezahrne
		if (strana != 0){
			//pokud je obsah 0, pak jeho hodnotu navysime na 1 kvuli nasobeni
			if (obsah == 0){
				obsah = 1;
			}
			obsah *= strana;
		}
	}
	//je treba vzit absolutni hodnotu z obsahu
	if (obsah < 0){
		obsah = -1 * obsah;
	}
	return obsah;
}

void Rstrom::VyberDvaZaznamy(int &l, int &r){
	int pom_souradnice[D][2];
	int nejvetsiObsah = 0;
	int obsah = 0;
	for (int i = 0; i < K+1; i++){
		for (int k = 0; k < PomocnyPole[i]->dimenze; k++){
			pom_souradnice[k][0] = PomocnyPole[i]->souradnice[k];
		}
		//vzdy je treba pole vynulovat aby se do nej mohli zapisovat souradnice s pripadnou mensi dimenzi
		//a mohli se pouzivat vsechny souradnice - tedy i ty neutralni ( 0 ) > dimenze aktualniho zaznamu
		for (int k = PomocnyPole[i]->dimenze; k < D; k++){
			pom_souradnice[k][0] = 0;
		}
		for (int j = 0; j < K+1; j++){ //-----------------------------------------------------------------------------------min porovnani
			if (i != j){
				for (int k = 0; k < PomocnyPole[j]->dimenze; k++){
					//zde porusuji svoji konvenci ze 0 je mensi souradnice a 1 vetsi, ale je to zde vporadku.
					//Pro vypocet obsahu se bere rozdil, jednotlive rozdily (jeden pro 1 dimenzi) se 
					//spolu nasobi a vysledek se vrati v absolutni hodnote
					pom_souradnice[k][1] = PomocnyPole[j]->souradnice[k];
				}
				for (int k = PomocnyPole[j]->dimenze; k < D; k++){
					pom_souradnice[k][1] = 0;
				}
				//Min vybira minimalni dimenzi - beru v potaz jen spolecny dimenze
				obsah = VypocitejObsah(pom_souradnice, Max(PomocnyPole[i]->dimenze, PomocnyPole[j]->dimenze));
				if (obsah > nejvetsiObsah){
					nejvetsiObsah = obsah;
					l = i;
					r = j;
				}
			}
		}
	}
}

void Rstrom::VyberDvaUzly(int &l, int &r){
	int pom_souradnice[D][2];
	int nejvetsiObsah = 0;
	int obsah = 0;
	for (int i = 0; i < K + 1; i++){
		//PomocnyPole obsahuje veskere Potomky + prave vkladany uzel
		for (int k = 0; k < PomocnyPoleUzlu[i]->dimenze; k++){
			pom_souradnice[k][0] = PomocnyPoleUzlu[i]->hranice[k][0];
			pom_souradnice[k][1] = PomocnyPoleUzlu[i]->hranice[k][1];
		}
		for (int k = PomocnyPole[i]->dimenze; k < D; k++){
			pom_souradnice[k][0] = 0;
			pom_souradnice[k][1] = 0;
		}
		for (int j = 0; j < K + 1; j++){
			for (int k = 0; k < PomocnyPoleUzlu[j]->dimenze; k++){
				if (PomocnyPoleUzlu[j]->hranice[k][0] < pom_souradnice[k][0]){
					pom_souradnice[k][0] = PomocnyPoleUzlu[j]->hranice[k][0];
				}
				if (PomocnyPoleUzlu[j]->hranice[k][1] > pom_souradnice[k][1]){
					pom_souradnice[k][1] = PomocnyPoleUzlu[j]->hranice[k][1];
				}
				for (int k = Max(PomocnyPole[i]->dimenze, PomocnyPole[j]->dimenze); k < D; k++){
					pom_souradnice[k][0] = 0;
					pom_souradnice[k][1] = 0;
				}
			}
			obsah = VypocitejObsah(pom_souradnice, Min(PomocnyPole[i]->dimenze, PomocnyPole[j]->dimenze));
			if (obsah > nejvetsiObsah){
				nejvetsiObsah = obsah;
				l = i;
				r = j;
			}
		}
	}
}

void Rstrom::RozdelUzel(Rstrom *podstrom){
	//Pro kazdy podstrom vybereme jeden uzel tak, aby byly co nejvice odlisne
	int levy = 0;
	int pravy = 0;
	//KvadratickySplit-------------------------------------------------------------------
	//upravit pocet prvku, zmensit hranice MBR a prepocitat obsah - tedy mbr
	for (int i = 0; i < K; i++){
		PomocnyPoleUzlu[i] = this->Potomci[i];
		this->Potomci[i] = NULL;
	}
	PomocnyPoleUzlu[K] = podstrom;

	VyberDvaUzly(levy, pravy);
	
	//tyto dva potomky presunu do pole vpravo
	Rstrom *pom = PomocnyPoleUzlu[levy];
	PomocnyPoleUzlu[levy] = PomocnyPoleUzlu[K - 1];
	PomocnyPoleUzlu[K - 1] = pom;
	levy = K - 1; //!!! Pamatovat si ze je to takhle

	pom = PomocnyPoleUzlu[pravy];
	PomocnyPoleUzlu[pravy] = PomocnyPoleUzlu[K];
	PomocnyPoleUzlu[K] = pom;
	pravy = K; //!!! Pamatovat si ze je to takhle

	dimenze = PomocnyPoleUzlu[levy]->dimenze;
	for (int i = 0; i < dimenze; i++){
		hranice[i][0] = PomocnyPoleUzlu[levy]->hranice[i][0];
		hranice[i][1] = PomocnyPoleUzlu[levy]->hranice[i][1];
	}
	pocetZaznamu = 1;
	mbr = PomocnyPoleUzlu[levy]->mbr;
	//levy zustava, pravy jde do novyho
	Potomci[0] = PomocnyPoleUzlu[levy];
	Rstrom *novyUzel = new Rstrom(PomocnyPoleUzlu[pravy]);

	//zjistim na kterou stranu budou jednotlive zaznamy lepe pasovat
	long long zmenal[K - 1];
	int pocetl = 1;
	bool prepocitatLevy = true;
	long long zmenar[K - 1];
	int pocetr = 1;
	bool prepocitatPravy = true;

	int maxIndex = 0;
	//podle dulezitosti prirazuji prvky
	for (int p = 0; p < K - 1; p++){
		//aby byla dodrzena podminka minimalniho poctu zaznamu v kazdem listu...
		maxIndex = DalsiPrvek(novyUzel, prepocitatLevy, prepocitatPravy, zmenal, zmenar);
		prepocitatLevy = false;
		prepocitatPravy = false;
		if (p - pocetl == m - 1){
			//zbytek musi prijit vlevo
			VlozUzel(PomocnyPoleUzlu[maxIndex]);
			pocetl++;
			prepocitatLevy = true;
			continue;
		}
		else if (p - pocetr == m - 1){
			//zbytek musi prijit vpravo
			novyUzel->VlozUzel(PomocnyPoleUzlu[maxIndex]);
			pocetr++;
			prepocitatPravy = true;
			continue;
		}

		//porovnani obsahu a prirazeni zaznamu
		//vkladam vlevo
		if (zmenal[maxIndex] < zmenar[maxIndex]){
			VlozUzel(PomocnyPoleUzlu[maxIndex]);
			pocetl++;
			prepocitatLevy = true;
		}
		//vkladam vpravo
		else if (zmenal[maxIndex] > zmenar[maxIndex]){
			novyUzel->VlozUzel(PomocnyPoleUzlu[maxIndex]);
			pocetr++;
			prepocitatPravy = true;
		}
		//zmena bude stejna - vyberu tridu s mensim obdelnikem
		else{
			//vkladam vlevo
			if (mbr < novyUzel->mbr){
				VlozUzel(PomocnyPoleUzlu[maxIndex]);
				pocetl++;
				prepocitatLevy = true;
			}
			//vkladam vpravo
			else if (mbr > novyUzel->mbr){
				novyUzel->VlozUzel(PomocnyPoleUzlu[maxIndex]);
				pocetr++;
				prepocitatPravy = true;
			}
			//pokud i obsahy jsou stejne, pak vkladam do toho listu, kde je mene zaznamu
			else{
				//vkladam vpravo
				if (this->pocetZaznamu > novyUzel->pocetZaznamu){
					novyUzel->VlozUzel(PomocnyPoleUzlu[maxIndex]);
					pocetr++;
					prepocitatPravy = true;
				}
				VlozUzel(PomocnyPoleUzlu[maxIndex]);
				pocetl++;
				prepocitatLevy = true;
			}
		}
		//tento zaznam jiz nebude vybran
		zmenal[maxIndex] = -1;
		zmenar[maxIndex] = -1;
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

bool Rstrom::Vyhledej(char *souradnice){
	if (souradnice[0] == '\n'){
		return false;
	}
	Rzaznam *z = new Rzaznam(souradnice);
	if (Koren->Vyhledej(z)){
		//printf("Nalezeno %d...\n", z->souradnice[0]);
		return true;
	}
	else{
		//printf("-NENalezeno %d\n", z->souradnice[0]);
		return false;
	}
}

bool Rstrom::Vyhledej(Rzaznam *z){
	if (JeStromList()){
		for (int i = 0; i < pocetZaznamu; i++){
			if (Zaznamy[i]->dimenze == z->dimenze){
				bool stejny = true;
				for (int j = 0; j < Zaznamy[i]->dimenze; j++){
					if (Zaznamy[i]->souradnice[j] != z->souradnice[j]){
						stejny = false;
					}
				}
				if (stejny){
					return true;
				}
			}
		}
		return false;
	}
	else{
		bool result = false;
		bool hranice = true;
		for (int i = 0; i < pocetZaznamu; i++){
			//pokud je hledany zaznam uvnitr obdelniku tohoto podstromu, volam v danem podstromu metodu Vyhledej
			if (Potomci[i]->dimenze >= z->dimenze){
				for (int j = 0; j < z->dimenze; j++){
					if (Potomci[i]->hranice[j][0] > z->souradnice[j] || Potomci[i]->hranice[j][1] < z->souradnice[j]){
						//hledany zaznam neni uvnitr obdelniku Potomci[i]
						hranice = false;
						break;
					}
				}
				if (hranice){
					result = result || Potomci[i]->Vyhledej(z);
					if (result){
						return result;
					}
				}
				hranice = true;
			}
		}
		return false;
	}
}

bool Rstrom::JeStromList(){
	if (Potomci[0] == NULL){
		return true;
	}
	return false;
}

void Rstrom::VypisPolozkyPlus(){
	if (this->Potomci[0] != NULL){
		for (int i = 0; i < pocetZaznamu; i++){
			//podminka pro nejpravejsi potomky...
			if (this->Potomci[i] != NULL){
				this->Potomci[i]->VypisPolozkyPlus();
			}
		}
	}
	else{
		for (int i = 0; i < pocetZaznamu; i++){
			PocetPolozek++;
			printf("zaznam: %d, dimenze: (%d), mbr: (%llu) -> ", i, Zaznamy[i]->dimenze, mbr);
			for (int j = 0; j < Zaznamy[i]->dimenze - 1; j++){
				printf("%d, ", Zaznamy[i]->souradnice[j]);
			}
			printf("%d\n", Zaznamy[i]->souradnice[Zaznamy[i]->dimenze - 1]);
		}
		for (int i = 0; i < dimenze-1; i++){
			printf("%d - %d, ", hranice[i][0], hranice[i][1]);
		}
		printf("%d - %d\n", hranice[dimenze - 1][0], hranice[dimenze - 1][1]);
	}
}

void Rstrom::VypisPolozky(){
	if (this->Potomci[0] != NULL){
		for (int i = 0; i < pocetZaznamu; i++){
			//podminka pro nejpravejsi potomky...
			if (this->Potomci[i] != NULL){
				this->Potomci[i]->VypisPolozky();
			}
		}
	}
	else{
		for (int i = 0; i < pocetZaznamu; i++){
			PocetPolozek++;
			for (int j = 0; j < Zaznamy[i]->dimenze - 1; j++){
				printf("%d, ", Zaznamy[i]->souradnice[j]);
			}
			printf("%d\n", Zaznamy[i]->souradnice[Zaznamy[i]->dimenze - 1]);
		}
	}
}

void Rstrom::Vypis(){
	Koren->VypisPolozky();
}

void Rstrom::VypisPlus(){
	Koren->VypisPolozkyPlus();
	printf("Pocet zaznamu: %d\n", PocetPolozek);
}

void Rstrom::UkazStrom(){
	printf("Koren: \n");
	Koren->VypisZaznamySPotomky(0);
}

void Rstrom::VypisZaznamySPotomky(int hloubka){
	if (Potomci[0] != NULL){
		//na zacatku pocet mezer urcuje zanoreni
		for (int i = 0; i < hloubka; i++){
			printf("  ");
		}
		printf("%d) (node), ", hloubka);
		printf("dimenze: \n ", dimenze);
		for (int j = 0; j < dimenze - 1; j++){
			for (int i = 0; i < hloubka; i++){
				printf("  ");
			}
			printf("%d: %d - %d,\n ", j, hranice[j][0], hranice[j][1]);
		}
		for (int i = 0; i < hloubka; i++){
			printf("  ");
		}
		printf("%d: %d - %d\n", dimenze - 1, hranice[dimenze - 1][0], hranice[dimenze - 1][1]);

		for (int i = 0; i < pocetZaznamu; i++){
			if (Potomci[i] != NULL){
				Potomci[i]->VypisZaznamySPotomky(hloubka+1);
			}
		}
	}
	else{
		for (int i = 0; i < hloubka; i++){
			printf("  ");
		}
		printf("%d) (leaf) (mbr: %llu)\n", hloubka, mbr);
		for (int j = 0; j < dimenze - 1; j++){
			for (int i = 0; i < hloubka; i++){
				printf("  ");
			}
			printf("%d: %d - %d \n", j, hranice[j][0], hranice[j][1]);
		}
		for (int i = 0; i < hloubka; i++){
			printf("  ");
		}
		printf("%d: %d - %d\n", dimenze - 1, hranice[dimenze - 1][0], hranice[dimenze - 1][1]);

		for (int i = 0; i < pocetZaznamu; i++){
			for (int i = 0; i < hloubka+1; i++){
				printf("  ");
			}
			printf("zaznam: %d, dimenze: (%d) -> ", i, Zaznamy[i]->dimenze);
			for (int j = 0; j < Zaznamy[i]->dimenze - 1; j++){
				printf("%d, ", Zaznamy[i]->souradnice[j]);
			}
			printf("%d\n", Zaznamy[i]->souradnice[Zaznamy[i]->dimenze - 1]);
		}
	}
}