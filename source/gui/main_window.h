#ifndef CESAR_GUI_WINDOWS_H
#define CESAR_GUI_WINDOWS_H

#include "gui.h"
#include "panels.h"
#include "side_windows.h"
#include "tables.h"
#include "text_display.h"

#include <atomic>
#include <wx/filedlg.h>
#include <wx/thread.h>

namespace cesar::gui {

struct MainWindow;
struct ProgramWindow;
struct DataWindow;

wxDECLARE_EVENT(wxEVT_THREAD_UPDATE, wxThreadEvent);

struct MainWindow : public wxFrame, public wxThreadHelper {

    // O tempo entre frames em segundos, para 60 frames por segundo.
    constexpr static double frame_time = (1.0 / 60.0);

    Cpu cpu;

    wxSemaphore semaphore;

    ProgramWindow *program_window;

    DataWindow *data_window;

    RegisterPanel *register_panels[8];

    ConditionPanel *condition_panels[4];

    ExecutionPanel *execution_panel;

    ButtonPanel *button_panel;

    TextDisplay *text_display;

    Base current_base = Base::Decimal;

    // Determina se as janelas laterais devem ser trazidas
    // para a frente quando a janela principal for selecionada.
    // Esta flag evita uma recursão de OnActivate.
    bool should_raise_windows;

    std::atomic<bool> thread_is_running;

    MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size);

    void UpdateSubwindowsPositions();

    void UpdatePanels();

    void RefreshPanels();

    void RunNextInstruction();

    void SetAddressValueAndUpdateTables(long address, std::int8_t value);

    void SetBase(Base new_base);

    void OnFileOpen(wxCommandEvent &event);

    void OnMove(wxMoveEvent &event);

    void OnExit(wxCommandEvent &event);

    void OnClose(wxCloseEvent &event);

    void OnRunButtonToggle(wxCommandEvent &event);

    void OnNextButtonClick(wxCommandEvent &event);

    void OnRegisterPanelDoubleClick(int register_number);

    void OnActivate(wxActivateEvent &event);

    void OnThreadUpdate(wxThreadEvent &event);

    void StartThread();

    virtual wxThread::ExitCode Entry();

    wxDECLARE_EVENT_TABLE();
};

} // namespace cesar::gui
#endif
