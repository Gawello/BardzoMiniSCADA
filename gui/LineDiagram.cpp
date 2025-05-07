#include "LineDiagram.h"
#include <QtCharts/QValueAxis>

LineDiagram::LineDiagram() {
    chart = new QChart();
    series = new QLineSeries();
    chart->addSeries(series);
    chart->createDefaultAxes();
}

LineDiagram::~LineDiagram() {
    delete chart;
}

QChart* LineDiagram::getChart() const {
    return chart;
}

void LineDiagram::updateData(const std::vector<double>& samples) {
    series->clear();
    for (int i = 0; i < samples.size(); ++i) {
        series->append(i, samples[i]);
    }
    chart->axes(Qt::Horizontal).first()->setRange(0.0, static_cast<double>(samples.size()));
    chart->axes(Qt::Vertical).first()->setRange(*std::min_element(samples.begin(), samples.end()), *std::max_element(samples.begin(), samples.end()));
}

void LineDiagram::setColor(const QColor& color) {
    QPen p = series->pen();
    p.setColor(color);
    series->setPen(p);
}
