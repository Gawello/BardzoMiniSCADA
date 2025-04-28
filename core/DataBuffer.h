#pragma once

#include <vector>
#include <deque>
#include <mutex>

class DataBuffer {
public:
    explicit DataBuffer(size_t maxSize = 1000);

    void addSample(double sample);
    std::vector<double> getSamples() const;

private:
    size_t maxSize;
    std::deque<double> buffer;
    mutable std::mutex mtx;
};
