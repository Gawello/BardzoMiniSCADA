#pragma once

#include "DataDisplay.h"
#include <QLabel>

class TextDisplay : public QLabel, public DataDisplay {
    Q_OBJECT

public:
    explicit TextDisplay(QWidget* parent = nullptr);

    void setValue(double value) override;
};
