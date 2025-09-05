#pragma once
#include <map>
#include <type_traits>
#include <limits>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QtCore/QDateTime>

#include "Dane.hpp"
#include "Typy.hpp"

template <class T>
class WykresLiniowy : public Dane<T> {
    static_assert(std::is_same_v<T, Probka>, "WykresLiniowy w tej wersji obsługuje Probka.");
public:
    explicit WykresLiniowy(const std::string& nazwa, QWidget* parent = nullptr)
        : nazwa_(nazwa)
    {
        chart_ = new QChart();
        chart_->legend()->setVisible(true);

        axisX_ = new QDateTimeAxis();
        axisX_->setTitleText("czas");
        axisX_->setFormat("HH:mm:ss");
        axisX_->setTickCount(8); // przyjemna gęstość

        axisY_ = new QValueAxis();
        axisY_->setTitleText("wartość");
        axisY_->setLabelFormat("%.2f");

        chart_->addAxis(axisX_, Qt::AlignBottom);
        chart_->addAxis(axisY_, Qt::AlignLeft);

        view_ = new QChartView(chart_, parent);
        view_->setRenderHint(QPainter::Antialiasing);
    }

    std::string nazwa() const override { return nazwa_; }
    QChartView* widzet() const { return view_; }

    void dodaj(const T& v) override {
        const auto key = v.etykieta;

        // Seria per etykieta, tworzona leniwie
        if (!serie_.count(key)) {
            auto* s = new QLineSeries();
            s->setName(key);
            chart_->addSeries(s);
            s->attachAxis(axisX_);
            s->attachAxis(axisY_);
            serie_[key] = s;
        }

        // Uwaga: oś czasu jest QDateTimeAxis -> x to ms od epoki
        auto* s = serie_[key];
        s->append(v.t, v.wartosc); // v.t to teraz ms (patrz OknoGlowne.cpp)

        // auto-skalowanie Y (na podstawie min/max)
        yMin_ = std::min(yMin_, v.wartosc);
        yMax_ = std::max(yMax_, v.wartosc);
        double span = yMax_ - yMin_;
        if (span < 1e-6) span = 1.0; // gdy wszystkie wartości równe
        axisY_->setRange(yMin_ - 0.05*span, yMax_ + 0.05*span);

        // okno 10 min na osi X
        const qint64 now_ms = static_cast<qint64>(v.t);
        const QDateTime now = QDateTime::fromMSecsSinceEpoch(now_ms);
        axisX_->setRange(now.addSecs(-600), now);

        ++count_;
    }

    using Dane<T>::dodaj;

    std::size_t rozmiar() const override { return count_; }
    std::optional<T> ostatnia() const override { return std::nullopt; }

private:
    std::string nazwa_;

    QChart*         chart_{nullptr};
    QChartView*     view_{nullptr};
    QDateTimeAxis*  axisX_{nullptr};
    QValueAxis*     axisY_{nullptr};
    std::map<QString, QLineSeries*> serie_;

    // prosty tracking do auto-skali Y
    double yMin_ = std::numeric_limits<double>::infinity();
    double yMax_ = -std::numeric_limits<double>::infinity();

    std::size_t count_ = 0;
};
