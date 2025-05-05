#include "TextDisplay.h"

TextDisplay::TextDisplay(QWidget* parent)
    : QLabel(parent) {}

void TextDisplay::setValue(double value) {
    logic.setValue(value);
    setText(QString::number(value, 'f', 2));

    if (logic.isInWarningState()) {
        setStyleSheet("QLabel { color: red; }");
    } else {
        setStyleSheet("QLabel { color: black; }");
    }
}

void TextDisplay::setWarningLimits(double min, double max) {
    logic.setWarningLimits(min, max);
}

bool TextDisplay::isInWarningState() const {
    return logic.isInWarningState();
}

double TextDisplay::getWarningMin() const {
    return logic.getWarningMin();
}

double TextDisplay::getWarningMax() const {
    return logic.getWarningMax();
}
