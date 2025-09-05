#pragma once
#include <map>
#include <type_traits>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include "Dane.hpp"
#include "Typy.hpp"
template <class T>
class Wyswietlacz : public Dane<T> {
    static_assert(std::is_same_v<T, Probka>, "Wyswietlacz w tej wersji obsługuje Probka.");
public:
    explicit Wyswietlacz(const std::string& nazwa, QWidget* parent=nullptr) : nazwa_(nazwa) {
        root_ = new QWidget(parent); auto* lay = new QVBoxLayout(root_);
        root_->setLayout(lay); opis_ = new QLabel(QString::fromStdString(nazwa_));
        lay->addWidget(opis_); lay->addStretch();
    }
    std::string nazwa() const override { return nazwa_; }
    QWidget* widzet() const { return root_; }
    void dodaj(const T& v) override {
        QLabel*& lab = map_[v.etykieta];
        if (!lab) { lab = new QLabel(root_); root_->layout()->addWidget(lab); }
        lab->setText(QString("%1 = %2").arg(v.etykieta).arg(v.wartosc, 0, 'f', 2));
        last_ = v; ++count_;
    }
    using Dane<T>::dodaj;
    std::size_t rozmiar() const override { return count_; }
    std::optional<T> ostatnia() const override { return last_; }
private:
    std::string nazwa_; QWidget* root_{nullptr}; QLabel* opis_{nullptr};
    std::map<QString, QLabel*> map_; std::optional<T> last_; std::size_t count_ = 0;
};
