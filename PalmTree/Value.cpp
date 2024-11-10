#include "Value.h"

int Value::decimalCount() const {
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
    return ct - trail;
}

std::string Value::to_string() const {
    if (isInt()) return std::to_string(asInt());
    else if (isDouble()) return std::to_string(asDouble());
    else if (isBool()) return asBool() ? "true" : "false"; // probably not great
    else if (isString()) return asString();
}

//
// Operator overloads
//

Value Value::operator+(const Value& other) const {
    if (isInt() && other.isInt()) return Value(asInt() + other.asInt());
    if (isDouble() && other.isDouble()) return Value(asDouble() + other.asDouble());
    if (isInt() && other.isDouble()) return Value(asInt() + other.asDouble());
    if (isDouble() && other.isInt()) return Value(asDouble() + other.asInt());
    throw std::runtime_error("Unsupported types for addition");
}
Value Value::operator-(const Value& other) const {
    if (isInt() && other.isInt()) return Value(asInt() - other.asInt());
    if (isDouble() && other.isDouble()) return Value(asDouble() - other.asDouble());
    if (isInt() && other.isDouble()) return Value(asInt() - other.asDouble());
    if (isDouble() && other.isInt()) return Value(asDouble() - other.asInt());
    throw std::runtime_error("Unsupported types for subtraction");
}
Value Value::operator*(const Value& other) const {
    if (isInt() && other.isInt()) return Value(asInt() * other.asInt());
    if (isDouble() && other.isDouble()) return Value(asDouble() * other.asDouble());
    if (isInt() && other.isDouble()) return Value(asInt() * other.asDouble());
    if (isDouble() && other.isInt()) return Value(asDouble() * other.asInt());
    throw std::runtime_error("Unsupported types for multiplication");
}
Value Value::operator/(const Value& other) const {
    if (isInt() && other.isInt()) return Value(asInt() / other.asInt());
    if (isDouble() && other.isDouble()) return Value(asDouble() / other.asDouble());
    if (isInt() && other.isDouble()) return Value(asInt() / other.asDouble());
    if (isDouble() && other.isInt()) return Value(asDouble() / other.asInt());
    throw std::runtime_error("Unsupported types for division");
}

bool Value::operator==(const Value& other) const {
    if (isInt() && other.isInt()) return asInt() == other.asInt();
    if (isDouble() && other.isDouble()) return asDouble() == other.asDouble();
    if (isInt() && other.isDouble()) return asInt() == other.asDouble();
    if (isDouble() && other.isInt()) return asDouble() == other.asInt();
    throw std::runtime_error("Unsupported types for equality");
}
bool Value::operator==(const double other) const {
    if (isDouble()) return asDouble() == other;
    else if (isInt()) return asInt() == other;
    throw std::runtime_error("Unsupported types for equality");
}
bool Value::operator==(const int other) const {
    if (isInt()) return asInt() == other;
    else if (isDouble()) return asDouble() == other;
    throw std::runtime_error("Unsupported types for equality");
}
bool Value::operator!=(const Value& other) const {
    if (isInt() && other.isInt()) return asInt() != other.asInt();
    if (isDouble() && other.isDouble()) return asDouble() != other.asDouble();
    if (isInt() && other.isDouble()) return asInt() != other.asDouble();
    if (isDouble() && other.isInt()) return asDouble() != other.asInt();
    throw std::runtime_error("Unsupported types for inequality");
}
bool Value::operator!=(const double other) const {
    if (isDouble()) return asDouble() != other;
    else if (isInt()) return asInt() != other;
    throw std::runtime_error("Unsupported types for inequality");
}
bool Value::operator!=(const int other) const {
    if (isInt()) return asInt() != other;
    else if (isDouble()) return asDouble() != other;
    throw std::runtime_error("Unsupported types for inequality");
}