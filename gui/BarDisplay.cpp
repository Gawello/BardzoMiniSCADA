#include "BarDisplay.h"

BarDisplay::BarDisplay(QWidget* parent)
    : QProgressBar(parent) {
    setRange(0, 100);
}

void BarDisplay::setValue(double v) {
    DataDisplay::setValue(v);
    setValue(static_cast<int>(v));
}
