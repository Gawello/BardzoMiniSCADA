#include "ElementConfigDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>

ElementConfigDialog::ElementConfigDialog(QWidget* parent)
    : QDialog(parent) {

    QVBoxLayout* layout = new QVBoxLayout(this);

    titleEdit = new QLineEdit(this);
    layout->addWidget(new QLabel("Tytuł:", this));
    layout->addWidget(titleEdit);

    minWarningSpin = new QDoubleSpinBox(this);
    maxWarningSpin = new QDoubleSpinBox(this);
    minWarningSpin->setRange(-100000, 100000);
    maxWarningSpin->setRange(-100000, 100000);
    layout->addWidget(new QLabel("Próg dolny (ostrzeżenie):", this));
    layout->addWidget(minWarningSpin);
    layout->addWidget(new QLabel("Próg górny (ostrzeżenie):", this));
    layout->addWidget(maxWarningSpin);

    buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    colorButton = new QPushButton("Wybierz kolor", this);
    layout->addWidget(colorButton);
    colorButton->setEnabled(false);

    connect(colorButton, &QPushButton::clicked, this, [this]() {
        QColor color = QColorDialog::getColor(currentColor, this, "Wybierz kolor linii");
        if (color.isValid()) {
            currentColor = color;
            colorButton->setText(color.name());
        }
    });

}

void ElementConfigDialog::setTitle(const QString& title) {
    titleEdit->setText(title);
}

QString ElementConfigDialog::getTitle() const {
    return titleEdit->text();
}

void ElementConfigDialog::setWarningLimits(double min, double max) {
    minWarningSpin->setValue(min);
    maxWarningSpin->setValue(max);
}

double ElementConfigDialog::getWarningMin() const {
    return minWarningSpin->value();
}

double ElementConfigDialog::getWarningMax() const {
    return maxWarningSpin->value();
}

void ElementConfigDialog::enableWarningControls(bool enable) {
    minWarningSpin->setEnabled(enable);
    maxWarningSpin->setEnabled(enable);
}
void ElementConfigDialog::enableColorControl(bool enable) {
    colorButton->setEnabled(enable);
}

void ElementConfigDialog::setColor(const QColor& color) {
    currentColor = color;
    colorButton->setText(color.name());
}

QColor ElementConfigDialog::getColor() const {
    return currentColor;
}
