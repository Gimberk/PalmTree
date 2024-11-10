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
    Value() { throw new std::runtime_error("Type required!"); }
    Value(int v) : value(v) {}
    Value(double v) : value(v) {}
    Value(const std::string& v) : value(v) {}
    Value(bool v) : value(v) {}

    const VariantType& get() const { return value; }
public:
    Value operator+(const Value& other) const {
        if (isInt() && other.isInt()) return Value(asInt() + other.asInt());
        if (isDouble() && other.isDouble()) return Value(asDouble() + other.asDouble());
        if (isInt() && other.isDouble()) return Value(asInt() + other.asDouble());
        if (isDouble() && other.isInt()) return Value(asDouble() + other.asInt());
        throw std::runtime_error("Unsupported types for addition");
    }
    Value operator-(const Value& other) const {
        if (isInt() && other.isInt()) return Value(asInt() - other.asInt());
        if (isDouble() && other.isDouble()) return Value(asDouble() - other.asDouble());
        if (isInt() && other.isDouble()) return Value(asInt() - other.asDouble());
        if (isDouble() && other.isInt()) return Value(asDouble() - other.asInt());
        throw std::runtime_error("Unsupported types for subtraction");
    }
    Value operator*(const Value& other) const {
        if (isInt() && other.isInt()) return Value(asInt() * other.asInt());
        if (isDouble() && other.isDouble()) return Value(asDouble() * other.asDouble());
        if (isInt() && other.isDouble()) return Value(asInt() * other.asDouble());
        if (isDouble() && other.isInt()) return Value(asDouble() * other.asInt());
        throw std::runtime_error("Unsupported types for multiplication");
    }
    Value operator/(const Value& other) const {
        if (isInt() && other.isInt()) return Value(asInt() / other.asInt());
        if (isDouble() && other.isDouble()) return Value(asDouble() / other.asDouble());
        if (isInt() && other.isDouble()) return Value(asInt() / other.asDouble());
        if (isDouble() && other.isInt()) return Value(asDouble() / other.asInt());
        throw std::runtime_error("Unsupported types for division");
    }
public:
    //
    // For all the comparisons
    //
    bool operator==(const Value& other) const {
        if (isInt() && other.isInt()) return asInt() == other.asInt();
        if (isDouble() && other.isDouble()) return asDouble() == other.asDouble();
        if (isInt() && other.isDouble()) return asInt() == other.asDouble();
        if (isDouble() && other.isInt()) return asDouble() == other.asInt();
        throw std::runtime_error("Unsupported types for equality");
    }
    bool operator==(const double other) const {
        if (isDouble()) return asDouble() == other;
        else if (isInt()) return asInt() == other;
        throw std::runtime_error("Unsupported types for equality");
    }
    bool operator==(const int other) const {
        if (isInt()) return asInt() == other;
        else if (isDouble()) return asDouble() == other;
        throw std::runtime_error("Unsupported types for equality");
    }
    bool operator!=(const Value& other) const {
        if (isInt() && other.isInt()) return asInt() != other.asInt();
        if (isDouble() && other.isDouble()) return asDouble() != other.asDouble();
        if (isInt() && other.isDouble()) return asInt() != other.asDouble();
        if (isDouble() && other.isInt()) return asDouble() != other.asInt();
        throw std::runtime_error("Unsupported types for inequality");
    }
    bool operator!=(const double other) const {
        if (isDouble()) return asDouble() != other;
        else if (isInt()) return asInt() != other;
        throw std::runtime_error("Unsupported types for inequality");
    }
    bool operator!=(const int other) const {
        if (isInt()) return asInt() != other;
        else if (isDouble()) return asDouble() != other;
        throw std::runtime_error("Unsupported types for inequality");
    }
public:
    int asInt() const { return std::get<int>(value); }
    double asDouble() const { return std::get<double>(value); }
    std::string asString() const { return std::get<std::string>(value); }
    bool asBool() const { return std::get<bool>(value); }

    bool isInt() const { return std::holds_alternative<int>(value); }
    bool isDouble() const { return std::holds_alternative<double>(value); }
    bool isString() const { return std::holds_alternative<std::string>(value); }
    bool isBool() const { return std::holds_alternative<bool>(value); }

    int decimalCount() const {
        if (!isDouble()) throw std::runtime_error("Invalid Value Type!");
        std::string str = to_string();
        int ct = 0, trail = 0;
        bool count = false, end = false;
        for (char c : str) {
            if (c == '.') {
                count = true; continue;
            }
            else if (c == '0') {
                end = true; trail++;
            }
            else {
                end = false; trail = 0;
            }
            if (!count) continue;
            ct++;
        }
        return ct-trail;
    }

    std::string to_string() const {
        if (isInt()) return std::to_string(asInt());
        else if (isDouble()) return std::to_string(asDouble());
        else if (isBool()) return asBool() ? "true" : "false"; // probably not great
        else if (isString()) return asString();
    }
private:
    VariantType value;
};