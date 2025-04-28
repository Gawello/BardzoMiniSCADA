#pragma once

#include "DataDisplay.h"
#include <QProgressBar>

class BarDisplay : public QProgressBar, public DataDisplay {
    Q_OBJECT

public:
    explicit BarDisplay(QWidget* parent = nullptr);

    void setValue(double value) override;
};
