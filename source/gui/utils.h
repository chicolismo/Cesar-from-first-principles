#ifndef CESAR_GUI_UTILS_H_
#define CESAR_GUI_UTILS_H_

#include "gui.h"

namespace cesar::gui {

std::int8_t TryConvertToByte(std::string str, Base base, bool *is_number);
std::int16_t TryConvertToWord(std::string str, Base base, bool *is_number);

} // namespace cesar::gui

#endif // UTILS_H_
