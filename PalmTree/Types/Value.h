#pragma once

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

class LambdaNode;

class Value {
public:
    using VariantType =
        std::variant<std::monostate, int, double, std::string, bool, std::shared_ptr<const LambdaNode>>;
public:
    Value() : mut(false) { }
    Value(int v) : value(v), mut(false) {}
    Value(double v) : value(v), mut(false) {}
    Value(const std::string& v) : value(v), mut(false) {}
    Value(bool v) : value(v), mut(false) {}
    Value(std::shared_ptr<const LambdaNode> v) : value(v), mut(false) {}

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
    std::shared_ptr<const LambdaNode> asLambda() const 
        { return std::get<std::shared_ptr<const LambdaNode>>(value); }

    bool isInt() const { return std::holds_alternative<int>(value); }
    bool isDouble() const { return std::holds_alternative<double>(value); }
    bool isString() const { return std::holds_alternative<std::string>(value); }
    bool isBool() const { return std::holds_alternative<bool>(value); }
    bool isNull() const { return std::holds_alternative<std::monostate>(value); }
    bool isNumeric() const {
        return std::holds_alternative<int>(value) || std::holds_alternative<double>(value);
    }
    bool isLambda() const { return std::holds_alternative<std::shared_ptr<const LambdaNode>>(value); }

    void setMutable(const bool mut) { this->mut = mut; }
    bool isMutable() const { return mut; }

    int decimalCount() const;
    std::string to_string() const;
private:
    VariantType value;
    bool mut;
};