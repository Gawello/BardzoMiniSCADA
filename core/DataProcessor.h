#pragma once

#include <QString>

class DataProcessor {
public:
    explicit DataProcessor(const QString& expression = "x");
    double process(double input) const;
    void setExpression(const QString& expr);

private:
    QString expression;

    double evaluateExpression(double x) const;
};
