/*************************************************
Program: RENT A CAR
Datoteka: rent_a_car.cpp
Okruzenje: Microsoft Visual Studio Ultimate 2013

Autor: Hrvoje Tadin, JP Informatika

Mentor: dr. sc. Marina Ivasic-Kos

*************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <fstream> // iostream, ofstream

using namespace std;

/*
Klasa Osoba je abstraktna klasa zato sto
se nigdje u programu ne stvaraju njezine
instance, vec iskljucivo sluzi kao bazna
klasa za njezine podklase koje je naslijedjuju.
Osim toga, u sebi sadrzi obicne atribute i
iskljucivo virtualne funkcije.
Vidljivost svih atributa je javna.
*/

class Osoba{
public:
	string ime, prezime;
	int godine, mob, *id;
	virtual void postaviId(int) = 0;

	Osoba() // konstruktor
	{
		id = new int; godine = 18; // dinamicka alokacija za "ID";
		//defaultna vrijednost godina je 18 
	}
	~Osoba() // destruktor
	{
		delete id; id = 0; // dealokacija memorije za "ID", te "ID"
		// postaje null pokazivac
	}
};

/*
Klasa Klijent nasljedjuje sve atribute
i funkcije od klase Osoba. Svi atributi
jedinstveni za klijenta imaju javnu
vidljivost. U klasi su definirana dva
konstruktora nad kojima se vrsi preopterecivanje
u ovisnosti o broju, tj. tipu parametra.
*/

class Klijent : public Osoba{
public:
	string dozvola; // vrijednosti: "da" ili "ne"
	string email;
	static int *broj_klijenata; // staticka varijabla koja kontrolira broj klijenata
	// kroz svaku alokaciju njegove instance, odnosno varijabla
	// se povecava kada god se pokrene konstruktor klase.

	void postaviId(int a) // varijabla "a" je vrijednost ID-a predhodnog klijenta,
		// te se iz nje racuna ID novog (uvecanjem za jedan broj)
	{
		*(id) = a + 1;
	}

	Klijent(string d) // prvi konstruktor koji prima vrijednost dozvole
	{
		(*broj_klijenata)++; // ovdje vrsim povecanje staticke varijable
		dozvola = d;
	}
	Klijent() // drugi konstruktor
	{
		(*broj_klijenata)++; // ovdje vrsim povecanje staticke varijable
		dozvola = "da"; // pocetna vrijednost dozvole je "da"
	}
	~Klijent(){} // Destruktor. Iako je pokazivac staticke varijable
	// deklariran u klasi, dealokacija memorije se vrsi izvan
	// klase.
};

/*
Klasa Djelatnik nasljedjuje atribute i funkcije
klase Osoba. Svi atriburi i funkcije su javne
vidljivosti. Staticka varijabla 'broj djelatnika'
kontrolira ukupan broj djelatnika tako da se njezina
vrijednost kroz konstruktor povecava.
*/

class Djelatnik : public Osoba{
public:
	string obrazovanje;
	float placa;
	static int *broj_djelatnika; // staticka varijabla

	void postaviId(int a) // 'a' je sifra zadnjeg unesenog djelatnika u bazu
	{
		*(id) = a + 1;
	}
	Djelatnik() // konstruktor
	{
		(*broj_djelatnika)++; // povecanje staticke varijable

		obrazovanje = "SSS"; // postavljena je defaultna vrijednost

		placa = 1200.00; // postavljena je defaultna vrijednost
	}
	~Djelatnik(){} // destruktor
};

/*
Klasa Automobil ima privatni atribut "sifra"
za koji su stvorene funkcije get() i set().
Funkcija getSifraAuto() jest konstantna funkcija,
sto znaci da ne moze mijenjati sadrzaj varijable
koju vraca. U istoj funkciji je primjenjen "this"
pokazivac koji osigurava da se ispise sadrzaj
atributa klase.
*/

class Automobil{
private:
	int sifra;
public:
	string reg;
	float cijenaDan;
	string marka, model;
	int brMjesta;

	int getSifraAuto() const
	{
		return this->sifra;
	}
	void setSifraAuto(int s){
		sifra = s;
	};
	Automobil()
	{
		this->cijenaDan = 550; // konstruktor
	}

	~Automobil(){} // destruktor

};

/*
Klasa Datum sadrzi atribute javne vidljivosti
i svi su dinamicki alocirani.
*/

class Datum{
public:
	int *dan, *mj, *god;

	/*
	U konstruktoru je odredjen defaultni datum.
	Za svaki atribut je dodijeljena memorija.
	*/

	Datum()
	{
		dan = new int; mj = new int; god = new int;
		*dan = 1; *mj = 1; *god = 2014;
	}

	/*
	U destuktoru je dealocirana memorije svih atributa,
	te su pokazivaci postavljeni na 'null' vrijednost.
	*/

	~Datum()
	{
		delete dan, mj, god;
		dan = 0; mj = 0; god = 0;
	}
};

/*
Klasa UgovorONajmu ima jedan privatan atribut
(sifru) za koje sam napravio get() i set().
get() je konstantna i u sebi sadrzi kljucnu
rijec 'this'.
Ova klasa je primjer klase u klasi zato sto
sadrzi instance druge klase (od klase Datum).
*/

class UgovorONajmu{

private:
	int sifra;

public:
	Datum datOvjere;
	Datum odDatum;
	Datum doDatum;
	int veza_auto;
	float iznosNajma;

	int getSifra() const // konstantna funkcija koja vraca
		// sadrzaj privatnog atributa
	{
		return this->sifra;
	}

	void setSifra(int sifra) // upis sadrzaja u privatni atribut
		// uz pomoc kljucne rijeci 'this'
	{
		this->sifra = sifra;
	}

	void linkNaAuto(int a) // ugovor je povezan sa odredjenim automobilom
		// preko sifre automobila (varijabla a)
	{
		veza_auto = a;
	}

	UgovorONajmu(){} // konstruktor

	~UgovorONajmu(){} // desturktor

};


/*
Funkcija login() prolazi kroz datoteku "pristup.txt",
te provjerava da li sifra koju je unio korisnik
postoji u bazi svih sifri.
*/

bool login(string &ime, int sifra)
{
	bool pronadjen = false;
	int s;
	ifstream pristup("pristup.txt"); //pomocu ifstream stvaram datotecni tijek
	//kojeg sam nazvao "pristup" i pristupam
	//datoteci kako bih iscitao sadrzaj (u ovom
	//slucaju kombinaciju sifri i imena).


	/*Zatim se u while petlji, u varijablu "s" privremeno sprema
	procitana sifra iz trenutnog retka datoteke sve dok nemam
	vise nista za procitati ili dok trazeni redak nije pronadjen*/

	while (pristup >> s && (pronadjen == false))
	{
		pristup >> ime;
		if (s == sifra)
		{
			pronadjen = true;
		}
	}
	system("CLS"); //Clear Screen funkcija
	return pronadjen; //vraca se potvrda da li je sifra pronadjena
}

/*
U nastavku slijede funkcije koje ispisuju izbornike (prva je za
korisnika-vlasnika, a druga za korisnika-djelatnika)
*/
void izbornik_v()
{
	cout << "\nOdaberite jednu od sljedecih opcija:\n\n";
	cout << "1 -> Popis djelatnika\n";
	cout << "2 -> Dodavanje novog djelatnika\n";
	cout << "3 -> Popis automobila\n";
	cout << "4 -> Dodavanje novog automobila\n";
	cout << "5 -> IZLAZ\n";
	cout << "\nVas odabir: ";
}
void izbornik_d()
{
	cout << "\nOdaberite jednu od sljedecih opcija:\n\n";
	cout << "1 -> Pregled klijenata\n";
	cout << "2 -> Dodavanje novog klijenta\n";
	cout << "3 -> Pregled ugovora o najmu\n";
	cout << "4 -> Stvaranje novog ugovora o najmu\n";
	cout << "5 -> IZLAZ\n";
	cout << "\nVas odabir: ";
}
/*
Funkcija izracun_najma() izracunava najam na temelju
dnevne cijene automobila i trajanja najma u danima.
*/

float izracun_najma(int z, int d)
{
	ifstream a("automobili.txt"); // stvaram datotecni tok za citanje datoteke "automobili.txt"

	int s, prvi; float cijena, najam; string b;

	if (a.is_open())
	{
		/*
		Pomocu while petlje krecem se po svim redcima datoteke
		i sadrzaj tog retka zapisujem u pomocne varijable.
		Za svaki redak provjerim da li je njegova sifra ista
		kao sifra automobila koji se zeli unajmiti (sifra
		automobila koji se zeli unajmiti je proslijedjena
		kroz listu argumenata funkcije). Ukoliko je sifra
		jednaka trazeni automobil je pronadjen, a njegova
		pripadajuca cijena se mnozi sa brojem dana i dobiva
		se iznos najma (broj dana se prima iz liste parametara
		funkcije). U slucaju da sifra nije zadovoljavajuca
		prelazim u citanje novog retka. I tako sve dok ne
		procitam cijelu datoteku.
		*/

		while (a >> prvi)
		{

			a >> b;
			a >> cijena;
			a >> b;
			a >> b;
			a >> s;

			if (prvi == z)
			{
				najam = cijena * float(d);
			}
		}
		a.close();
	}
	else cout << "\nNemoguce otvoriti datoteku automobili.txt.\n";
	return najam; // vraca se ukupna cijena najma (float)
}

/*
Funkcija unos_dana_najma() prima parametar tipa vektor koji
je na pola popunjen u funkciji novi_ugovor(). U funkciji
unos_dana_najma() popunjava se ostatak vektora sa vrijednostima
pocetka najma i zavrsetka najma. Nakon toga slijedi izracun ukupnog
trajanja dana kao razlika predhodno popunjenih datuma.
*/

int unos_dana_najma(vector<UgovorONajmu*> *ugo)
{
	cout << "\nDatum pocetka najma:\n";
	cout << "\nDan: "; cin >> *(ugo->at(0)->odDatum.dan);
	cout << "Mjesec: ";  cin >> *(ugo->at(0)->odDatum.mj);
	cout << "Godina: ";  cin >> *(ugo->at(0)->odDatum.god);
	cout << "\nDatum zavrsetka najma:\n";
	cout << "\nDan: "; cin >> *(ugo->at(0)->doDatum.dan);
	cout << "Mjesec: ";  cin >> *(ugo->at(0)->doDatum.mj);
	cout << "Godina: ";  cin >> *(ugo->at(0)->doDatum.god);

	int days(0); int month_days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if (*(ugo->at(0)->odDatum.god) == *(ugo->at(0)->doDatum.god))
	{
		if (*(ugo->at(0)->odDatum.mj) == *(ugo->at(0)->doDatum.mj))
		{
			days = *(ugo->at(0)->doDatum.dan) - *(ugo->at(0)->odDatum.dan);
		}
		else
		{
			for (int i = (*(ugo->at(0)->odDatum.mj)); i < *(ugo->at(0)->doDatum.mj) - 1; i++)
			{
				days += month_days[i];
			}
			days += month_days[*(ugo->at(0)->odDatum.mj) - 1] - *(ugo->at(0)->doDatum.dan) + *(ugo->at(0)->doDatum.dan);
		}
	}
	else
	{
		for (int i = 0; i < *(ugo->at(0)->doDatum.mj) - 1; i++)
		{
			days += month_days[i];
		}
		for (int i = (*(ugo->at(0)->odDatum.mj)); i < 12; i++)
		{
			days += month_days[i];
		}
		if (*(ugo->at(0)->doDatum.god) - (*(ugo->at(0)->odDatum.god)) >= 0)
			days += (*(ugo->at(0)->doDatum.god) - (*(ugo->at(0)->odDatum.god)) - 1) * 365 +
			month_days[*(ugo->at(0)->odDatum.mj) - 1] - (*(ugo->at(0)->doDatum.dan)) + (*(ugo->at(0)->doDatum.dan));
	}

	return days;
}

/*
Funkcija koliko_ugovora() ulazi u datoteku
s ugovorima ("ugovori.txt"). Prebrojavanje
redaka sam ostvario tako da sam prebrojao
sve integer vrijednosti svih redaka i zatim
ih podijelio s brojem atributa. Na taj nacin
sam dobio koliko ugovora, odnosno zapisanih
redaka postoji u toj datoteci.
*/

int koliko_ugovora()
{
	ifstream koliko("ugovori.txt"); // stvaram datotecni tok za citanje datoteke

	int brojac(0), br; // inicijaliziram brojac na nulu i stvaram pomocnu varijablu br

	while (koliko >> br) // brojim sve integere u datoteci
	{
		brojac++;
	}
	brojac = brojac / 12; // kada podijelim sve prebrojene integere sa 12 atributa
	// dobijem broj redaka u datoteci, zato sto svaki redak
	// koji opisuje ugovore sadrzi tocno 12 atributa
	return brojac;
}

/*
Funkcija "koliko" prebrojava broj
zapisa iz datoteke koja joj je
poslana u listi argumenata pod
imenom "naziv".
*/

int koliko(string naziv)
{
	ifstream koliko(naziv);
	int brojac(0); string a;
	while (getline(koliko, a)) // putem funkcije 'getline' cita se linija do \n
	{
		brojac++; // ovdje povecavam brojac za svaku
		// novu procitanu liniju
	}
	return brojac;
}

/*
U funkciji "dodaj_klijenta()" podatke zapisujem
u instancu klase klijent, zatim ih proslijedjujem
kao novi redak u datoteku "klijenti.txt".
*/

void dodaj_klijenta()
{

	Klijent *k = new Klijent; //dinamièki alocirana memorija za jednu instancu klase Klijent

	/*
	Zatim otvaram datotecni tok za ulaz u datoteku "klijenti.txt",
	te postavljam zastavicu "app" kako bih mogao dodati novi redak
	u datoteku bez brisanja predhodno upisanog sadrzaja.
	*/

	ofstream kl("klijenti.txt", ios::app);

	int b = koliko("klijenti.txt"); // ukupan broj klijenata u datoteci

	if (kl.is_open()) // provjera da li je datoteka uspjesno otvorena
	{
		system("CLS");
		cout << "UNOS KLIJENTA\n\n";
		cout << "Trenutni broj klijenata u bazi: " << b << endl << endl;

		/*
		Funkcija klase Klijent koja se zove "postaviId()" automatski
		izracunava vrijednost novog Id-a klijenta na temelju Id-a
		zadnjeg upisanog klijenta.
		*/

		k->postaviId(b); kl << *(k->id) << " ";

		cin.sync(); cin.clear(); // cistim buffer prije nego sto ucitam string sa getline funkcijom

		cout << "Ime: "; getline(cin, k->ime); kl << k->ime << " ";
		cout << "Prezime: "; getline(cin, k->prezime); kl << k->prezime << " ";
		cout << "Dob: "; cin >> k->godine; kl << k->godine << " ";
		cout << "Mobitel: "; cin >> k->mob; kl << k->mob << " ";
		cout << "Vozacka dozvola (da / ne): "; cin.sync(); cin.clear(); getline(cin, k->dozvola); kl << k->dozvola << " ";
		cout << "Email: "; cin.sync(); cin.clear(); getline(cin, k->email); kl << k->email << endl;

		cout << "\nKlijent je uspjesno unesen!\n";
		kl.close(); // zatvaram datotecni tok za zapis
	}
	else
	{
		cout << "\nDatoteka o djelatnicima se ne moze otvoriti!\n";
	}

	delete k; // dealokacija memorije za klijenta
}


/*
Funkcija ispis_klijenata() otvara datoteku "klijenti.txt",
omoguæava èitanje po redcima i istovremeno ispisivanje
procitanog sadrzaja na ekran u skladu sa uputom.
*/

void ispis_klijenata()
{
	ifstream kl("klijenti.txt"); // definiram datotecni tok za ulaz u datoteku

	int b = koliko("klijenti.txt"), i; string s; // funkcija koliko() vraca broj upisanih klijenata u bazu
	if (kl.is_open()) // provjeravam da li je datoteka uspjesno otvorena
	{
		system("CLS"); // cistim ekran na kojem je bio ispisan izbornik za djelatnika
		cout << "POPIS KLIJENATA\n\n";
		cout << "Trenutni broj klijenata u bazi: " << b << endl;

		while (kl >> i) // citam sve retke datoteke "klijenti.txt"
			// i odmah spremam vrijednost ID-a
		{
			cout << "\nID: " << i; kl >> s;
			cout << "\nIme: " << s; kl >> s;
			cout << "\nPrezime: " << s; kl >> i;
			cout << "\nDob: " << i; kl >> i;
			cout << "\nMobitel: " << i; kl >> s;
			cout << "\nVozacka dozvola: " << s; kl >> s;
			cout << "\nEmail: " << s << endl;
		}
		kl.close(); // zatvaram datotecni tok za citanje
	}
	else
	{
		cout << "\nDatoteka o klijentima se ne moze otvoriti!\n";
	}

}

/*
Funkcija "dodaj_djelatnika" omogucava zapis podataka
o novom djelatniku u datoteku "djelatnici.txt". Prvo
sam dinamicki stvorio instancu klase djelatnik kako bih
u nju pohranio te podatke. Zatim sam iscitao sve atribute
klase i preusmjerio ih u datoteku.
*/

void dodaj_djelatnika()
{

	Djelatnik *d = new Djelatnik; //dinamicki alocirana instanca klase Djelatnik

	ofstream dj("djelatnici.txt", ios::app); // otvaram datotecni tok za upis podataka
	// u navedenu datoteku i postavljam zastavicu
	// "app" koja mi omogucava da nadodam svoji
	// novi zapis iza zadnjeg postojeceg bez ikakvog
	// brisanja.
	int b = koliko("djelatnici.txt");
	if (dj.is_open()) // provjera da li je datoteka uspjesno otvorena
	{
		system("CLS"); //Clear Screen funkcija za brisanje predhodno ispisanog izbornika
		cout << "UNOS DJELATNIKA\n";
		cout << "\nTrenutni broj djelatnika u bazi: " << b << endl << endl;
		/*
		A sada pozivam funkciju "postaviId()" koja ce umjesto
		vlasnika automatski izracunati slijedeci moguci ID
		za djelatnika koji se unosi. Funkcija prima sifru
		zadnjeg djelatnika iz baze te nova sifra postaje
		zadnja sifra uvecana za 1. ID koji sam pohranio u
		klasu zapisujem u datoteku kao prvu vrijednost.
		*/
		d->postaviId(b); dj << *(d->id) << " ";

		cin.sync(); cin.clear(); // cistim buffer kako bi getline uspjesno
		// procitao string

		/*
		Zatim, ono sto korisnik posalje sa tipkovnice upisem u odgovarajuci
		dio instance klase, a zatim to sto je u klasi zapise u odgovarajuci
		redak datoteke.
		*/
		cout << "Ime: "; getline(cin, d->ime); dj << d->ime << " ";
		cout << "Prezime: "; getline(cin, d->prezime); dj << d->prezime << " ";
		cout << "Dob: "; cin >> d->godine; dj << d->godine << " ";
		cout << "Mobitel: "; cin >> d->mob; dj << d->mob << " ";
		cout << "Obrazovanje(SSS, VSS): "; cin.sync(); cin.clear(); getline(cin, d->obrazovanje); dj << d->obrazovanje << " ";
		cout << "Iznos place (kn): "; cin >> d->placa; dj << d->placa << endl;

		cout << "\nDjelatnik je uspjesno unesen!\n";
	}
	else
	{
		cout << "\nDatoteka o djelatnicima se ne moze otvoriti!\n";
	}
	dj.close(); // zatvaram datotecni tok za zapis

	delete d; // dealokacije memorije za djelatnike
}

/*
U nastavku je funkcija za ispis djelatnika, koja putem ifstream toka
ulazi u datoteku "djelatnici.txt", iscitava red po red i ispisuje na
ekran.
*/

void ispis_djelatnika()
{

	ifstream dj("djelatnici.txt");
	int b = koliko("djelatnici.txt"), i; float f; string s; //pozivam funkciju koja
	//prebrojava broj zapisa
	//u datoteci djelatnici

	if (dj.is_open()) // sa "is_open" provjeravam da li je
		// datoteka uspjesno otvorena
	{
		system("CLS"); // koristim Clear Screen kako bi se obrisao
		// predhodno ispisan izbornik
		cout << "POPIS DJELATNIKA\n\n";
		cout << "Trenutni broj djelatnika u bazi: " << b << endl;

		/*
		U dolje navedenoj while petlji, za svaki redak iz datoteke prvo
		citam ID, Ime, Prezime, Godine, Mobitel, Obrazovanje te iznos
		place. Svaku procitanu vrijednost istovremeno ispisujem na ekran
		uz pridruzeni opis.
		*/

		while (dj >> i)
		{
			cout << "\nID: " << i; dj >> s;
			cout << "\nIme: " << s; dj >> s;
			cout << "\nPrezime: " << s; dj >> i;
			cout << "\nGodine: " << i; dj >> i;
			cout << "\nMobitel: " << i; dj >> s;
			cout << "\nObrazovanje(SSS, VSS): " << s; dj >> f;
			cout.setf(ios::fixed); // fiskiranje decimalne tocke
			cout.setf(ios::showpoint); // prikaz decimalne tocke
			cout.precision(2); // zaokruzivanje na dvije decimale
			cout << "\nIznos place (kn): " << f << endl;
		}
	}
	else
	{
		cout << "\nDatoteka o djelatnicima se ne moze otvoriti!\n";
	}
	dj.close(); // zatvaranje datotecnog toka
}

/*
Funkcija "unos_auto()" omogucava zapis podataka
o novom automobilu u instancu klase Automobil,
a zatim dijelove instance zapisuje u odgovarajucem
obliku u novi redak datoteke automobili.txt
*/

void unos_auto()
{
	ofstream fout("automobili.txt", ios::app);

	/*
	Osim funkcije is_open(), provjera da li je datoteka
	uspjesno otvorena moze se napraviti tako da provjerimo
	da li je rijec za datotecni tok "faut"==true, jer ako je
	'false' to znaci da se datoteka nije otvorila i ne
	mozemo joj pristupiti.
	*/

	if (!fout) {
		cout << "Datoteka se ne moze otvoriti za pisanje.\n";

	}
	Automobil a;
	system("CLS"); cout << "UNOS AUTOMOBILA:\n\n";
	int k = koliko("automobili.txt"); cout << "Trenutni broj automobila u bazi: " << k << ".\n\n";

	/*
	Postavljanje sifre automobila sam napravio tako da se zapisuje automatski,
	bez utjecaja korisnika. Kako je sifra privatni atribut klase, pozvao sam
	"set" funkciju u koju saljem broj zadnjeg upisanog automobila i uvecavam
	ga za jedan, sto postaje nova sifra od novog auta.
	*/

	a.setSifraAuto(k + 1); fout << a.getSifraAuto() << " ";

	/*
	Ostale atribute popunjavam tako da vrijednosti koje salje korisnik
	zapisujem u instancu klase koju zatim proslijedjujem u datoteku
	(atribut po atribut).
	*/

	cout << "Registracija: "; cin >> a.reg; fout << a.reg << " ";
	cout << "Dnevna cijena: "; cin >> a.cijenaDan; fout << a.cijenaDan << " ";
	cout << "Marka: "; cin >> a.marka; fout << a.marka << " ";
	cout << "Model: "; cin >> a.model; fout << a.model << " ";
	cout << "Broj sjedecih mjesta: "; cin >> a.brMjesta; fout << a.brMjesta << endl;

	cout << "\nAutomobil je uspjesno unesen!\n";
	fout.close(); // zatvaram datotecni tok za zapis
}

/*
Funkcija "ispis_automobila()" otvara datoteku
"automobili.txt", cita zapis po zapis koji
istovremeno ispisujem na ekran.
*/

void ispis_automobila()
{
	cout << "POPIS AUTOMOBILA\n\n";
	ifstream ulaz("automobili.txt"); // stvaram datotecni tok za citanje datoteke
	if (ulaz.is_open()) // provjera da li je datoteka uspjesno otvorena
	{

		string s; int b; float f; // pomocne varijable za spremanje vrijednosti koje
		// citam iz datoteke


		while (ulaz >> s) // kontroliram da se procita bas svaki postojeci redak u
			// datoteci
		{
			cout << "\nSifra: "; cout << s;
			cout << "\nReg: "; ulaz >> s; cout << s;
			cout.setf(ios::fixed); // fiksiranje decimalne tocke
			cout.setf(ios::showpoint); // prikaz decimalne tocke
			cout.precision(2); // zaokruzivanje na dvije decimale
			cout << "\nCijena po danu(kn): "; ulaz >> f; cout << f;
			cout << "\nMarka: "; ulaz >> s; cout << s;
			cout << "\nModel: "; ulaz >> s; cout << s;
			cout << "\nBroj sjedecih mjesta: "; ulaz >> b; cout << b << endl;
		}
		ulaz.close(); // zatvaranje datotecnog toka za citanje
	}
	else cout << "\nNemoguce otvoriti datoteku o ugovorima.\n";
}

/*
Funkcija novi_ugovor() vrsi zapis novih podataka u vektor
s jednom klasom UgovorONajmu. Nakon sto se popuni vektor,
sve sto je zapisano u njega zapisuju se u odgovarajuci
redak datoteke "ugovori.txt".
*/

void novi_ugovor()
{
	/*
	Stvaram pokazivac na vektor pod nazivom 'ugo' koji u
	sebi sadrzi pokazivac na jednu instancu klase UgovorONajmu,
	te istovoremeno alociram memoriju za vektor.
	*/

	vector<UgovorONajmu*> *ugo = new vector<UgovorONajmu*>(1);

	/*
	Dinamicki alociram memoriju za instancu koja se nalazi
	na nultoj poziciji vektora (to je ujedno jedina instanca
	u vektoru).
	*/

	ugo->at(0) = new UgovorONajmu;

	int z, dani;

	int br_ugovora = koliko_ugovora(); // sifra zadnjeg unesenog ugovora

	system("CLS"); // koristim Clear Screen kako bi se obrisao
	// predhodno ispisan izbornik

	cout << "STVARANJE NOVOG UGOVORA\n\n";
	cout << "Trenutni broj ugovora u bazi: " << br_ugovora << endl;

	/*
	Svi slijedeci podaci se popunjavaju na nacin da
	se pozicioniramo na vektor u nultu poziciju, te
	onda na odredjeni atribut klase.

	Privatni atribut sifra se popunjava uz set() funkciju
	koja prima sifru zadnjeg ugovora uvecanu za 1 (sto
	postaje nova sifra).
	*/

	ugo->at(0)->setSifra(br_ugovora + 1);
	cout << "\nDatum ovjere:\n";
	cout << "\nDan: "; cin >> *(ugo->at(0)->datOvjere.dan);
	cout << "Mjesec: "; cin >> *(ugo->at(0)->datOvjere.mj);
	cout << "Godina: "; cin >> *(ugo->at(0)->datOvjere.god);
	cout << "\nPonudjeni automobili:\n";

	ispis_automobila();
	cout << "\nUpisite sifru zeljenog autombila iz ponudjenog izbornika: "; cin >> z;

	ugo->at(0)->linkNaAuto(z); // 'z' je sifra unajmljenog automobila koja se
	// spaja sa odredjenim ugovorom

	dani = unos_dana_najma(ugo); // funkcija vraca ukupan broj dana najma

	ugo->at(0)->iznosNajma = izracun_najma(z, dani); // funkcija vraca iznos najma
	// i iznos se zapisuje u vektor


	cout << "\nUgovor je uspjesno unesen!\n\nUKUPNA CIJENA NAJMA (kn): " << ugo->at(0)->iznosNajma << endl;

	/*
	Sada slijedi zapis sadrzaja iz vektora u odgovarajucu
	liniju u datoteci "ugovori.txt".
	*/

	ofstream ispis("ugovori.txt", ios::app);

	ispis << ugo->at(0)->getSifra() << " ";
	ispis << *(ugo->at(0)->datOvjere.dan) << " " << *(ugo->at(0)->datOvjere.mj) << " " << *(ugo->at(0)->datOvjere.god) << " ";
	ispis << *(ugo->at(0)->odDatum.dan) << " " << *(ugo->at(0)->odDatum.mj) << " " << *(ugo->at(0)->odDatum.god) << " ";
	ispis << *(ugo->at(0)->doDatum.dan) << " " << *(ugo->at(0)->doDatum.mj) << " " << *(ugo->at(0)->doDatum.god) << " ";
	ispis << ugo->at(0)->veza_auto << " " << ugo->at(0)->iznosNajma << endl;
	ispis.close();

	delete ugo->at(0); // dealociranje memorije instance UgovorONajmu iz vektora 'ugo'

	delete ugo; // dealociranje memorije za vektor

}

/*
U funkciji pregled_ugovori() unos novog ugovora
je ostvaren preko vektora popunjenog sa klasama
naziva "UgovorONajmu". Svaki procitani redak iz
datoteke se sprema u jedan ugovor iz vektora.
Nakon sto se vektor popuni do kraja izlistavamo
njegov sadrzaj na ekran uz odgovarajuce poruke.
*/

void pregled_ugovori()
{
	int b, m, g; float f; // pomocne varijable kod citanja datoteke

	system("CLS"); // Clear Screen koji cisti ekran sa sadrzajem izbornika
	cout << "PREGLED UGOVORA\n\n";

	ifstream ulaz("ugovori.txt"); // datotecni tok za citanje datoteke

	int br = koliko_ugovora(); // vazno je poznavati broj ugovora zbog
	// stvaranja pocetne velicine vektora

	cout << "Trenutni broj ugovora u bazi: " << br << endl << endl;

	/*
	Stvaram pokazivac "list" na vektor popunjen pokazivacima
	na klasu "UgovorONajmu". Pritom dodjelujem memoriju za taj
	vektor koji ce sadrzavati "br" ugovora.
	*/

	vector<UgovorONajmu*> *list = new vector<UgovorONajmu*>(br);

	/*
	Za svaki ugovor ciji je pokazivac sadrzan u vektoru dinamicki
	dodjelujem memoriju u slijedecoj for petlji. Kroz vektor prolazim
	uz pomoc "at()" funkcije.
	*/

	for (int i = 0; i < br; i++){ list->at(i) = new UgovorONajmu; }

	int i(0);

	if (ulaz.is_open()) // provjera da li je datoteka otvorena
	{
		/*
		while petlja osigurava prolaz kroz sve redove datoteke "ugovori.txt".
		Svaka vrijednost koja se procita odmah se zapisuje u vektor "list"
		u odgovarajucu klasu na odgovarajucu poziciju.
		*/
		while (ulaz >> b)
		{
			list->at(i)->setSifra(b);
			ulaz >> b; ulaz >> m; ulaz >> g;
			*(list->at(i)->datOvjere.dan) = b; *(list->at(i)->datOvjere.mj) = m; *(list->at(i)->datOvjere.god) = g;
			ulaz >> b; ulaz >> m; ulaz >> g;
			*(list->at(i)->odDatum.dan) = b; *(list->at(i)->odDatum.mj) = m; *(list->at(i)->odDatum.god) = g;
			ulaz >> b; ulaz >> m; ulaz >> g;
			*(list->at(i)->doDatum.dan) = b; *(list->at(i)->doDatum.mj) = m; *(list->at(i)->doDatum.god) = g;
			ulaz >> b; list->at(i)->linkNaAuto(b);
			ulaz >> f; list->at(i)->iznosNajma = f;
			i = i + 1;
		}
		ulaz.close(); // zatvaranje datotecnog toka za citanje 
	}
	else cout << "\nNemoguce otvoriti datoteku o ugovorima.\n";

	/*
	Uz pomoc for petlje prolazim po cijelom vektoru, tocnije
	za svaki j-ti ugovor ispisujem sve njegove podatke.
	*/

	for (int j = 0; j<br; j++){
		cout << "Sifra: " << list->at(j)->getSifra() << endl;
		cout << "Datum ovjere: " << *(list->at(j)->datOvjere.dan) << "." << *(list->at(j)->datOvjere.mj) << "." << *(list->at(j)->datOvjere.god) << endl;
		cout << "Od datuma: " << *(list->at(j)->odDatum.dan) << "." << *(list->at(j)->odDatum.mj) << "." << *(list->at(j)->odDatum.god) << endl;
		cout << "Do datuma: " << *(list->at(j)->doDatum.dan) << "." << *(list->at(j)->doDatum.mj) << "." << *(list->at(j)->doDatum.god) << endl;
		cout << "Sifra unajmljenog auta: " << list->at(j)->veza_auto << endl;
		cout.setf(ios::fixed); // fiksiram decimalnu tocku
		cout.setf(ios::showpoint); // prikaz decimalne tocke
		cout.precision(2); // zaokruzio sam na dvije decimale
		cout << "Iznos najma: " << list->at(j)->iznosNajma << endl << endl;
	}
}

/*
Ovdje dinamicki dodjelujem memoriju za obje staticke varijable iz klasa,
te im istovremeno zadajem pocetnu vrijednost. Dodjela memorije se vrsi
samo jednom, a njihovo povecanje se kontrolira u konstruktoru za svaku
instancu.
*/

int *(Djelatnik::broj_djelatnika) = new int(koliko("djelatnici.txt"));
int *(Klijent::broj_klijenata) = new int(koliko("klijenti.txt"));

int main()
{
	int sifra, izbor; bool prijava = false; string ime;
	char odg;

	do
	{
		system("CLS"); //u system saljem naredbu Clear Screen da "pocistim" ekran
		cout << "\n\t**************************************";
		cout << "\n\t**                                  **";
		cout << "\n\t**          Dobrodosli u            **";
		cout << "\n\t**                                  **";
		cout << "\n\t**           RENT A CAR             **";
		cout << "\n\t**                                  **";
		cout << "\n\t**************************************";

		cout << "\n\n\n\tUnesite sifru pristupa: "; cin >> sifra;

		prijava = login(ime, sifra); //poziv funkcije login koja mi vraca potvrdu da
		//li postoji takav registrirani korisnik

		if (prijava) // sifra je autentificirana
		{
			cout << "Dobrodosao " << ime << endl << endl;
			if (ime == "Vlasnik")
			{
				do{
					izbornik_v();
					cin >> izbor;

					switch (izbor)
					{
					case 1: //popis djelatnika
						ispis_djelatnika();
						break;
					case 2: //dodavanje djelatnika
						dodaj_djelatnika();
						break;
					case 3: //ispis popisa automobila
						system("CLS");
						ispis_automobila();
						break;
					case 4: //dodavanje automobila
						unos_auto();
						break;
					default: cout << "\nIzlaz\n";
						break;
					}
					cout << "\nPovratak na glavni izbornik (d/n)?"; cin >> odg;
					system("CLS");

				} while (odg == 'd');

			}
			else if (ime == "Djelatnik")
			{
				do{
					izbornik_d();
					cin >> izbor;

					switch (izbor)
					{
					case 1://pregled klijenata
						ispis_klijenata();
						break;
					case 2: // dodavanje novog klijenta
						dodaj_klijenta();
						break;
					case 3: //pregled ugovora
					{
						pregled_ugovori();
						break;
					}
					case 4: //stvaranje novog ugovora o najmu
					{
						novi_ugovor();
						break;
					}
					default: cout << "\nIzlaz\n";
						break;
					}
					cout << "\nPovratak na glavni izbornik (d/n)?"; cin >> odg;
					system("CLS");
				} while (odg == 'd');
			}
		}
		else //prijava nije uspjela
		{
			cout << "Neispravna sifra!\n";
		}

		cout << "\nZelite li se ulogirati kao drugaciji korisnik (d/n)? "; cin >> odg;
	} while (odg == 'd'); // koristio sam ovdje do-while petlju kako bih omogucio
	// ponovno logiranje (u izboru kao "vlasnik" ili "djelatnik"),
	// te ukoliko je korisnik pogrijesio kod unosa sifre; ima priliku
	// je ponovno unijeti

	system("CLS");
	cout << "\nHvala na koristenju 'Rent a car' aplikacije!" << endl;
	cin.sync();
	cin.get();
	return 0;
}