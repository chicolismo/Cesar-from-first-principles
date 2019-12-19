#include "gui/gui.h"
#include "gui/main_window.h"

namespace cesar {

class Application : public wxApp {
public:
    virtual bool OnInit();
};

bool Application::OnInit() {
    setlocale(LC_ALL, "pt_BR");

    auto *window =
        new gui::MainWindow(wxT("Cesar"));
    window->Center(wxBOTH);
    window->Show(true);
    SetTopWindow(window);
    return true;
}

} // namespace cesar

wxIMPLEMENT_APP(cesar::Application);
