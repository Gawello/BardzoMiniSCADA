#include "BarDiagram.h"
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>

BarDiagram::BarDiagram() {
    chart = new QChart();
    barSet = new QBarSet("Data");
    series = new QBarSeries();
    series->append(barSet);
    chart->addSeries(series);
    chart->createDefaultAxes();
}

BarDiagram::~BarDiagram() {
    delete chart;
}

QChart* BarDiagram::getChart() const {
    return chart;
}

void BarDiagram::updateData(const std::vector<double>& samples) {
    barSet->remove(0, barSet->count());
    for (const auto& sample : samples) {
        *barSet << sample;
    }
    chart->axes(Qt::Vertical).first()->setRange(*std::min_element(samples.begin(), samples.end()), *std::max_element(samples.begin(), samples.end()));
}
