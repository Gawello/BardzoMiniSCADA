#include "BarDisplay.h"

BarDisplay::BarDisplay(QWidget* parent)
    : QProgressBar(parent) {}

void BarDisplay::setValue(double value) {
    logic.setValue(value);
    QProgressBar::setValue(static_cast<int>(value));

    if (logic.isInWarningState()) {
        setStyleSheet("QProgressBar { background-color: red; }");
    } else {
        setStyleSheet("");
    }
}

void BarDisplay::setWarningLimits(double min, double max) {
    logic.setWarningLimits(min, max);
}

bool BarDisplay::isInWarningState() const {
    return logic.isInWarningState();
}

double BarDisplay::getWarningMin() const {
    return logic.getWarningMin();
}

double BarDisplay::getWarningMax() const {
    return logic.getWarningMax();
}
