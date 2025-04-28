#include "DataBuffer.h"

DataBuffer::DataBuffer(size_t maxSize)
    : maxSize(maxSize) {}

void DataBuffer::addSample(double sample) {
    std::lock_guard<std::mutex> lock(mtx);
    if (buffer.size() >= maxSize) {
        buffer.pop_front();
    }
    buffer.push_back(sample);
}

std::vector<double> DataBuffer::getSamples() const {
    std::lock_guard<std::mutex> lock(mtx);
    return std::vector<double>(buffer.begin(), buffer.end());
}
