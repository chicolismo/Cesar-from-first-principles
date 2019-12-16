#include "main_window.h"
#include "panels.h"
#include "utils.h"

#include <chrono>
#include <cstdint>
#include <mutex>
#include <thread>
#include <wx/event.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>

namespace cesar::gui {

#define IS_DISPLAY_ADDRESS(address)                                            \
    (address) >= Cpu::BEGIN_DISPLAY_ADDRESS &&                                 \
        (address) <= Cpu::END_DISPLAY_ADDRESS

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

MainWindow::MainWindow(
    const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size,
          wxDEFAULT_FRAME_STYLE & ~(wxMAXIMIZE_BOX | wxRESIZE_BORDER)),
      semaphore(1, 1), should_raise_windows(true), thread_is_running(false) {

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

    auto *register_grid = new wxFlexGridSizer(3, 3, 4, 4);
    register_grid->Add(register_panels[0], 1, wxEXPAND);
    register_grid->Add(register_panels[1], 1, wxEXPAND);
    register_grid->Add(register_panels[2], 1, wxEXPAND);
    register_grid->Add(register_panels[3], 1, wxEXPAND);
    register_grid->Add(register_panels[4], 1, wxEXPAND);
    register_grid->Add(register_panels[5], 1, wxEXPAND);
    register_grid->Add(register_panels[6], 1, wxEXPAND);
    register_grid->AddStretchSpacer();
    register_grid->Add(register_panels[7], 1, wxEXPAND);

    // Painéis de condições
    condition_panels[0] = new ConditionPanel(this, wxT("N"));
    condition_panels[1] = new ConditionPanel(this, wxT("Z"));
    condition_panels[2] = new ConditionPanel(this, wxT("V"));
    condition_panels[3] = new ConditionPanel(this, wxT("C"));
    auto condition_box = new wxBoxSizer(wxHORIZONTAL);
    condition_box->Add(condition_panels[0]);
    condition_box->AddStretchSpacer();
    condition_box->Add(condition_panels[1]);
    condition_box->AddStretchSpacer();
    condition_box->Add(condition_panels[2]);
    condition_box->AddStretchSpacer();
    condition_box->Add(condition_panels[3]);

    // Painel de execuções
    execution_panel = new ExecutionPanel(this);
    // Painel de botões
    button_panel = new ButtonPanel(this);

    auto *middle_right_sizer = new wxBoxSizer(wxVERTICAL);
    middle_right_sizer->Add(condition_box, 0, wxEXPAND);
    middle_right_sizer->AddStretchSpacer();
    middle_right_sizer->Add(button_panel, 0, wxEXPAND);

    auto *middle_sizer = new wxBoxSizer(wxHORIZONTAL);
    middle_sizer->Add(execution_panel);
    middle_sizer->Add(middle_right_sizer);

    auto *main_sizer = new wxBoxSizer(wxVERTICAL);

    main_sizer->Add(register_grid, 1, wxALL | wxEXPAND, 10);
    main_sizer->Add(
        middle_sizer, 1, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 10);

    SetSizerAndFit(main_sizer);

    // Criando o menu
    auto *menu_file = new wxMenu;
    menu_file->Append(ID_FileOpen, "&Abrir...\tCtrl-A", "Abrir um arquivo");
    menu_file->Append(ID_FileOpen, "&Salvar\tCtrl-S", "Salva o arquivo atual");
    menu_file->Append(wxID_EXIT, "&Sair\tCtrl-x", "Termina o programa");

    auto *menu_bar = new wxMenuBar;
    menu_bar->Append(menu_file, "&Arquivo");
    SetMenuBar(menu_bar);

    Center(wxBOTH);
}

void MainWindow::UpdateSubwindowsPositions() {
    const unsigned gap = 10;
    const wxSize size = GetSize();
    const wxPoint pos = GetPosition();
    const wxSize pwsize = program_window->GetSize();

    program_window->SetPosition(
        wxPoint(pos.x - pwsize.GetWidth() - gap, pos.y));

    data_window->SetPosition(wxPoint(pos.x + size.GetWidth() + gap, pos.y));

    text_display->SetPosition(wxPoint(
        program_window->GetPosition().x, gap + pos.y + size.GetHeight()));

    program_window->SetSize(
        program_window->GetSize().GetWidth(), size.GetHeight());

    data_window->SetSize(data_window->GetSize().GetWidth(), size.GetHeight());

    // text_display->SetSize(text_display->GetClientSize());
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

void MainWindow::SetAddressValueAndUpdateTables(
    const long address, const std::int8_t value) {
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
    // TODO: Testar se os dados do arquivo atual foram alterados e exibir
    // diálogo apropriado.

    wxFileDialog dialog(this, _("Abrir arquivos .MEM do Cesar"), "", "",
        "Arquivos MEM (*.mem)|*mem", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (dialog.ShowModal() == wxID_CANCEL) {
        return; // Abertura de arquivo cancelada.
    }

    const std::string filename(dialog.GetPath().mb_str(wxConvUTF8));

    // TODO: Enviar para o cpu ler os dados.
    if (cpu.read_memory_from_binary_file(filename)) {
        UpdatePanels();
    }
}

void MainWindow::OnMove(wxMoveEvent &WXUNUSED(event)) {
    UpdateSubwindowsPositions();
}

void MainWindow::OnExit(wxCommandEvent &WXUNUSED(event)) { Close(true); }

void MainWindow::OnClose(wxCloseEvent &WXUNUSED(event)) {
    while (GetThread() && GetThread()->IsRunning()) {
        GetThread()->Pause();
        GetThread()->Delete();
    }
    Destroy(); // Destroi a janela principal
}

void MainWindow::OnRunButtonToggle(wxCommandEvent &event) {
    auto *toggle_button =
        static_cast<wxBitmapToggleButton *>(event.GetEventObject());
    if (toggle_button->GetValue() && !thread_is_running) {
        cpu.halted = false;
        thread_is_running = true;
        StartThread();
    }
    else {
        thread_is_running = false;
    }
}

void MainWindow::OnNextButtonClick(wxCommandEvent &WXUNUSED(event)) {
    if (!thread_is_running) {
        RunNextInstruction();
        UpdatePanels();
    }
}

void MainWindow::RunNextInstruction() { cpu.execute_next_instruction(); }

void MainWindow::OnRegisterPanelDoubleClick(int register_number) {
    RegisterPanel *panel = register_panels[register_number];
    std::uint16_t value = panel->current_value;
    wxString current_value;
    if (current_base == Base::Decimal) {
        current_value.Printf("%d", value);
    }
    else {
        current_value.Printf("%x", value);
    }
    wxString message;
    message.Printf("Digite novo valor para o registrador %d", register_number);
    wxTextEntryDialog dialog(this, message, wxT("Novo valor"), current_value,
        wxTextEntryDialogStyle, panel->GetPosition());
    if (dialog.ShowModal() == wxID_OK) {
        bool is_valid_number;
        std::string input(dialog.GetValue());
        std::int16_t value =
            TryConvertToWord(input, current_base, &is_valid_number);
        if (is_valid_number) {
            cpu.registers[register_number] = value;
            panel->SetValue(static_cast<std::uint16_t>(value));
            panel->Refresh();
        }
    }
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

void MainWindow::OnThreadUpdate(wxThreadEvent &WXUNUSED(event)) {
    // Atualiza a interface e libera o semáforo.
    UpdatePanels();
    RefreshPanels();
    semaphore.Post();
}

wxThread::ExitCode MainWindow::Entry() {
    auto start = std::chrono::system_clock::now();

    while (!GetThread()->TestDestroy()) {
        RunNextInstruction();

        auto now = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = now - start;

        // Notifica a thread principal para atualizar a interface 60 vezes por
        // segundo.
        if (elapsed_seconds.count() > frame_time) {
            start = now;
            wxQueueEvent(GetEventHandler(), new wxThreadEvent());
            semaphore.Wait();
        }

        if (cpu.halted) {
            thread_is_running = false;
            button_panel->btn_run->SetValue(false);
            break;
        }

        if (!thread_is_running) {
            break;
        }
    }

    // Antes de encerrar a thread, notifica um última vez para atualizar a
    // interface.
    wxQueueEvent(GetEventHandler(), new wxThreadEvent());
    semaphore.Wait();

    return static_cast<wxThread::ExitCode>(0);
}

} // namespace cesar::gui
