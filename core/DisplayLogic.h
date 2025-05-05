#pragma once

class DisplayLogic {
public:
    void setValue(double value);
    void setWarningLimits(double min, double max);
    bool isInWarningState() const;

    double getWarningMin() const { return minWarning; }
    double getWarningMax() const { return maxWarning; }

private:
    double value = 0.0;
    double minWarning = 0.0;
    double maxWarning = 100.0;
};
