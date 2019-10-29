#ifndef SIDE_WINDOWS_H
#define SIDE_WINDOWS_H

#include "../hardware.h"
#include "gui.h"
#include "tables.h"

#include <wx/dialog.h>

struct ProgramWindow : public wxDialog {
    Cpu *cpu;
    wxStaticText *label;
    wxTextCtrl *input;
    std::size_t current_value;
    Base current_base;
    ProgramTable *table;

    ProgramWindow(wxWindow *parent, Cpu *cpu, const wxString &title);

    void UpdateLabelAndInputValues();

    void OnTextInputEnter(wxCommandEvent &event);

    void OnClose(wxCloseEvent &event);

    void SetBase(Base new_base);

    void OnItemSelected(wxListEvent &event);

wxDECLARE_EVENT_TABLE();
};


struct DataWindow : public wxDialog {
    Cpu *cpu;
    wxStaticText *label;
    wxTextCtrl *input;
    std::size_t current_value;
    Base current_base;
    DataTable *table;

    DataWindow(wxWindow *parent, Cpu *cpu, const wxString &title);

    void UpdateLabelAndInputValues();

    void OnTextInputEnter(wxCommandEvent &event);

    void OnClose(wxCloseEvent &event);

    void SetBase(Base new_base);

    void OnItemSelected(wxListEvent &event);

wxDECLARE_EVENT_TABLE();
};

#endif // SIDE_WINDOWS_H
