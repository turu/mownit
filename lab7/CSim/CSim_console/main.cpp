#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cmath>
#include "CVector.h"
#include "CFlags.h"

using namespace std;
using namespace CSim;

void ConsoleSim()
{
    string path, iter, name, fpath;
    CT gstep, G;
    char d;
    int flag = 0;

    cout<<"Podaj bezwzgledna sciezke do katalogu symulacji:\n";
    cin>>path;
    cout<<"Podaj nazwe symulacji. ";
    cout<<"W podanej lokalizacji zostanie utworzony katalog symulacji: ";
    cin>>name;
    cout<<"Prosze podac globalny krok (Uwaga: kroki poszczegolnych obiektow moga roznic sie w przypadku wlaczonej opcji zmiennego kroku,\n\
lub gdy podano inna wartosc przy inicjalizacji obiektu): ";
    cin>>gstep;
    cout<<"Prosze podac stala grawitacji G (z powodu niedokladnosci typow zmiennoprzecinkowych zaleca sie wartosc 1): ";
    cin>>G;

    cout<<"Prosze podac bezwzgledna sciezke do pliku z danymi.\nPlik powinien miec nastepujacy format:\n\
    - kazdy obiekt w osobnej linii\n\
    - kazda linia formatu: [mass] [step] [x] [y] [z] [vx] [vy] [vz]\n\
    - kazda kolumna oddzielona bialym znakiem/znakami; (bez znakow '[' i ']')"<<endl;
    cin>>fpath;

    cout<<"Prosze podac ilosc iteracji: ";
    cin>>iter;

    cout<<"Czy zezwolic na automatyczne, indiwidualne dostrajanie wielkosci kroku? Uwaga: Polepsza precyzje obliczen, pogarsza wydajnosc. [y/n] ";
    cin>>d;
    if(d == 'y' || d == 'Y')
        flag = CE_ADJUSTABLE;
    else
        flag = CE_NOT_ADJUSTABLE;

    char snum[10];
    sprintf(snum, "%lf", (double)gstep);
    string sstep(snum);
    sprintf(snum, "%lf", (double)G);
    string sg(snum);
    string exe = "./CSim_visual -m t -w y -p "+path+" -n "+name+" -d "+fpath+" -s "+sstep+" -g "+sg+" -i "+iter+" -a "+(flag&CE_ADJUSTABLE ? "y" : "n");
    //execute subprogram
    system(exe.c_str());
}

void ReRun()
{
    string path, x, y;

    cout<<"Podaj bezwzgledna sciezke do katalogu symulacji (katalog w ktorym znajduje sie podkatalog 'traces'):\n";
    cin>>path;
    cout<<"Podaj szerokosc ekranu: ";
    cin>>x;
    cout<<"Podaj wysokosc ekranu: ";
    cin>>y;
    string exe = "./CSim_visual -m r -p "+path+" -x "+x+" -y "+y;
    //execute subprogram
    system(exe.c_str());
}

void RealTime()
{
    int flag = 0;
    string path, name, fpath, x, y;
    CT gstep, G;
    char d;

    cout<<"Czy zapisywac przebieg symulacji do plikow? [y/n] ";
    cin>>d;
    if(d == 'y' || d == 'Y')
        flag = CE_WRITE;
    else
        flag = CE_NOT_WRITE;
    cout<<"Czy zezwolic na automatyczne, indiwidualne dostrajanie wielkosci kroku? Uwaga: Polepsza precyzje obliczen, pogarsza wydajnosc. [y/n] ";
    cin>>d;
    if(d == 'y' || d == 'Y')
        flag |= CE_ADJUSTABLE;
    else
        flag |= CE_NOT_ADJUSTABLE;
    CFlags = flag;
    if(flag&CE_WRITE)
    {
        cout<<"Podaj bezwzgledna sciezke do katalogu symulacji:\n";
        cin>>path;
    }
    cout<<"Podaj nazwe symulacji. ";
    if(flag&CE_WRITE) cout<<"W podanej lokalizacji zostanie utworzony katalog symulacji: ";
    cin>>name;
    cout<<"Prosze podac globalny krok (Uwaga: kroki poszczegolnych obiektow moga roznic sie w przypadku wlaczonej opcji zmiennego kroku,\n\
lub gdy podano inna wartosc przy inicjalizacji obiektu): ";
    cin>>gstep;
    cout<<"Prosze podac stala grawitacji G (z powodu niedokladnosci typow zmiennoprzecinkowych zaleca sie wartosc 1): ";
    cin>>G;

    cout<<"Prosze podac bezwzgledna sciezke do pliku z danymi.\nPlik powinien miec nastepujacy format:\n\
    - kazdy obiekt w osobnej linii\n\
    - kazda linia formatu: [mass] [step] [x] [y] [z] [vx] [vy] [vz]\n\
    - kazda kolumna oddzielona bialym znakiem/znakami; (bez znakow '[' i ']')"<<endl;
    cin>>fpath;
    cout<<"Podaj szerokosc ekranu: ";
    cin>>x;
    cout<<"Podaj wysokosc ekranu: ";
    cin>>y;
    char snum[10];
    sprintf(snum, "%lf", (double)gstep);
    string sstep(snum);
    sprintf(snum, "%lf", (double)G);
    string sg(snum);
    string exe = "./CSim_visual -m s -w "+(flag&CE_WRITE ? "y -p "+path : "n")+" -n "+name+" -d "+fpath+" -s "+sstep+" -g "+sg+" -x "+x+" -y "+y+" -a "+(flag&CE_ADJUSTABLE ? "y" : "n");
    //execute subprogram
    system(exe.c_str());
}

int main(int argc, char ** argv)
{
    int mode;
    cout<<"Witaj w CSim !!!\n";
    cout<<"Podaj tryb pracy:\n- Wizualizacja wczesniej przeprowadzonej symulacji - 1\n\
- Symulacja bez wizualizacji (tylko zapis do plikow) - 2\n\
- Symulacja w czasie rzeczywistym - 3\n> ";
    cin>>mode;
    switch(mode)
    {
        case 1:
            ReRun();
        break;
        case 2:
            ConsoleSim();
        break;
        case 3:
            RealTime();
        break;
        default:
            exit(1);
    }
    return 0;
}
