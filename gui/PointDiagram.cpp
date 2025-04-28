#include "PointDiagram.h"
#include <QtCharts/QValueAxis>

PointDiagram::PointDiagram() {
    chart = new QtCharts::QChart();
    series = new QtCharts::QScatterSeries();
    chart->addSeries(series);
    chart->createDefaultAxes();
}

PointDiagram::~PointDiagram() {
    delete chart;
}

QtCharts::QChart* PointDiagram::getChart() const {
    return chart;
}

void PointDiagram::updateData(const std::vector<double>& samples) {
    series->clear();
    for (int i = 0; i < samples.size(); ++i) {
        series->append(i, samples[i]);
    }
    chart->axes(Qt::Horizontal).first()->setRange(0, samples.size());
    chart->axes(Qt::Vertical).first()->setRange(*std::min_element(samples.begin(), samples.end()), *std::max_element(samples.begin(), samples.end()));
}
