#pragma once

#include <cctype>
#include <cstdlib>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace arclab {

struct JsonValue {
    using Array = std::vector<JsonValue>;
    using Object = std::map<std::string, JsonValue>;
    using Storage = std::variant<std::nullptr_t, bool, double, std::string, Array, Object>;

    Storage value = nullptr;

    bool is_null() const { return std::holds_alternative<std::nullptr_t>(value); }
    bool is_bool() const { return std::holds_alternative<bool>(value); }
    bool is_number() const { return std::holds_alternative<double>(value); }
    bool is_string() const { return std::holds_alternative<std::string>(value); }
    bool is_array() const { return std::holds_alternative<Array>(value); }
    bool is_object() const { return std::holds_alternative<Object>(value); }

    const Array* array() const { return std::get_if<Array>(&value); }
    const Object* object() const { return std::get_if<Object>(&value); }
    const std::string* string() const { return std::get_if<std::string>(&value); }
    const double* number() const { return std::get_if<double>(&value); }
    const bool* boolean() const { return std::get_if<bool>(&value); }
};

struct JsonParseResult {
    std::optional<JsonValue> value;
    std::string error;
};

class JsonParser {
public:
    explicit JsonParser(std::string_view text)
        : text_(text)
    {
    }

    JsonParseResult parse()
    {
        skip_whitespace();
        std::optional<JsonValue> value = parse_value();
        if (!value) {
            return { std::nullopt, error_ };
        }

        skip_whitespace();
        if (!at_end()) {
            return { std::nullopt, "Unexpected trailing JSON content." };
        }

        return { value, {} };
    }

private:
    bool at_end() const
    {
        return index_ >= text_.size();
    }

    char peek() const
    {
        return at_end() ? '\0' : text_[index_];
    }

    char advance()
    {
        return at_end() ? '\0' : text_[index_++];
    }

    bool consume(char expected)
    {
        if (peek() != expected) {
            return false;
        }

        ++index_;
        return true;
    }

    void skip_whitespace()
    {
        while (!at_end() && std::isspace(static_cast<unsigned char>(peek())) != 0) {
            ++index_;
        }
    }

    std::optional<JsonValue> parse_value()
    {
        skip_whitespace();
        if (at_end()) {
            error_ = "Unexpected end of JSON.";
            return std::nullopt;
        }

        switch (peek()) {
        case '{':
            return parse_object();
        case '[':
            return parse_array();
        case '"':
            return parse_string_value();
        case 't':
            return parse_literal("true", JsonValue { true });
        case 'f':
            return parse_literal("false", JsonValue { false });
        case 'n':
            return parse_literal("null", JsonValue {});
        default:
            if (peek() == '-' || std::isdigit(static_cast<unsigned char>(peek())) != 0) {
                return parse_number();
            }
            error_ = "Unexpected JSON value.";
            return std::nullopt;
        }
    }

    std::optional<JsonValue> parse_literal(std::string_view literal, JsonValue value)
    {
        if (text_.substr(index_, literal.size()) != literal) {
            error_ = "Invalid JSON literal.";
            return std::nullopt;
        }

        index_ += literal.size();
        return value;
    }

    std::optional<JsonValue> parse_object()
    {
        consume('{');
        JsonValue::Object object;
        skip_whitespace();

        if (consume('}')) {
            return JsonValue { object };
        }

        while (!at_end()) {
            skip_whitespace();
            std::optional<std::string> key = parse_string();
            if (!key) {
                error_ = "Expected JSON object key.";
                return std::nullopt;
            }

            skip_whitespace();
            if (!consume(':')) {
                error_ = "Expected ':' after JSON object key.";
                return std::nullopt;
            }

            std::optional<JsonValue> value = parse_value();
            if (!value) {
                return std::nullopt;
            }

            object.emplace(*key, *value);

            skip_whitespace();
            if (consume('}')) {
                return JsonValue { object };
            }

            if (!consume(',')) {
                error_ = "Expected ',' or '}' in JSON object.";
                return std::nullopt;
            }
        }

        error_ = "Unterminated JSON object.";
        return std::nullopt;
    }

    std::optional<JsonValue> parse_array()
    {
        consume('[');
        JsonValue::Array array;
        skip_whitespace();

        if (consume(']')) {
            return JsonValue { array };
        }

        while (!at_end()) {
            std::optional<JsonValue> value = parse_value();
            if (!value) {
                return std::nullopt;
            }

            array.push_back(*value);

            skip_whitespace();
            if (consume(']')) {
                return JsonValue { array };
            }

            if (!consume(',')) {
                error_ = "Expected ',' or ']' in JSON array.";
                return std::nullopt;
            }
        }

        error_ = "Unterminated JSON array.";
        return std::nullopt;
    }

    std::optional<JsonValue> parse_string_value()
    {
        std::optional<std::string> value = parse_string();
        if (!value) {
            return std::nullopt;
        }

        return JsonValue { *value };
    }

    std::optional<std::string> parse_string()
    {
        if (!consume('"')) {
            return std::nullopt;
        }

        std::string result;
        while (!at_end()) {
            const char character = advance();
            if (character == '"') {
                return result;
            }

            if (character != '\\') {
                result.push_back(character);
                continue;
            }

            if (at_end()) {
                error_ = "Unterminated JSON escape.";
                return std::nullopt;
            }

            const char escaped = advance();
            switch (escaped) {
            case '"':
            case '\\':
            case '/':
                result.push_back(escaped);
                break;
            case 'b':
                result.push_back('\b');
                break;
            case 'f':
                result.push_back('\f');
                break;
            case 'n':
                result.push_back('\n');
                break;
            case 'r':
                result.push_back('\r');
                break;
            case 't':
                result.push_back('\t');
                break;
            default:
                error_ = "Unsupported JSON string escape.";
                return std::nullopt;
            }
        }

        error_ = "Unterminated JSON string.";
        return std::nullopt;
    }

    std::optional<JsonValue> parse_number()
    {
        const std::size_t start = index_;
        if (peek() == '-') {
            ++index_;
        }

        while (!at_end() && std::isdigit(static_cast<unsigned char>(peek())) != 0) {
            ++index_;
        }

        if (peek() == '.') {
            ++index_;
            while (!at_end() && std::isdigit(static_cast<unsigned char>(peek())) != 0) {
                ++index_;
            }
        }

        if (peek() == 'e' || peek() == 'E') {
            ++index_;
            if (peek() == '+' || peek() == '-') {
                ++index_;
            }
            while (!at_end() && std::isdigit(static_cast<unsigned char>(peek())) != 0) {
                ++index_;
            }
        }

        const std::string numberText(text_.substr(start, index_ - start));
        char* end = nullptr;
        const double number = std::strtod(numberText.c_str(), &end);
        if (end == numberText.c_str() || *end != '\0') {
            error_ = "Invalid JSON number.";
            return std::nullopt;
        }

        return JsonValue { number };
    }

    std::string_view text_;
    std::size_t index_ = 0;
    std::string error_;
};

inline JsonParseResult parse_json(std::string_view text)
{
    return JsonParser(text).parse();
}

inline const JsonValue* find_json_member(const JsonValue& value, std::string_view key)
{
    const JsonValue::Object* object = value.object();
    if (object == nullptr) {
        return nullptr;
    }

    const auto found = object->find(std::string(key));
    if (found == object->end()) {
        return nullptr;
    }

    return &found->second;
}

} // namespace arclab
