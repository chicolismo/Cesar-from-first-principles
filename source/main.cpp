#include "gui/gui.h"
#include "gui/main_window.h"

#include <locale>

namespace cesar {

class Application : public wxApp {
public:
    virtual bool OnInit();
};

bool Application::OnInit() {
    std::locale::global(std::locale("pt_BR.utf8"));
    auto *window = new gui::MainWindow(wxT("Cesar"));
    window->Center(wxBOTH);
    window->Show(true);
    SetTopWindow(window);
    return true;
}

} // namespace cesar

wxIMPLEMENT_APP(cesar::Application);
