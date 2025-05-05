#include "DisplayLogic.h"

void DisplayLogic::setValue(double v) {
    value = v;
}

void DisplayLogic::setWarningLimits(double min, double max) {
    minWarning = min;
    maxWarning = max;
}

bool DisplayLogic::isInWarningState() const {
    return value < minWarning || value > maxWarning;
}
