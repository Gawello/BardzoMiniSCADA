#pragma once

#include <QObject>
#include <QtCharts>
#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QBarSeries>
#include <vector>

// QT_BEGIN_NAMESPACE
// namespace QtCharts {

class QChartView;
}
// QT_END_NAMESPACE

class Diagram {
public:
    virtual ~Diagram() = default;

    virtual QCharts::QChart* getChart() const = 0;
    virtual void updateData(const std::vector<double>& samples) = 0;
};
