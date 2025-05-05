#include "ElementConfigDialog.h"
#include <QVBoxLayout>
#include <QLabel>

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
