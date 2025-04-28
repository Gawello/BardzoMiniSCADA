#include "DataLogger.h"

DataLogger::DataLogger(const QString& filename)
    : file(filename) {
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        stream.setDevice(&file);
    }
}

DataLogger::~DataLogger() {
    file.close();
}

void DataLogger::logSample(double sample) {
    if (file.isOpen()) {
        stream << QString::number(sample) << "\n";
        stream.flush();
    }
}
