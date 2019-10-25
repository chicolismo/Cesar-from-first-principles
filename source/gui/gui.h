#ifndef GUI_H
#define GUI_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "../hardware.h"

#include <wx/dialog.h>
#include <wx/event.h>
#include <wx/filedlg.h>
#include <wx/listctrl.h>
#include <wx/textctrl.h>

enum Base { Decimal, Hexadecimal };

const int BASE_VALUE[] = {
    [Decimal] = 10,
    [Hexadecimal] = 16
};

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
};
#endif // GUI_H
