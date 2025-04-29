#include "DataDisplay.h"

void DataDisplay::setValue(double v) {
    value = v;
}

void DataDisplay::setWarningLimits(double min, double max) {
    minWarning = min;
    maxWarning = max;
}

bool DataDisplay::isInWarningState() const {
    return value < minWarning || value > maxWarning;
}
