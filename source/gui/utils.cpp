#include "utils.h"

#include <algorithm>

std::int8_t TryConvertToNumber(std::string str, Base base, bool *is_number) {
    std::int8_t value = 0;
    const bool is_decimal = base == Base::Decimal;
    const auto fn = is_decimal ? &isdigit : &isxdigit;
    bool valid = std::all_of(std::begin(str), std::end(str), fn);
    if (valid) {
        long int number = strtol(str.data(), nullptr, is_decimal ? 10 : 16);
        if (number > std::numeric_limits<uint8_t>::max()) {
            valid = false;
        }
        else {
            value = static_cast<std::int8_t>(number);
        }
    }
    *is_number = valid;
    return value;
}

