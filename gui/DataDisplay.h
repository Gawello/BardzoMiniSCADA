#pragma once

#include <QString>
#include <QObject>

class DataDisplay{
public:
    virtual ~DataDisplay() = default;

    virtual void setValue(double value);
    virtual void setWarningLimits(double min, double max);
    virtual bool isInWarningState() const;

protected:
    double value = 0.0;
    double minWarning = 0.0;
    double maxWarning = 100.0;
};
