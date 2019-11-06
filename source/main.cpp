#include "gui/gui.h"
#include "gui/main_window.h"
#include "hardware.h"

class Cesar : public wxApp {
public:
    virtual bool OnInit();
};

bool Cesar::OnInit() {
    MainWindow *window = new MainWindow(wxT("Cesar"), wxDefaultPosition, wxSize(640, 480));
    window->Center(wxBOTH);
    window->Show(true);
    SetTopWindow(window);
    return true;
}

wxIMPLEMENT_APP(Cesar);
