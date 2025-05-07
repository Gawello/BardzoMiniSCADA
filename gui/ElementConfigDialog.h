#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QColor>

class ElementConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit ElementConfigDialog(QWidget* parent = nullptr);

    void setTitle(const QString& title);
    QString getTitle() const;

    void setWarningLimits(double min, double max);
    double getWarningMin() const;
    double getWarningMax() const;

    void enableWarningControls(bool enable);

private:
    QLineEdit* titleEdit;
    QDoubleSpinBox* minWarningSpin;
    QDoubleSpinBox* maxWarningSpin;
    QDialogButtonBox* buttons;
    QPushButton* colorButton;
    QColor currentColor;

public:
    void enableColorControl(bool enable);
    void setColor(const QColor& color);
    QColor getColor() const;
};
