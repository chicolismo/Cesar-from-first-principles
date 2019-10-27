#include "windows.h"

#include "images/config.xpm"
#include "panels.h"

#include <cstdlib>
#include <iostream>
#include <wx/bmpbuttn.h>
#include <wx/msgdlg.h>

#define IS_DISPLAY_ADDRESS(address) \
    (address) >= Cpu::BEGIN_DISPLAY_ADDRESS && (address) <= Cpu::END_DISPLAY_ADDRESS

template <typename SideWindow>
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

// ===========================================================================
// MainWindow
// ===========================================================================

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(ID_FileOpen, MainWindow::OnFileOpen)
    EVT_MENU(wxID_EXIT, MainWindow::OnExit)
    EVT_MOVE(MainWindow::OnMove)
wxEND_EVENT_TABLE()


MainWindow::MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size,
          wxDEFAULT_FRAME_STYLE & ~(wxMAXIMIZE_BOX | wxRESIZE_BORDER)) {

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

    text_display = new TextDisplay(this, &cpu);
    text_display->Show(true);

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
    grid->Add(register_panels[0], 0, wxALL, -2);
    grid->Add(register_panels[1], 0, wxALL, -2);
    grid->Add(register_panels[2], 0, wxALL, -2);
    grid->Add(register_panels[3], 0, wxALL, -2);
    grid->Add(register_panels[4], 0, wxALL, -2);
    grid->Add(register_panels[5], 0, wxALL, -2);
    grid->Add(register_panels[6], 0, wxALL, -2);
    grid->AddStretchSpacer();
    grid->Add(register_panels[7], 0, wxALL, -2);

    // Painéis de condições
    condition_panels[0] = new ConditionPanel(this, wxT("N"));
    condition_panels[1] = new ConditionPanel(this, wxT("Z"));
    condition_panels[2] = new ConditionPanel(this, wxT("V"));
    condition_panels[3] = new ConditionPanel(this, wxT("C"));
    auto condition_box = new wxBoxSizer(wxHORIZONTAL);
    for (auto &cb : condition_panels) {
        condition_box->Add(cb, 0, wxALL, -2);
    }

    // Painel de execuções
    execution_panel = new ExecutionPanel(this);
    // Painel de botões
    button_panel = new ButtonPanel(this);

    // Criando o menu
    auto *menu_file = new wxMenu;
    menu_file->Append(ID_FileOpen, "&Abrir...\tCtrl-A", "Abrir um arquivo");
    menu_file->Append(ID_FileOpen, "&Salvar\tCtrl-S", "Salva o arquivo atual");
    menu_file->Append(wxID_EXIT, "&Sair\tCtrl-x", "Termina o programa");

    auto *menu_bar = new wxMenuBar;
    menu_bar->Append(menu_file, "&Arquivo");
    SetMenuBar(menu_bar);

    auto *middle_right_sizer = new wxBoxSizer(wxVERTICAL);
    middle_right_sizer->Add(condition_box);
    middle_right_sizer->AddStretchSpacer();
    middle_right_sizer->Add(button_panel, 0, wxEXPAND);

    auto *middle_sizer = new wxBoxSizer(wxHORIZONTAL);
    middle_sizer->Add(execution_panel);
    middle_sizer->Add(middle_right_sizer, 0, wxEXPAND);

    auto *top_sizer = new wxBoxSizer(wxVERTICAL);
    top_sizer->Add(grid, 0, wxLEFT | wxTOP | wxRIGHT, 10);
    top_sizer->Add(middle_sizer, 0, wxLEFT | wxBOTTOM | wxRIGHT, 10);
    SetSizerAndFit(top_sizer);

    Center(wxBOTH);

    execution_panel->access_display->SetValue(4567u);
    execution_panel->instruction_display->SetValue(5467u);
}


void MainWindow::OnFileOpen(wxCommandEvent &WXUNUSED(event)) {
    // TODO: Testar se os dados do arquivo atual foram alterados e exibir diálogo apropriado.

    wxFileDialog dialog(this, _("Abrir arquivos .MEM do Cesar"), "", "",
        "Arquivos MEM (*.mem)|*mem", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (dialog.ShowModal() == wxID_CANCEL) {
        return; // Abertura de arquivo cancelada.
    }

    const std::string filename(dialog.GetPath().mb_str());

    // TODO: Enviar para o cpu ler os dados.
    if (cpu.read_memory_from_binary_file(filename)) {
        UpdatePanels();
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
    text_display->SetPosition(wxPoint(program_window->GetPosition().x, gap + pos.y + size.GetHeight()));
    program_window->SetSize(program_window->GetSize().GetWidth(), size.GetHeight());
    data_window->SetSize(data_window->GetSize().GetWidth(), size.GetHeight());
}


void MainWindow::UpdatePanels() {
    program_window->table->Refresh();
    data_window->table->Refresh();
    text_display->PaintNow();
}


void MainWindow::SetAddressValueAndUpdateTables(const long address, const Byte value) {
    const auto unsigned_address = static_cast<UWord>(address);
    cpu.memory[unsigned_address] = value;
    program_window->table->RefreshItem(address);
    data_window->table->RefreshItem(address);
    if (IS_DISPLAY_ADDRESS(unsigned_address)) {
        text_display->PaintNow();
    }
}


void MainWindow::SetBase(const Base new_base) {
    current_base = new_base;
    for (auto &panel : register_panels) {
        panel->SetBase(new_base);
    }
    program_window->SetBase(new_base);
    data_window->SetBase(new_base);
}

// ===========================================================================
// ProgramWindow
// ===========================================================================

wxBEGIN_EVENT_TABLE(ProgramWindow, wxDialog)
    EVT_CLOSE(ProgramWindow::OnClose)
    EVT_LIST_ITEM_SELECTED(wxID_ANY, ProgramWindow::OnItemSelected)
    EVT_TEXT_ENTER(ID_ValueInput, ProgramWindow::OnTextInputEnter)
wxEND_EVENT_TABLE()


ProgramWindow::ProgramWindow(wxWindow *parent, Cpu *cpu, const wxString &title)
    : wxDialog(
          parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxCAPTION | wxRESIZE_BORDER) {

    this->cpu = cpu;
    table = new ProgramTable(this, cpu);
    label = new wxStaticText(this, wxID_ANY, wxT("0"), wxDefaultPosition,
        wxSize(60, wxDefaultSize.GetHeight()), wxALIGN_RIGHT);
    label->Wrap(-1);
    input = new wxTextCtrl(this, ID_ValueInput, wxEmptyString, wxDefaultPosition,
        wxSize(60, wxDefaultSize.GetHeight()), wxTE_PROCESS_ENTER);

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


void ProgramWindow::SetBase(Base new_base) {
    current_base = new_base;
    table->current_base = new_base;
    table->Refresh();
    // TODO: Converter o valor do label;
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
    ::OnTextInputEnter<ProgramWindow>(this);
}

// ===========================================================================
// DataWindow
// ===========================================================================

wxBEGIN_EVENT_TABLE(DataWindow, wxDialog)
    EVT_CLOSE(DataWindow::OnClose)
    EVT_LIST_ITEM_SELECTED(wxID_ANY, DataWindow::OnItemSelected)
    EVT_TEXT_ENTER(ID_ValueInput, DataWindow::OnTextInputEnter)
wxEND_EVENT_TABLE()


DataWindow::DataWindow(wxWindow *parent, Cpu *cpu, const wxString &title)
    : wxDialog(
          parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxCAPTION | wxRESIZE_BORDER) {
    this->cpu = cpu;

    table = new DataTable(this, cpu);
    label = new wxStaticText(this, wxID_ANY, wxT("0"), wxDefaultPosition,
        wxSize(60, wxDefaultSize.GetHeight()), wxALIGN_RIGHT);
    label->Wrap(-1);
    input = new wxTextCtrl(this, ID_ValueInput, wxEmptyString, wxDefaultPosition,
        wxSize(60, wxDefaultSize.GetHeight()), wxTE_PROCESS_ENTER);

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


void DataWindow::SetBase(Base new_base) {
    current_base = new_base;
    table->current_base = new_base;
    table->Refresh();
    // TODO: Converter o valor do label;
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
    ::OnTextInputEnter<DataWindow>(this);
}
