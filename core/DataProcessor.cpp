#include "DataProcessor.h"
#include <QtMath> // dla funkcji matematycznych

DataProcessor::DataProcessor(const QString& expression)
    : expression(expression) {}

void DataProcessor::setExpression(const QString& expr) {
    expression = expr;
}

double DataProcessor::process(double input) const {
    return evaluateExpression(input);
}

// Bardzo prosty parser — przykładowy (tylko x, x^2, sqrt(x))
double DataProcessor::evaluateExpression(double x) const {
    if (expression == "x")
        return x;
    else if (expression == "x^2")
        return x * x;
    else if (expression == "sqrt(x)")
        return qSqrt(x);
    else
        return x; // fallback
}
