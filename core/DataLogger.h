#pragma once

#include <QString>
#include <QFile>
#include <QTextStream>

class DataLogger {
public:
    explicit DataLogger(const QString& filename);
    ~DataLogger();

    void logSample(double sample);

private:
    QFile file;
    QTextStream stream;
};
