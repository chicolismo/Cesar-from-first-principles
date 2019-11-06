#include "main_window.h"

#include "panels.h"

#include <wx/msgdlg.h>

#define IS_DISPLAY_ADDRESS(address) \
    (address) >= Cpu::BEGIN_DISPLAY_ADDRESS && (address) <= Cpu::END_DISPLAY_ADDRESS


// ===========================================================================
// MainWindow
// ===========================================================================
wxDEFINE_EVENT(wxEVT_THREAD_UPDATE, wxThreadEvent);

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(ID_FileOpen, MainWindow::OnFileOpen)
    EVT_MENU(wxID_EXIT, MainWindow::OnExit)
    EVT_CLOSE(MainWindow::OnClose)
    EVT_MOVE(MainWindow::OnMove)
    EVT_TOGGLEBUTTON(ID_Run, MainWindow::OnRunButtonToggle)
    EVT_BUTTON(ID_Next, MainWindow::OnNextButtonClick)
    EVT_ACTIVATE(MainWindow::OnActivate)
wxEND_EVENT_TABLE()

static wxMutex *mutex = new wxMutex();
static wxSemaphore *s = new wxSemaphore(1, 1);

MainWindow::MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size,
          wxDEFAULT_FRAME_STYLE & ~(wxMAXIMIZE_BOX | wxRESIZE_BORDER)) {

    should_raise_windows = true;
    thread_should_run = false;

    // this->Bind(wxEVT_THREAD_UPDATE, &MainWindow::OnThreadUpdate, this);
    this->Bind(wxEVT_THREAD, &MainWindow::OnThreadUpdate, this);

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
    register_panels[0] = new RegisterPanel(this, 0, wxT("R0:"));
    register_panels[1] = new RegisterPanel(this, 1, wxT("R1:"));
    register_panels[2] = new RegisterPanel(this, 2, wxT("R2:"));
    register_panels[3] = new RegisterPanel(this, 3, wxT("R3:"));
    register_panels[4] = new RegisterPanel(this, 4, wxT("R4:"));
    register_panels[5] = new RegisterPanel(this, 5, wxT("R5:"));
    register_panels[6] = new RegisterPanel(this, 6, wxT("R6: (SP)"));
    register_panels[7] = new RegisterPanel(this, 7, wxT("R7: (PC)"));

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
}


void MainWindow::UpdateSubwindowsPositions() {
    const unsigned gap = 10;
    const wxSize size = GetSize();
    const wxPoint pos = GetPosition();
    const wxSize pwsize = program_window->GetSize();
    program_window->SetPosition(wxPoint(pos.x - pwsize.GetWidth() - gap, pos.y));
    data_window->SetPosition(wxPoint(pos.x + size.GetWidth() + gap, pos.y));
    text_display->SetPosition(
        wxPoint(program_window->GetPosition().x, gap + pos.y + size.GetHeight()));
    program_window->SetSize(program_window->GetSize().GetWidth(), size.GetHeight());
    data_window->SetSize(data_window->GetSize().GetWidth(), size.GetHeight());
}


void MainWindow::UpdatePanels() {
    for (std::size_t i = 0; i < 8; ++i) {
        register_panels[i]->SetValue(cpu.registers[i]);
    }
    condition_panels[0]->led_display->SetTurnedOn(cpu.condition.negative == 1);
    condition_panels[1]->led_display->SetTurnedOn(cpu.condition.zero == 1);
    condition_panels[2]->led_display->SetTurnedOn(cpu.condition.overflow == 1);
    condition_panels[3]->led_display->SetTurnedOn(cpu.condition.carry == 1);
}


void MainWindow::SetAddressValueAndUpdateTables(const long address, const std::int8_t value) {
    const auto unsigned_address = static_cast<std::uint16_t>(address);
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


void MainWindow::OnClose(wxCloseEvent &WXUNUSED(event)) {
    while (GetThread() && GetThread()->IsRunning()) {
        GetThread()->Pause();
        GetThread()->Delete();
    }
    Destroy(); // Destroi a janela principal
}


void MainWindow::OnRunButtonToggle(wxCommandEvent &event) {
    auto *button = static_cast<wxBitmapToggleButton *>(event.GetEventObject());
    if (button->GetValue() && !thread_should_run) {
        thread_should_run = true;
        StartThread();
    }
    else {
        thread_should_run = false;
    }
}


void MainWindow::OnNextButtonClick(wxCommandEvent &WXUNUSED(event)) {
    // wxCriticalSectionLocker lock(cpu_cs);
    RunNextInstruction();
    UpdatePanels();
}


void MainWindow::RunNextInstruction() {
    cpu.execute_next_instruction();
}


void MainWindow::OnRegisterPanelDoubleClick([[maybe_unused]] int register_number) {
    // TODO
    // Exibir diálogo para ler o novo valor do registrador.
    // Apenas valores no formato correto da base serão aceitos.
}


void MainWindow::OnActivate(wxActivateEvent &event) {
    if (event.GetActive()) {
        if (should_raise_windows) {
            should_raise_windows = false;
            program_window->Raise();
            data_window->Raise();
            text_display->Raise();
            Raise();
        }
        else {
            should_raise_windows = true;
        }
    }
    event.Skip();
}


void MainWindow::OnThreadUpdate(wxThreadEvent &WXUNUSED(event)) {
    RefreshPanels();
    s->Post();
}

void MainWindow::StartThread() {
    if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR) {
        wxLogError("Não foi possível criar a thread");
        return;
    }

    if (GetThread()->Run() != wxTHREAD_NO_ERROR) {
        wxLogError("Não foi possível executar a thread");
        return;
    }
}

void MainWindow::RefreshPanels() {
    program_window->table->Refresh();
    data_window->table->Refresh();
    for (std::size_t i = 0; i < 8; ++i) {
        register_panels[i]->digital_display->Refresh();
        register_panels[i]->binary_display->Refresh();
    }
    condition_panels[0]->led_display->Refresh();
    condition_panels[1]->led_display->Refresh();
    condition_panels[2]->led_display->Refresh();
    condition_panels[3]->led_display->Refresh();
}

wxThread::ExitCode MainWindow::Entry() {
    while (!GetThread()->TestDestroy()) {
        s->Wait();
        wxMutexGuiEnter();
        RunNextInstruction();
        UpdatePanels();
        wxMutexGuiLeave();
        wxQueueEvent(GetEventHandler(), new wxThreadEvent());
        if (!thread_should_run) {
            break;
        }
    }
    return static_cast<wxThread::ExitCode>(0);
}
