#ifndef CESAR_GUI_SIDE_WINDOWS_H
#define CESAR_GUI_SIDE_WINDOWS_H

#include "gui.h"
#include "tables.h"

#include <wx/dialog.h>

namespace cesar::gui {

struct ProgramWindow : public wxDialog {
  Cpu *cpu;
  wxStaticText *label;
  wxTextCtrl *input;
  std::size_t current_value;
  Base current_base;
  ProgramTable *table;

  ProgramWindow(wxWindow *parent, Cpu *cpu, const wxString &title);

  void UpdateLabelAndInputValues();

  void OnTextInputEnter(wxCommandEvent &event);

  void OnClose(wxCloseEvent &event);

  void SetBase(Base new_base);

  void OnItemSelected(wxListEvent &event);

  wxDECLARE_EVENT_TABLE();
};


struct DataWindow : public wxDialog {
  Cpu *cpu;
  wxStaticText *label;
  wxTextCtrl *input;
  std::size_t current_value;
  Base current_base;
  DataTable *table;

  DataWindow(wxWindow *parent, Cpu *cpu, const wxString &title);

  void UpdateLabelAndInputValues();

  void OnTextInputEnter(wxCommandEvent &event);

  void OnClose(wxCloseEvent &event);

  void SetBase(Base new_base);

  void OnItemSelected(wxListEvent &event);

  wxDECLARE_EVENT_TABLE();
};

} // namespace cesar::gui

#endif
