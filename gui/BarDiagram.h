#pragma once

#include "Diagram.h"

class BarDiagram : public Diagram {
public:
    BarDiagram();
    ~BarDiagram();

    QtCharts::QChart* getChart() const override;
    void updateData(const std::vector<double>& samples) override;

private:
    QtCharts::QChart* chart;
    QtCharts::QBarSeries* series;
    QtCharts::QBarSet* barSet;
};
