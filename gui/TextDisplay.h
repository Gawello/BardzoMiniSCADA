#pragma once

#include <QLabel>
#include "DataDisplay.h"
#include "DisplayLogic.h"

class TextDisplay : public QLabel, public DataDisplay {
    Q_OBJECT

public:
    explicit TextDisplay(QWidget* parent = nullptr);

    void setValue(double value) override;
    void setWarningLimits(double min, double max) override;
    bool isInWarningState() const override;

    double getWarningMin() const override;
    double getWarningMax() const override;

private:
    DisplayLogic logic;
};
