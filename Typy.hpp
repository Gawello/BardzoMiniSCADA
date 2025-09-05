#pragma once
#include <QString>
#include <ostream>
struct Probka {
    double t{};
    double wartosc{};
    QString etykieta{};
    friend std::ostream& operator<<(std::ostream& os, const Probka& p) {
        os << "t=" << p.t << ", " << p.etykieta.toStdString() << " = " << p.wartosc;
        return os;
    }
};
