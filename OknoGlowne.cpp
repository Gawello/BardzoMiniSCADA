#include "OknoGlowne.hpp"
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QStatusBar>
#include <QtCore/QDateTime>
#include <random>
OknoGlowne::OknoGlowne(QWidget* parent) : QMainWindow(parent) {
    magazyn_    = std::make_unique<ZbiorDanych<Probka>>("Magazyn", 600);
    wykres_     = std::make_unique<WykresLiniowy<Probka>>("Wykres", this);
    wyswietlacz_= std::make_unique<Wyswietlacz<Probka>>("Aktualne wartości", this);
    odbiorcy_.push_back(magazyn_.get());
    odbiorcy_.push_back(wykres_.get());
    odbiorcy_.push_back(wyswietlacz_.get());
    auto* central = new QWidget(this);
    auto* rootLay = new QHBoxLayout(central);
    rootLay->addWidget(wykres_->widzet(), 3);
    rootLay->addWidget(wyswietlacz_->widzet(), 1);
    setCentralWidget(central);
    statusBar()->showMessage("Start — generuję próbki co 1 s");
    connect(&timer_, &QTimer::timeout, this, &OknoGlowne::generujPrzykladoweDane);
    timer_.start(1000);
}
void OknoGlowne::generujPrzykladoweDane() {
    const double t = QDateTime::currentMSecsSinceEpoch();
    static std::mt19937 rng(42);
    static std::normal_distribution<double> g(0.0, 0.05);
    Probka cisnienie{ t, 10.0 + g(rng), "Ciśnienie [bar]" };
    Probka temperatura{ t, 25.0 + 0.5 * g(rng), "Temperatura [°C]" };
    static bool stan = false; stan = !stan;
    Probka zawor{ t, stan ? 1.0 : 0.0, "Zawór (0/1)" };
    for (auto* d : odbiorcy_) d->dodaj(cisnienie);
    for (auto* d : odbiorcy_) d->dodaj(temperatura);
    for (auto* d : odbiorcy_) d->dodaj(std::move(zawor));
    statusBar()->showMessage(QString("Próbek w magazynie: %1").arg(magazyn_->rozmiar()));
}
