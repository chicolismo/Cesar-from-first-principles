#ifndef GUI_H
#define GUI_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "../hardware.h"

#include <variant>
#include <wx/dialog.h>
#include <wx/event.h>
#include <wx/filedlg.h>
#include <wx/listctrl.h>
#include <wx/minifram.h>

enum {
    ID_FileOpen = 1,
    ID_FileSave,
};

class MainWindow;
class ProgramWindow;
//class DataWindow;
class Table;
class ProgramTable;
class DataTable;

struct MainWindow : public wxFrame {
    Cpu cpu;

    ProgramWindow *program_window;
    //DataWindow *data_window;

    MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size);

    void OnFileOpen(wxCommandEvent &event);
    void OnMove(wxMoveEvent &event);
    void OnExit(wxCommandEvent &event);
    void UpdateSubwindowsPositions();

    wxDECLARE_EVENT_TABLE();
};

struct ProgramWindow : public wxDialog {
    Cpu *cpu;
    ProgramTable *table;
    wxStaticText *label;
    wxTextCtrl *input;

    ProgramWindow(
        wxWindow *parent, Cpu *cpu, const wxString &title, const wxPoint &pos, const wxSize &size);

    void OnClose(wxCloseEvent &event);

    wxDECLARE_EVENT_TABLE();
};

//struct DataWindow : public wxDialog {
    //Cpu *cpu;
    //DataTable *table;
    //wxStaticText *label;
    //wxTextCtrl *input;

    //DataWindow(
        //wxWindow *parent, Cpu *cpu, const wxString &title, const wxPoint &pos, const wxSize &size);

    //void OnClose(wxCloseEvent &event);

    //wxDECLARE_EVENT_TABLE();
//};

struct ProgramTable : public wxListCtrl {
    Cpu *cpu;
    Byte *data;
    ProgramTable(wxWindow *parent, Cpu *cpu);
    wxString OnGetItemText(long item, long column) const;
};

//struct DataTable : public wxListCtrl {
    //Cpu *cpu;
    //Byte *data;
    //DataTable(wxWindow *parent, Cpu *cpu);
    //wxString OnGetItemText(long item, long column) const;
//};

#endif // GUI_H
