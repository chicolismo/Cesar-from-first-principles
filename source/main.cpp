#include "gui/gui.h"
#include "hardware.h"
#include <cstdlib>
#include <fstream>
#include <iostream>

class Cesar : public wxApp {
public:
    virtual bool OnInit();
};

bool Cesar::OnInit() {
    MainWindow *window = new MainWindow(_("Cesar"), wxDefaultPosition, wxSize(640, 480));
    window->Show(true);
    SetTopWindow(window);
    return true;
}

wxIMPLEMENT_APP(Cesar);
