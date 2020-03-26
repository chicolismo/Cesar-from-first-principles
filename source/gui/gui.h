#ifndef CESAR_GUI_GUI_H
#define CESAR_GUI_GUI_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#else
#include <wx/wxprec.h>
#endif

#include "../hardware/hardware.h"

#include <wx/event.h>

namespace cesar::gui {
using Cpu = cesar::hardware::Cpu;

enum Base { Decimal, Hexadecimal };

enum {
  ID_FileOpen = 1,
  ID_FileSave,
  ID_ValueInput,
  ID_R0,
  ID_R1,
  ID_R2,
  ID_R3,
  ID_R4,
  ID_R5,
  ID_R6,
  ID_R7,
  ID_Decimal,
  ID_Hexadecimal,
  ID_Run,
  ID_Next,
};

} // namespace cesar::gui

#endif // GUI_H
