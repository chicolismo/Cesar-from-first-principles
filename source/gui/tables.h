#ifndef TABLES_H
#define TABLES_H
#include "../hardware.h"
#include <wx/listctrl.h>

struct ProgramTable;
struct DataTable;

struct ProgramTable : public wxListCtrl {
    Cpu *cpu;
    Byte *data;
    uint16_t program_counter;

    ProgramTable(wxWindow *parent, Cpu *cpu);
    wxString OnGetItemText(long item, long column) const;
    void SetProgramCounter(Word pc);
};

struct DataTable : public wxListCtrl {
    Cpu *cpu;
    Byte *data;

    DataTable(wxWindow *parent, Cpu *cpu);
    wxString OnGetItemText(long item, long column) const;
};

#endif//TABLES_H
