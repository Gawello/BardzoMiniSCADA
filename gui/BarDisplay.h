#pragma once

#include <QProgressBar>
#include "DataDisplay.h"
#include "DisplayLogic.h"

class BarDisplay : public QProgressBar, public DataDisplay {
    Q_OBJECT

public:
    explicit BarDisplay(QWidget* parent = nullptr);

    void setValue(double value) override;
    void setWarningLimits(double min, double max) override;
    bool isInWarningState() const override;

    double getWarningMin() const override;
    double getWarningMax() const override;

private:
    DisplayLogic logic;
};
