#pragma once

#include "Diagram.h"

class LineDiagram : public Diagram {
public:
    LineDiagram();
    ~LineDiagram();

    QChart* getChart() const override;
    void updateData(const std::vector<double>& samples) override;

private:
    QChart* chart;
    QLineSeries* series;
};
