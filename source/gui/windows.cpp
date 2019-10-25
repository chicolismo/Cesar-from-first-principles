#include "windows.h"
#include "panels/panels.h"

#include "images/config.xpm"

#include <iostream>
#include <wx/msgdlg.h>
#include <wx/bmpbuttn.h>


template<typename SideWindow>
void OnTextInputEnter(SideWindow *side_window) {
    MainWindow *window = (MainWindow *) (side_window->GetParent());
    wxString label_text = side_window->label->GetLabelText();
    wxString input_text = side_window->input->GetValue();
    long address = 0L;
    long value = 0L;
    if (input_text.IsNumber()) {
        label_text.ToLong(&address, window->current_base);
        input_text.ToLong(&value, window->current_base);
        Byte byte = static_cast<Byte>(value);
        window->SetAddressValueAndUpdateTables(address, byte);
    }
}


//----------------------------------------------------------------------------//
// Main Window                                                                //
//----------------------------------------------------------------------------//

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
        EVT_MENU(ID_FileOpen, MainWindow::OnFileOpen)
        EVT_MENU(wxID_EXIT, MainWindow::OnExit)
        EVT_MOVE(MainWindow::OnMove)
wxEND_EVENT_TABLE()


MainWindow::MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {

    // Inicializando as janelas laterais
    program_window = new ProgramWindow(this, &cpu, wxT("Programa"));
    const wxSize psize = program_window->GetSize();
    program_window->SetMaxSize(wxSize(psize.GetWidth(), wxDefaultCoord));
    program_window->SetMinSize(wxSize(psize.GetWidth(), 300));

    data_window = new DataWindow(this, &cpu, wxT("Dados"));
    const wxSize dsize = data_window->GetSize();
    data_window->SetMaxSize(wxSize(dsize.GetWidth(), wxDefaultCoord));
    data_window->SetMinSize(wxSize(dsize.GetWidth(), 300));

    program_window->Show(true);
    data_window->Show(true);

    // Displays de registradores
    register_panels[0] = new RegisterPanel(this, ID_R0, wxT("R0:"));
    register_panels[1] = new RegisterPanel(this, ID_R1, wxT("R1:"));
    register_panels[2] = new RegisterPanel(this, ID_R2, wxT("R2:"));
    register_panels[3] = new RegisterPanel(this, ID_R3, wxT("R3:"));
    register_panels[4] = new RegisterPanel(this, ID_R4, wxT("R4:"));
    register_panels[5] = new RegisterPanel(this, ID_R5, wxT("R5:"));
    register_panels[6] = new RegisterPanel(this, ID_R6, wxT("R6: (SP)"));
    register_panels[7] = new RegisterPanel(this, ID_R7, wxT("R7: (PC)"));

    auto *grid = new wxGridSizer(3, 3, 0, 0);
    grid->Add(register_panels[0], 1, wxEXPAND | wxALL, -2);
    grid->Add(register_panels[1], 1, wxEXPAND | wxALL, -2);
    grid->Add(register_panels[2], 1, wxEXPAND | wxALL, -2);
    grid->Add(register_panels[3], 1, wxEXPAND | wxALL, -2);
    grid->Add(register_panels[4], 1, wxEXPAND | wxALL, -2);
    grid->Add(register_panels[5], 1, wxEXPAND | wxALL, -2);
    grid->Add(register_panels[6], 1, wxEXPAND | wxALL, -2);
    grid->AddStretchSpacer();
    grid->Add(register_panels[7], 1, wxEXPAND | wxALL, -2);

    // Painéis de condições
    condition_panels[0] = new ConditionPanel(this, wxT("N"));
    condition_panels[1] = new ConditionPanel(this, wxT("Z"));
    condition_panels[2] = new ConditionPanel(this, wxT("V"));
    condition_panels[3] = new ConditionPanel(this, wxT("C"));
    auto condition_box = new wxBoxSizer(wxHORIZONTAL);
    for (auto &cb : condition_panels) {
        condition_box->Add(cb, 0, wxALL, -2);
    }

    // Criando o menu
    auto *menu_file = new wxMenu;
    menu_file->Append(ID_FileOpen, "&Abrir...\tCtrl-A", "Abrir um arquivo");
    menu_file->Append(ID_FileOpen, "&Salvar\tCtrl-S", "Salva o arquivo atual");
    menu_file->Append(wxID_EXIT, "&Sair\tCtrl-x", "Termina o programa");

    auto *menu_bar = new wxMenuBar;
    menu_bar->Append(menu_file, "&Arquivo");
    SetMenuBar(menu_bar);

    execution_panel = new ExecutionPanel(this);

    auto *lower_sizer = new wxBoxSizer(wxHORIZONTAL);
    lower_sizer->Add(execution_panel);
    lower_sizer->Add(condition_box);

    auto *big_box = new wxBoxSizer(wxVERTICAL);
    big_box->Add(grid, 1, wxEXPAND);
    big_box->Add(lower_sizer, 0);
    big_box->Fit(this);
    SetSizer(big_box);
    Layout();
    big_box->Fit(this);
    Center(wxBOTH);

    execution_panel->access_display->SetValue(4567u);
    execution_panel->instruction_display->SetValue(5467u);
}


void MainWindow::OnFileOpen(wxCommandEvent &WXUNUSED(event)) {
    // TODO: Testar se os dados do arquivo atual foram alterados e exibir diálogo apropriado.

    wxFileDialog dialog(
        this, _("Abrir arquivos .MEM do Cesar"), "", "", "Arquivos MEM (*.mem)|*mem", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (dialog.ShowModal() == wxID_CANCEL) {
        return; // Abertura de arquivo cancelada.
    }

    const std::string filename(dialog.GetPath().mb_str());

    // TODO: Enviar para o cpu ler os dados.
    if (cpu.read_memory_from_binary_file(filename)) {
        program_window->table->RefreshItems(0, MEM_SIZE - 1);
        data_window->table->RefreshItems(0, MEM_SIZE - 1);
    }
}


void MainWindow::OnMove(wxMoveEvent &WXUNUSED(event)) {
    UpdateSubwindowsPositions();
}


void MainWindow::OnExit(wxCommandEvent &WXUNUSED(event)) {
    Close(true);
}


void MainWindow::UpdateSubwindowsPositions() {
    const unsigned gap = 10;
    const wxSize size = GetSize();
    const wxPoint pos = GetPosition();
    const wxSize pwsize = program_window->GetSize();
    program_window->SetPosition(wxPoint(pos.x - pwsize.GetWidth() - gap, pos.y));
    data_window->SetPosition(wxPoint(pos.x + size.GetWidth() + gap, pos.y));
}


void MainWindow::SetAddressValueAndUpdateTables(long address, Byte value) {
    const auto uaddress = static_cast<std::size_t>(address);
    cpu.memory[uaddress] = value;
    program_window->table->RefreshItem(address);
    data_window->table->RefreshItem(address);
}


//----------------------------------------------------------------------------//
// Program Window                                                             //
//----------------------------------------------------------------------------//

wxBEGIN_EVENT_TABLE(ProgramWindow, wxDialog)
        EVT_CLOSE(ProgramWindow::OnClose)
        EVT_LIST_ITEM_SELECTED(wxID_ANY, ProgramWindow::OnItemSelected)
        EVT_TEXT_ENTER(ID_ValueInput, ProgramWindow::OnTextInputEnter)
wxEND_EVENT_TABLE()


ProgramWindow::ProgramWindow(wxWindow *parent, Cpu *cpu, const wxString &title)
    : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxCAPTION | wxRESIZE_BORDER) {

    this->cpu = cpu;
    table = new ProgramTable(this, cpu);
    label = new wxStaticText(
        this, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize(60, wxDefaultSize.GetHeight()), wxALIGN_RIGHT);
    label->Wrap(-1);
    input =
        new wxTextCtrl(this, ID_ValueInput, wxEmptyString, wxDefaultPosition, wxSize(60, wxDefaultSize.GetHeight()),
                       wxTE_PROCESS_ENTER);

    auto *vbox = new wxBoxSizer(wxVERTICAL);
    auto *hbox = new wxBoxSizer(wxHORIZONTAL);

    hbox->Add(label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 10);
    hbox->Add(input, 0, wxALL, 0);

    vbox->Add(table, 1, wxEXPAND | wxALL, 4);
    vbox->Add(hbox, 0, wxALIGN_RIGHT | wxALL, 4);

    SetSizer(vbox);
    Layout();
    Fit();
    vbox->Fit(this);
}


void ProgramWindow::OnClose(wxCloseEvent &event) {
    event.StopPropagation(); // Não pode fechar as janelas laterais.
}


void ProgramWindow::OnItemSelected(wxListEvent &event) {
    auto row = static_cast<std::size_t>(event.GetIndex());
    label->SetLabel(wxString::Format(wxT("%ld"), row));
    input->SetValue(wxString::Format(wxT("%d"), cpu->memory[row]));
    input->SetFocus();
    input->SetSelection(-1, -1);
}


void ProgramWindow::OnTextInputEnter(wxCommandEvent &WXUNUSED(event)) {
    ::OnTextInputEnter < ProgramWindow > (this);
}


//----------------------------------------------------------------------------//
// Data Window                                                                //
//----------------------------------------------------------------------------//

wxBEGIN_EVENT_TABLE(DataWindow, wxDialog)
        EVT_CLOSE(DataWindow::OnClose)
        EVT_LIST_ITEM_SELECTED(wxID_ANY, DataWindow::OnItemSelected)
        EVT_TEXT_ENTER(ID_ValueInput, DataWindow::OnTextInputEnter)
wxEND_EVENT_TABLE()


DataWindow::DataWindow(wxWindow *parent, Cpu *cpu, const wxString &title)
    : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxCAPTION | wxRESIZE_BORDER) {
    this->cpu = cpu;

    table = new DataTable(this, cpu);
    label = new wxStaticText(
        this, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize(60, wxDefaultSize.GetHeight()), wxALIGN_RIGHT);
    label->Wrap(-1);
    input =
        new wxTextCtrl(this, ID_ValueInput, wxEmptyString, wxDefaultPosition, wxSize(60, wxDefaultSize.GetHeight()),
                       wxTE_PROCESS_ENTER);

    auto *vbox = new wxBoxSizer(wxVERTICAL);
    auto *hbox = new wxBoxSizer(wxHORIZONTAL);

    hbox->Add(label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL | wxTEXT_ALIGNMENT_RIGHT, 10);
    hbox->Add(input, 0, wxALL, 0);

    vbox->Add(table, 1, wxEXPAND | wxALL, 4);
    vbox->Add(hbox, 0, wxALIGN_RIGHT | wxALL, 4);

    SetSizer(vbox);
    Layout();
    Fit();
    vbox->Fit(this);
}


void DataWindow::OnClose(wxCloseEvent &event) {
    event.StopPropagation(); // Não pode fechar as janelas laterais.
}


void DataWindow::OnItemSelected(wxListEvent &event) {
    auto row = static_cast<std::size_t>(event.GetIndex());
    label->SetLabel(wxString::Format(wxT("%ld"), row));
    input->SetValue(wxString::Format(wxT("%d"), cpu->memory[row]));
    input->SetFocus();
    input->SetSelection(-1, -1);
}


void DataWindow::OnTextInputEnter(wxCommandEvent &WXUNUSED(event)) {
    ::OnTextInputEnter < DataWindow > (this);
}
