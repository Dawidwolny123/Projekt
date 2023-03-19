#pragma once
#include <iostream>

#include <memory>
#include <fstream>
#include <vector>
#include <string>

class Uczen {
public:
    std::string name_;
    std::string surname_;
    std::string birthyear_;
    std::string address_;
    /**Konstruktor
    @param name_ = ""
    @param surname_ = ""
    @param birthyear_ = ""
    @param address_ = ""
    */
    Uczen() { name_ = ""; surname_ = ""; birthyear_ = ""; address_ = ""; };
    /**Konstruktor przeci¹¿ony
    @param pesel_ = pesel
    @param name_ = name
    @param surname_ = surname
    @param birthyear_ = birthdate
    @param address_ = address
    */
    Uczen(const int& pesel, const std::string& name, const std::string& surname, const std::string& birthdate, const std::string& address)
        : pesel_(pesel), name_(name), surname_(surname), birthyear_(birthdate), address_(address) {}
    int pesel_;

    ~Uczen() {}
    friend std::ostream& operator<<(std::ostream& os, const Uczen& obj);

};