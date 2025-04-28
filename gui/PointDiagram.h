#pragma once

#include "Diagram.h"

class PointDiagram : public Diagram {
public:
    PointDiagram();
    ~PointDiagram();

    QtCharts::QChart* getChart() const override;
    void updateData(const std::vector<double>& samples) override;

private:
    QtCharts::QChart* chart;
    QtCharts::QScatterSeries* series;
};
