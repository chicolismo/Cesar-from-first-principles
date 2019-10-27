#ifndef WINDOWS_H
#define WINDOWS_H

#include "gui.h"
#include "panels.h"
#include "tables.h"
#include "text_display.h"
#include <wx/filedlg.h>
#include <wx/dialog.h>
#include <wx/textctrl.h>

struct MainWindow;
struct ProgramWindow;
struct DataWindow;
struct TextDisplay;

struct MainWindow : public wxFrame {
    Cpu cpu;

    ProgramWindow *program_window;
    DataWindow *data_window;

    RegisterPanel *register_panels[8];

    ConditionPanel *condition_panels[4];

    ExecutionPanel *execution_panel;

    ButtonPanel *button_panel;

    TextDisplay *text_display;

    Base current_base = Base::Decimal;

    MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size);

    void OnFileOpen(wxCommandEvent &event);

    void OnMove(wxMoveEvent &event);

    void OnExit(wxCommandEvent &event);

    void OnMaximize(wxMaximizeEvent &event);

    void OnResize(wxSizeEvent &event);

    void UpdateSubwindowsPositions();

    void UpdatePanels();

    void SetAddressValueAndUpdateTables(long address, Byte value);

    void SetBase(Base new_base);
wxDECLARE_EVENT_TABLE();
};


struct ProgramWindow : public wxDialog {
    Cpu *cpu;
    ProgramTable *table;
    wxStaticText *label;
    wxTextCtrl *input;
    Base current_base;

    ProgramWindow(wxWindow *parent, Cpu *cpu, const wxString &title);

    void SetBase(Base base);

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
    Base current_base;

    DataWindow(wxWindow *parent, Cpu *cpu, const wxString &title);

    void SetBase(Base base);

    void OnClose(wxCloseEvent &event);

    void OnItemSelected(wxListEvent &event);

    void OnTextInputEnter(wxCommandEvent &event);

wxDECLARE_EVENT_TABLE();
};

#endif
