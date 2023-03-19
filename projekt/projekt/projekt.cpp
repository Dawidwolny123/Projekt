#include <iostream>
#include <memory>
#include <fstream>
#include <vector>

#include <string>

/*Funkcja ta wyswietla menu programu, sklada sie z 11 opcji.*/
void menu() {
    std::cout << "PROGRAM Z OCENAMI UCZNIOW" << std::endl << std::endl;
    std::cout << "Wybierz opcje" << std::endl << std::endl;
    std::cout << "1 - Wyswietl uczniow(pesel, imie, nazwisko, data urodzenia oraz adres zamieszkania)" << std::endl;
    std::cout << "2 - Wyswietl oceny oraz przedmioty ucznia" << std::endl;
    std::cout << "3 - Wyswietl liste uczniow od konca" << std::endl;
    std::cout << "4 - Wyswietl oceny oraz przedmioty ucznia od konca" << std::endl;
    std::cout << "5 - Posortuj oceny malejaco." << std::endl;
    std::cout << "6 - Posortuj oceny rosnaco" << std::endl;
    std::cout << "7 - Usun ucznia po peselu" << std::endl;
    std::cout << "8 - Znajdz ucznia na podstawie peselu" << std::endl;
    std::cout << "9 - Dodaj ucznia" << std::endl;
    std::cout << "10- Dodaj ocene" << std::endl;
    std::cout << "11- Zamknij system" << std::endl;
}
/* Klasa Uczen zawierajaca imie nazwisko rok urodzenia oraz adres, posiada konstruktor domyslny*/

class Uczen {
 public:
     std::string name_;
     std::string surname_;
     std::string birthyear_;
     std::string address_;
     
     Uczen() { name_ = ""; surname_ = ""; birthyear_ = ""; address_ = ""; };
     
     
  Uczen(const int& pesel, const std::string& name, const std::string& surname, const std::string& birthdate, const std::string& address)
      : pesel_(pesel), name_(name), surname_(surname), birthyear_(birthdate), address_(address) {}
   int pesel_;
  
   ~Uczen() {}
  friend std::ostream& operator<<(std::ostream& os, const Uczen& obj);
  
};

/*Klasa Oceny zawierajaca konstruktor domyslny, posiada nastepujace zmienne : id oceny przedmiot oraz typ egzaminu*/
class Oceny {
public:
    int id_;
    int oceny_;
    std::string przedmiot_;
    std::string exam_Type;

    
    Oceny() { id_ = 0, oceny_ = 0; przedmiot_ = ""; exam_Type = ""; };

    
    Oceny(const int& id, const int& oceny,const std::string& przedmiot_, const std::string& examType)
        : id_(id), oceny_(oceny),przedmiot_(przedmiot_), exam_Type(examType){}
    
    ~Oceny() {}
    friend std::ostream& operator<<(std::ostream& os, const Oceny& obj);
};

std::ostream& operator<<(std::ostream& os, const Oceny& obj)
{
    os << obj.id_ << '\t' << obj.oceny_ <<'\t'<<  obj.przedmiot_ << '\t' << obj.exam_Type << '\t';
    return os;
}
std::ostream& operator<<(std::ostream& os, const Uczen& obj)
{
    os << obj.pesel_ << '\t' << obj.name_ << '\t' << obj.surname_ << '\t'<< obj.birthyear_ << '\t'<< obj.address_;
    return os;
}
/* Szablon klasy DListNode zawierajacy konstruktor, dane, oraz dwa inteligentne wskazniki(shared oraz weak) next_ oraz prev_.*/
template <typename T>
class DListNode {
public:
    DListNode(const T& data) : data(data) {}
    T data;
    std::shared_ptr<DListNode<T>> next_;
    std::weak_ptr<DListNode<T>> prev_;
    ~DListNode() {}
};
/* Szablon klasy DList zawierajacy konstruktor, inteligentny wskaznik shared_ptr head_ oraz tail_.*/
template <typename T>
class DList {
    
    
public:
    std::shared_ptr<DListNode<T>> head_;
    std::shared_ptr<DListNode<T>> tail_;
    DList() : head_(nullptr), tail_(nullptr) {}
    ~DList() {}
    /* Metoda void wstawNaPrzod(const T& data) wstawia elementy na przod listy dwukierunkowej.*/
    void wstawNaPrzod(const T& data) {
        std::shared_ptr<DListNode<T>> new_node = std::make_shared<DListNode<T>>(data);
        if (head_ == nullptr) {
            head_ = new_node;
            tail_ = new_node;
        }
        else {
            new_node->next_ = head_;
            head_->prev_ = new_node;
            head_ = new_node;
        }
    }
    
    /* Metoda void wyswietl()  wyswietla liste dwukierunkowa.*/
    void wyswietl()
    {
        std::shared_ptr<DListNode<T>> tymczasowy = head_;
        while (tymczasowy)
        {
            std::cout << tymczasowy->data<<std::endl;
            tymczasowy = tymczasowy->next_;
        }
    }
    /* Metoda wyswietlOdKonca() wyswietla liste dwukierunkowa od konca.*/
    void wyswietlOdKonca()
    {
        std::shared_ptr<DListNode<T>> tymczasowy = tail_;
        while (tymczasowy)
        {
            std::cout << tymczasowy->data << std::endl;
            tymczasowy = tymczasowy->prev_.lock();
        }
    }
    /* Metoda usunElement(int abc, auto(*lambda)(T&, int&)->bool) usuwa element z listy dwukierunkowej na podstawie peselu podanego przez uzytkownika.*/
    void usunElement(int abc, auto(*lambda)(T&, int&)->bool) {
        std::shared_ptr<DListNode<T>> current = head_;
        while (current != nullptr) {
            if (lambda(current->data , abc)) {
                if (current == head_) {
                    head_ = current->next_;
                }
                else {
                    current->prev_.lock()->next_ = current->next_;
                }
                if (current == tail_) {
                    tail_ = current->prev_.lock();
                }
                else {
                    current->next_->prev_ = current->prev_;
                }
                break;
            }
            current = current->next_;
        }
    }
    /* Metoda znajdujaca element listy na podstawie peselu podanego przez uzytkownika.*/
    T znajdz(int abc, auto(*lambda)(T&, int&)->bool) {
        std::shared_ptr<DListNode<T>> current = head_;
        while (current != nullptr) {
            if (lambda(current->data, abc))
                return current.get()->data;
            current = current->next_;
        }
        T uczen;
        uczen.pesel_ = 0;
        return uczen;
    }
    /* Metoda ta wskazuje na ostatni element listy.*/
    T last() {
        return tail_.get()->data;
    }
    /* Metoda ta void sortujSelectionRosnaco(auto(*lambda)(T&, T&)->bool) sortuje rosnaco  oceny.*/
    void sortujSelectionRosnaco(auto(*lambda)(T&, T&)->bool) {
        for (std::shared_ptr<DListNode<T>> i = head_; i != tail_; i = i->next_) {
            std::shared_ptr<DListNode<T>> min = i;
            for (std::shared_ptr<DListNode<T>> j = i->next_; j != nullptr; j = j->next_) {
                if (lambda(j->data, min->data))
                    min = j;
            }
            std::swap(i->data, min->data);
        }
    }
    /* Metoda void sortujSelectionMalejaco(auto(*lambda)(T&, T&)->bool) sortuje malejaco oceny.*/
    void sortujSelectionMalejaco(auto(*lambda)(T&, T&)->bool) {
        for (std::shared_ptr<DListNode<T>> i = head_; i != tail_; i = i->next_) {
            std::shared_ptr<DListNode<T>> max = i;
            for (std::shared_ptr<DListNode<T>> j = i->next_; j != nullptr; j = j->next_) {
                if (lambda(j->data , max->data))
                    max = j;
            }
            std::swap(i->data, max->data);
        }

    }
    /* Metoda void zapis(std::string nazwa) jest odpowiedzialna za zapisywanie elementow listy do pliku.*/
    void zapis(std::string nazwa) {
        std::ofstream plik(nazwa);
        if (plik) {
            std::shared_ptr<DListNode<T>> curr = head_;
            while (curr != nullptr) {
                plik << curr->data << std::endl;
                curr = curr->next_;
            }
        }
    }
    
};


int main() {
    menu();
    /* wyrażenia lambda pozwala na sortowanie ocen, peselu oraz znajdowanie ludzi po numerze pesel.*/
    auto lambda1 = [](Oceny& value1, Oceny& value2)->bool { return value1.oceny_ <= value2.oceny_; };
    auto lambda2 = [](Oceny& value1, Oceny& value2)->bool { return value1.oceny_ >= value2.oceny_; };
    auto lambda3 = [](Oceny& value1, int& value2)->bool { return value1.oceny_ == value2; };
    auto lambda4 = [](Uczen& value1, int& value2)->bool { return value1.pesel_ == value2; };
    auto lambda5 = [](Oceny& value1, int& value2)->bool { return value1.id_ == value2; };
    
    DList<Oceny> lista;
    DList<Uczen> lista2;
    
    /* wczytywanie z pliku tekstowego oceny.txt*/
    std::ifstream inFile("oceny.txt");
    int id_;
    int oceny_;
    std::string exam_Type;
    std::string przedmiot_;
    Oceny ocenka;
    
    while (inFile >> id_ >> oceny_>>przedmiot_ >> exam_Type)
    {
        ocenka.id_ = id_;
        ocenka.oceny_ = oceny_;
        ocenka.przedmiot_ = przedmiot_;
        ocenka.exam_Type = exam_Type;
        
        lista.wstawNaPrzod(ocenka);
        
        
    }
    /* wczytywanie z pliku tekstowego uczniowie.txt*/
    std::ifstream inFile2("uczniowie.txt");
    int pesel_;
    std::string name_;
    std::string surname_;
    std::string birthyear_;
    std::string address_;
    Uczen uczenn;

    while (inFile2 >>pesel_ >> name_ >> surname_ >> birthyear_ >> address_)
    {
        uczenn.pesel_ = pesel_;
        uczenn.name_ = name_;
        uczenn.surname_ = surname_;
        uczenn.birthyear_ = birthyear_;
        uczenn.address_ = address_;
        lista2.wstawNaPrzod(uczenn);
    }
  
    
   
    
    Uczen found;
    
    int wybor;
    std::cin >> wybor;
    while (std::cin.fail()) {
        std::cout << "Wprowadziles bledne dane" << std::endl;

        std::cin.clear();
        std::cin.ignore(1000, '\n');

        std::cout << "Wprowadz dane ponownie:";
        std::cin >> wybor;
    }
    while (wybor != 11) {

        switch (wybor) {
        case 1: {

            std::cout << "Wyswietl liste uczniow" << std::endl;
            lista2.wyswietl();
            menu();
            break;
        }
        case 2:{
            std::cout << "Wyswietl oceny oraz przedmioty ucznia" << std::endl;
            lista.wyswietl();
            menu();
            break;
        }
        case 3:{
            std::cout << "Wyswietl liste uczniow od konca" << std::endl;
            lista2.wyswietlOdKonca();
            menu();
            break;
        }
        case 4:{
            std::cout << "Wyswietl oceny oraz przedmioty ucznia od konca" << std::endl;
            lista.wyswietlOdKonca();
            menu();
            break;
        }
        case 5: {
            std::cout << "Posortuj oceny malejaco" << std::endl;
            lista.sortujSelectionMalejaco(lambda2);
            lista.wyswietl();
            menu();
            break;
        }
        case 6:{
            std::cout << "Posortuj oceny rosnaco" << std::endl;
            lista.sortujSelectionRosnaco(lambda1);
            lista.wyswietl();
            menu();
            break;
        }
        case 7:{
            std::cout << "Usun ucznia po peselu" << std::endl;
            std::cout << "Wpisz pesel osoby, ktora ma byc usunieta" << std::endl;
            int a;
            std::cin >> a;
            lista2.usunElement(a, lambda4);
            lista.usunElement(a, lambda5);
            
            break;
        }
        case 8:{
            int abc;
            std::cout << "Podaj pesel" << std::endl;
            std::cin >> abc;
            found = lista2.znajdz(abc, lambda4);
            if (found.pesel_ != 0) {
                std::cout << "Znaleziono element o wartosci " << found << std::endl;
            }
            else {
                std::cout << "Nie znaleziono elementu o podanej wartosci" << std::endl;
            }
            
            break;
        }
        case 9:
        {
            std::cout << "Dodaj ucznia" << std::endl;
            std::cout << "Podaj pesel, imie, nazwisko, rok urodzenia i adres" << std::endl;
            int pesel;
            std::string imie;
            std::string nazwisko;
            std::string data;
            std::string adres;
            std::cin >> pesel;
            std::cin >> imie;
            std::cin >> nazwisko;
            std::cin >> data;
            std::cin >> adres;
            Uczen uczen___(pesel, imie, nazwisko, data, adres);
            lista2.wstawNaPrzod(uczen___);
            std::cout << "Dodano ucznia" << std::endl;
            
            break;
        }
        case 10:
        {
            std::cout << "Dodaj ocene" << std::endl;
            std::cout << "Podaj pesel, ocene, przedmiot oraz rodzaj odpowiedzi" << std::endl;
            
            int id;
            int ocena;
            std::string przedmiot;
            std::string rodzaj;
            std::cin >> id;
            std::cin >> ocena;
            std::cin >> przedmiot;
            std::cin >> rodzaj;
            Oceny ocennna(id, ocena, przedmiot, rodzaj);
            lista.wstawNaPrzod(ocennna);
            std::cout << "Dodano ocene" << std::endl;
            
            break;
        }
        default:
        {
            system("cls");
            menu();
            std::cout << "Zostala wprowadzona zla wartosc" << std::endl;


            break;
        }
        }
        std::cin >> wybor;
    }
    
    lista.zapis("oceny.txt");
    lista2.zapis("uczniowie.txt");
    
    

}
