#include "TextDisplay.h"

TextDisplay::TextDisplay(QWidget* parent)
    : QLabel(parent) {}

void TextDisplay::setValue(double v) {
    DataDisplay::setValue(v);
    QString text = QString::number(v, 'f', 2);
    setText(text);

    if (isInWarningState()) {
        setStyleSheet("QLabel { color: red; }");
    } else {
        setStyleSheet("QLabel { color: black; }");
    }
}
