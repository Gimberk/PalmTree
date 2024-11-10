#pragma once

#include <variant>
#include <string>
#include <iostream>
#include <unordered_map>
#include <stdexcept>

class Value {
public:
    using VariantType = std::variant<int, double, std::string, bool>;
public:
    Value() { }
    Value(int v) : value(v), mut(false) {}
    Value(double v) : value(v), mut(false) {}
    Value(const std::string& v) : value(v), mut(false) {}
    Value(bool v) : value(v), mut(false) {}

    const VariantType& get() const { return value; }
public:
    Value operator+(const Value& other) const;
    Value operator-(const Value& other) const;
    Value operator*(const Value& other) const;
    Value operator/(const Value& other) const;
public:
    bool operator==(const Value& other) const;
    bool operator==(const double other) const;
    bool operator==(const int other) const;
    bool operator!=(const Value& other) const;
    bool operator!=(const double other) const;
    bool operator!=(const int other) const;
public:
    int asInt() const { return std::get<int>(value); }
    double asDouble() const { return std::get<double>(value); }
    std::string asString() const { return std::get<std::string>(value); }
    bool asBool() const { return std::get<bool>(value); }

    bool isInt() const { return std::holds_alternative<int>(value); }
    bool isDouble() const { return std::holds_alternative<double>(value); }
    bool isString() const { return std::holds_alternative<std::string>(value); }
    bool isBool() const { return std::holds_alternative<bool>(value); }

    void setMutable(const bool mut) { this->mut = mut; }
    bool isMutable() const { return mut; }

    int decimalCount() const;
    std::string to_string() const;
private:
    VariantType value;
    bool mut;
};