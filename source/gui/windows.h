#ifndef WINDOWS_H
#define WINDOWS_H

#include "gui.h"
#include "panels/panels.h"
#include "tables.h"
#include <wx/filedlg.h>
#include <wx/dialog.h>
#include <wx/textctrl.h>

struct MainWindow;
struct ProgramWindow;
struct DataWindow;

struct MainWindow : public wxFrame {
    Cpu cpu;

    ProgramWindow *program_window;
    DataWindow *data_window;

    RegisterPanel *register_panels[8];

    ConditionPanel *condition_panels[4];

    ExecutionPanel *execution_panel;

    Base current_base = Base::Decimal;

    MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size);

    void OnFileOpen(wxCommandEvent &event);

    void OnMove(wxMoveEvent &event);

    void OnExit(wxCommandEvent &event);

    void UpdateSubwindowsPositions();

    void SetAddressValueAndUpdateTables(long address, Byte value);

wxDECLARE_EVENT_TABLE();
};


struct ProgramWindow : public wxDialog {
    Cpu *cpu;
    ProgramTable *table;
    wxStaticText *label;
    wxTextCtrl *input;

    ProgramWindow(wxWindow *parent, Cpu *cpu, const wxString &title);

    void OnClose(wxCloseEvent &event);

    void OnItemSelected(wxListEvent &event);

    void OnTextInputEnter(wxCommandEvent &event);

wxDECLARE_EVENT_TABLE();
};

struct DataWindow : public wxDialog {
    Cpu *cpu;
    DataTable *table;
    wxStaticText *label;
    wxTextCtrl *input;

    DataWindow(wxWindow *parent, Cpu *cpu, const wxString &title);

    void OnClose(wxCloseEvent &event);

    void OnItemSelected(wxListEvent &event);

    void OnTextInputEnter(wxCommandEvent &event);

wxDECLARE_EVENT_TABLE();
};

#endif
