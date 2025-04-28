#include "LineDiagram.h"
#include <QtCharts/QValueAxis>

LineDiagram::LineDiagram() {
    chart = new QtCharts::QChart();
    series = new QtCharts::QLineSeries();
    chart->addSeries(series);
    chart->createDefaultAxes();
}

LineDiagram::~LineDiagram() {
    delete chart;
}

QtCharts::QChart* LineDiagram::getChart() const {
    return chart;
}

void LineDiagram::updateData(const std::vector<double>& samples) {
    series->clear();
    for (int i = 0; i < samples.size(); ++i) {
        series->append(i, samples[i]);
    }
    chart->axes(Qt::Horizontal).first()->setRange(0, samples.size());
    chart->axes(Qt::Vertical).first()->setRange(*std::min_element(samples.begin(), samples.end()), *std::max_element(samples.begin(), samples.end()));
}
