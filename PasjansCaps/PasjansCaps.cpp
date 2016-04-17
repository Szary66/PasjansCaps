#include <Windows.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
using namespace std;

class Karta{
	string figura;
	string kolor;
public:
	Karta(string figura, string kolor):figura(figura), kolor(kolor){}

	bool operator== (const Karta& karta){
		return (figura == karta.figura && karta.kolor == kolor)? true:false;
	}

	void dane(string figura, string kolor){
		this->figura = figura;
		this->kolor = kolor;
	}
	string inline Figura()const{
		return figura;
	}
	string inline Kolor(){
		return kolor;
	}
	string inline Awers()const{
		return kolor + figura;
	}
};

struct Paczka{
	Paczka(){ Wypeni(); }
	string tab[13]{"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "D", "K", "A"};
	string tab2[4]{"\x3", "\x4", "\x5", "\x6"};
	vector<Karta> karty;

	void Wypeni(){
		for(size_t i = 0; i < 4; ++i){
			for(size_t j = 0; j < 13; ++j){
				karty.push_back(Karta(tab[j], tab2[i]));
			}
		}
	}
};

class Gra{
public:
	Gra():czyZaznaczona(false){}
private:
	vector<Karta> talia;
	size_t kursor;
	size_t zaznaczenie;

	bool czyZaznaczona;

public:
	enum EtapGry{ Start, Rozgrywka };

	bool& CzyZaznaczona(){ return czyZaznaczona; }

	void Wyswietlanie(){
		size_t j = 0;
		system("cls");

		for(auto i : talia){
			if(czyZaznaczona){
				if(talia[zaznaczenie] == i && zaznaczenie == j){
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | BACKGROUND_BLUE);
				} else if(i == talia[kursor] && kursor == j){
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | BACKGROUND_BLUE);
				} else
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xED);
			} else{
				if(i == talia[kursor] && kursor == j){
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | BACKGROUND_BLUE);
				} else
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xED);
			}
			cout << i.Awers().c_str() << "\t";
			++j;
			if(!(j % 13))
				cout << endl;
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xED);
	}

	vector<Karta>& Talia(){
		return talia;
	}

	size_t& Kursor(){
		return kursor;
	}

	size_t& Zaznaczenie(){
		return zaznaczenie;
	}

private:
	void LosowanieBezPowtorzen(vector<Karta> zakres){
		uint16_t i = 4 * 13;
		vector<Karta>::iterator iter;
		while(i){
			uint32_t k = rand() % i;
			iter = zakres.begin() + k;
			talia.push_back(zakres[k]);
			zakres.erase(iter);
			--i;
		}
	}

	//*********************************************TUTAJ TUTAJ**************************************************************
	void LosowanieBezPoprawnychKart(vector<Karta> zakres){
		uint16_t i = 4 * 13;
		while(i-- > 0){

		}
	}

	void UstawKursor(){
		kursor = 0;
		zaznaczenie = -1;
	}

	void UsunAsy(){
		for(auto& i : talia){
			if(i.Figura() == "A")
				i.dane(" ", " ");
		}
	}

	bool PorównajFigury(size_t z, size_t na){
		if(atoi(talia[z].Figura().c_str()) < 11 && atoi(talia[z].Figura().c_str()) > 1){
			if(atoi(talia[z].Figura().c_str()) - atoi(talia[na - 1].Figura().c_str()) == 1)
				return true;
		} else{
			if(talia[na - 1].Figura() == "10" && talia[z].Figura() == "J")
				return true;
			else if(talia[na - 1].Figura() == "J" && talia[z].Figura() == "D")
				return true;
			else if(talia[na - 1].Figura() == "D" && talia[z].Figura() == "K")
				return true;
		}
		return false;
	}

public:
	void PrzesunKarte(size_t z, size_t na){
		if(talia[na].Awers() == "  "){
			if(na % 13 == 0){
				if(talia[z].Figura() == "2"){
					talia[na].dane(talia[z].Figura(), talia[z].Kolor());
					talia[z].dane(" ", " ");
				}//*********************************************************************TUTAJ TERAZ PROBLEM string - 1 ***********************************
			} else if(talia[z].Kolor() == talia[na - 1].Kolor()){
				if(PorównajFigury(z, na)){
					talia[na].dane(talia[z].Figura(), talia[z].Kolor());
					talia[z].dane(" ", " ");
				}
			}
		}
	}

	void Losowanie(EtapGry etapGry, Paczka talia){
		switch(etapGry){
		case EtapGry::Start:      LosowanieBezPowtorzen(talia.karty);   UsunAsy();          break;
		case EtapGry::Rozgrywka:  LosowanieBezPoprawnychKart(talia.karty);					break;
		default:                  UstawKursor();											break;
		}
	}
};

//Klasa odpowiedzia³na za sterowanie wyboru karty//
class Sterowanie{
public:
	UCHAR klawisz;
	bool koniec = false;

	enum Klawiatura{
		strzalkaGora = 72, strzalkaLewa = 75, strzalkaPrawa = 80, stralkaDol = 77, spacja = ' ', enter = 13
	};
public:
	~Sterowanie(){}
	void Aktualizacja(Gra& gra){
		klawisz = _getch();
		switch(klawisz){
		case Klawiatura::strzalkaPrawa: Prawa(gra.Kursor());												break;
		case Klawiatura::stralkaDol:    Dol(gra.Kursor());													break;
		case Klawiatura::strzalkaGora:  Gora(gra.Kursor());													break;
		case Klawiatura::strzalkaLewa:  Lewa(gra.Kursor());													break;
		case Klawiatura::spacja:        Zaznacz(gra);		                                                break;
		case Klawiatura::enter:         gra.Losowanie(Gra::EtapGry::Rozgrywka, Paczka());		            break;
		}
	}

private:
	void Zaznacz(Gra &gra){
		bool& b = gra.CzyZaznaczona();
		if(b){
			gra.PrzesunKarte(gra.Zaznaczenie(), gra.Kursor());
			b = false;
		} else{
			size_t& z = gra.Zaznaczenie();
			z = gra.Kursor();
			b = true;
		}
	}

	void Dol(size_t &kursor){
		kursor += 1;
		if(kursor > 4 * 13 - 1)
			kursor = 4 * 13 - 1;
	}

	void Gora(size_t &kursor){
		kursor -= 13;
		if(kursor > 4 * 13 - 1)
			kursor = 1;
	}

	void Lewa(size_t &kursor){
		kursor -= 1;
		if(kursor > 4 * 13 - 1)
			kursor = 0;
	}

	void Prawa(size_t &kursor){
		kursor += 13;
		if(kursor > 4 * 13 - 1)
			kursor = 4 * 13 - 1;
	}
};


int main(){
	srand(uint32_t(time(nullptr)));
	system("Color ED");

	Sterowanie sterowanie;
	Paczka paczka;
	Gra gra;

	bool graj = true;
	gra.Losowanie(Gra::EtapGry::Start, paczka);

	while(graj){
		gra.Wyswietlanie();
		sterowanie.Aktualizacja(gra);
	}
	system("pause");
	return 0;
}