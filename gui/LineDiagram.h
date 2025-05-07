#pragma once

#include "Diagram.h"

class LineDiagram : public Diagram {
    Q_OBJECT
public:
    LineDiagram();
    ~LineDiagram();

    QChart* getChart() const override;
    void updateData(const std::vector<double>& samples) override;
    void setColor(const QColor& color);

private:
    QChart* chart;
    QLineSeries* series;
};
