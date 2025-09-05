#pragma once
#include <string>
#include <cstddef>
#include <optional>
template <class T>
class Dane {
public:
    virtual ~Dane() = default;
    virtual std::string nazwa() const = 0;
    virtual void dodaj(const T& v) = 0;
    virtual void dodaj(T&& v) { dodaj(static_cast<const T&>(v)); }
    virtual std::size_t rozmiar() const = 0;
    virtual std::optional<T> ostatnia() const = 0;
};
