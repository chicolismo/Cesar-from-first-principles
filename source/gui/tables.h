#ifndef CESAR_GUI_TABLES_H
#define CESAR_GUI_TABLES_H

#include "gui.h"
#include <wx/listctrl.h>

namespace cesar::gui {

struct ProgramTable;
struct DataTable;

struct ProgramTable : public wxListCtrl {
    Cpu *cpu;
    std::int8_t *data;
    uint16_t program_counter;
    Base current_base;

    ProgramTable(wxWindow *parent, Cpu *cpu);
    wxString OnGetItemText(long item, long column) const;
    void SetProgramCounter(std::int16_t pc);
};

struct DataTable : public wxListCtrl {
    Cpu *cpu;
    std::int8_t *data;
    Base current_base;

    DataTable(wxWindow *parent, Cpu *cpu);
    wxString OnGetItemText(long item, long column) const;
};

} // namespace cesar::gui

#endif
