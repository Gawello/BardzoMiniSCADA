#pragma once
#include <vector>
#include <algorithm>
#include <mutex>
#include <optional>
#include "Dane.hpp"
template <class T>
class ZbiorDanych : public Dane<T> {
public:
    explicit ZbiorDanych(std::string n, std::size_t capacity = 600)
        : nazwa_(std::move(n)), capacity_(capacity) {}
    std::string nazwa() const override { return nazwa_; }
    void dodaj(const T& v) override {
        std::scoped_lock lk(mtx_);
        if (buf_.size() == capacity_) buf_.erase(buf_.begin());
        buf_.push_back(v); last_ = v;
    }
    void dodaj(T&& v) override {
        std::scoped_lock lk(mtx_);
        if (buf_.size() == capacity_) buf_.erase(buf_.begin());
        last_ = v; buf_.push_back(std::move(v));
    }
    std::size_t rozmiar() const override { std::scoped_lock lk(mtx_); return buf_.size(); }
    std::optional<T> ostatnia() const override { std::scoped_lock lk(mtx_); return last_; }
    const std::vector<T>& dane() const { return buf_; }
private:
    std::string nazwa_;
    std::size_t capacity_;
    mutable std::mutex mtx_;
    std::vector<T> buf_;
    std::optional<T> last_;
};
