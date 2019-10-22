#ifndef GUI_H
#define GUI_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/dialog.h>
#include <wx/event.h>
#include <wx/filedlg.h>

enum {
    ID_FileOpen = 1
};

class MainWindow;
class SideWindow;

class MainWindow : public wxFrame {
    SideWindow *program_window, *data_window;

public:
    MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size);

    void OnFileOpen(wxCommandEvent &event);
    void OnMove(wxMoveEvent &event);
    void UpdateSubwindowsPositions();

    wxDECLARE_EVENT_TABLE();
};

class SideWindow : public wxDialog {
public:
    SideWindow(wxWindow *parent, const wxString &title, const wxPoint &pos, const wxSize &size);
};

#endif // GUI_H
