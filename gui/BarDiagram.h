#pragma once

#include "Diagram.h"

class BarDiagram : public Diagram {
public:
    BarDiagram();
    ~BarDiagram();

    QChart* getChart() const override;
    void updateData(const std::vector<double>& samples) override;

private:
    QChart* chart;
    QBarSeries* series;
    QBarSet* barSet;
};
