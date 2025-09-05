#pragma once
#include <QtWidgets/QMainWindow>
#include <QtCore/QTimer>
#include <memory>
#include <vector>
#include "Dane.hpp"
#include "ZbiorDanych.hpp"
#include "WykresLiniowy.hpp"
#include "Wyswietlacz.hpp"
class OknoGlowne : public QMainWindow {
    Q_OBJECT
public:
    explicit OknoGlowne(QWidget* parent=nullptr);
private slots:
    void generujPrzykladoweDane();
private:
    std::vector<Dane<Probka>*> odbiorcy_;
    std::unique_ptr<ZbiorDanych<Probka>> magazyn_;
    std::unique_ptr<WykresLiniowy<Probka>> wykres_;
    std::unique_ptr<Wyswietlacz<Probka>> wyswietlacz_;
    QTimer timer_;
};
