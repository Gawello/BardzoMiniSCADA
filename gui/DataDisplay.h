#pragma once

class DataDisplay {
public:
    virtual ~DataDisplay() = default;

    virtual void setValue(double value) = 0;
    virtual void setWarningLimits(double min, double max) = 0;
    virtual bool isInWarningState() const = 0;

    virtual double getWarningMin() const = 0;
    virtual double getWarningMax() const = 0;
};
