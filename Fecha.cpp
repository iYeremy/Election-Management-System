#include "Fecha.h"

Fecha::Fecha() : dia(0), mes(0), year(0) {}

Fecha::Fecha(int dia, int mes, int year) : dia(dia), mes(mes), year(year) {}

int Fecha::getDia() const {
    return dia;
}

int Fecha::getMes() const {
    return mes;
}

int Fecha::getYear() const {
    return year;
}

int Fecha::edadActual(int actualYear) const {
    return actualYear - year;
}
