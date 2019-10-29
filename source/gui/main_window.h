#ifndef WINDOWS_H
#define WINDOWS_H

#include "gui.h"
#include "panels.h"
#include "tables.h"
#include "text_display.h"
#include <wx/filedlg.h>
#include "side_windows.h"

struct MainWindow;
struct ProgramWindow;
struct DataWindow;

struct MainWindow : public wxFrame {
    Cpu cpu;

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
    bool should_raise_windows = true;

    MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size);

    void UpdateSubwindowsPositions();

    void UpdatePanels();

    void SetAddressValueAndUpdateTables(long address, std::int8_t value);

    void SetBase(Base new_base);

    void OnFileOpen(wxCommandEvent &event);

    void OnMove(wxMoveEvent &event);

    void OnExit(wxCommandEvent &event);

    void OnNext(wxCommandEvent &event);

    void OnRegisterPanelDoubleClick(int register_number);

    void OnActivate(wxActivateEvent &event);

    wxDECLARE_EVENT_TABLE();
};


#endif
