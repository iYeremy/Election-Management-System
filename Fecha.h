#ifndef FECHA_H
#define FECHA_H

class Fecha {
private:
    int dia;
    int mes;
    int year;

public:
    Fecha();
    Fecha(int dia, int mes, int year);

    int getDia() const;
    int getMes() const;
    int getYear() const;
    int edadActual(int actualYear) const;
};

#endif
