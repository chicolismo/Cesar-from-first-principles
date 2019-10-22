#include "gui.h"

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(ID_FileOpen, MainWindow::OnFileOpen)
    EVT_MOVE(MainWindow::OnMove)
wxEND_EVENT_TABLE()

MainWindow::MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {

    // Inicializando as janelas laterais
    program_window = new SideWindow(this, _("Programa"), wxDefaultPosition, wxSize(400, 600));
    data_window = new SideWindow(this, _("Dados"), wxDefaultPosition, wxSize(300, 600));

    program_window->Show(true);
    data_window->Show(true);


    // Criando o menu
    wxMenu *menu_file = new wxMenu;
    menu_file->Append(ID_FileOpen, "&Abrir...\tCtrl-A", "Abrir um arquivo");

    wxMenuBar *menu_bar = new wxMenuBar;
    menu_bar->Append(menu_file, "&Arquivo");
    SetMenuBar(menu_bar);
}


void MainWindow::OnFileOpen(wxCommandEvent &WXUNUSED(event)) {
    // TODO: Testar se os dados do arquivo atual foram alterados e exibir
    // diálogo apropriado.

    wxFileDialog dialog(this, _("Abrir arquivos .MEM do Cesar"), "", "",
        "Arquivos MEM (*.mem)|*mem", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (dialog.ShowModal() == wxID_CANCEL) {
        return; // Abertura de arquivo cancelada.
    }

    const std::string filename(dialog.GetPath().mb_str());

    // TODO: Enviar para o cpu ler os dados.
}


void MainWindow::OnMove(wxMoveEvent &WXUNUSED(event)) { UpdateSubwindowsPositions(); }


void MainWindow::UpdateSubwindowsPositions() {
    const unsigned gap = 10;
    const wxSize size = GetSize();
    const wxPoint pos = GetPosition();
    const wxSize pwsize = program_window->GetSize();
    program_window->SetPosition(wxPoint(pos.x - pwsize.GetWidth() - gap, pos.y));
    data_window->SetPosition(wxPoint(pos.x + size.GetWidth() + gap, pos.y));
}
