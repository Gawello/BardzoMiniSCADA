#pragma once

#include "Diagram.h"

class LineDiagram : public Diagram {
public:
    LineDiagram();
    ~LineDiagram();

    QtCharts::QChart* getChart() const override;
    void updateData(const std::vector<double>& samples) override;

private:
    QtCharts::QChart* chart;
    QtCharts::QLineSeries* series;
};
