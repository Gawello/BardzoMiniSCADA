#pragma once

#include "Diagram.h"

class PointDiagram : public Diagram {
public:
    PointDiagram();
    ~PointDiagram();

    QChart* getChart() const override;
    void updateData(const std::vector<double>& samples) override;

private:
    QChart* chart;
    QScatterSeries* series;
};
