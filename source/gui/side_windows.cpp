#include "side_windows.h"

#include "main_window.h"

#include "utils.h"
#include <wx/toplevel.h>

namespace cesar::gui {

// ===========================================================================
// ProgramWindow
// ===========================================================================

wxBEGIN_EVENT_TABLE(ProgramWindow, wxDialog)
    EVT_CLOSE(ProgramWindow::OnClose)
    EVT_LIST_ITEM_SELECTED(wxID_ANY, ProgramWindow::OnItemSelected)
    EVT_TEXT_ENTER(ID_ValueInput, ProgramWindow::OnTextInputEnter)
wxEND_EVENT_TABLE()

ProgramWindow::ProgramWindow(wxWindow *parent, Cpu *cpu, const wxString &title)
    : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER) {
    //: wxDialog(parent, wxID_ANY, title) {

    this->cpu = cpu;
    this->current_base = Base::Decimal;
    this->current_value = 0L;

    table = new ProgramTable(this, cpu);

    label = new wxStaticText(this, wxID_ANY, wxT("0"), wxDefaultPosition,
        wxSize(60, wxDefaultSize.GetHeight()), wxALIGN_RIGHT);
    label->Wrap(-1);

    input =
        new wxTextCtrl(this, ID_ValueInput, wxEmptyString, wxDefaultPosition,
            wxSize(60, wxDefaultSize.GetHeight()), wxTE_PROCESS_ENTER);

    auto *vbox = new wxBoxSizer(wxVERTICAL);
    auto *hbox = new wxBoxSizer(wxHORIZONTAL);

    hbox->Add(label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
    hbox->Add(input, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    vbox->Add(table, 1, wxEXPAND | wxALL, 4);
    vbox->Add(hbox, 0, wxALL | wxALIGN_RIGHT, 4);

    SetSizer(vbox);
    Layout();
    Fit();
    vbox->Fit(this);
}

void ProgramWindow::OnTextInputEnter(wxCommandEvent &WXUNUSED(event)) {
    std::string input_text{this->input->GetValue().mb_str()};
    bool is_valid_number;
    std::int8_t value =
        TryConvertToByte(input_text, this->current_base, &is_valid_number);
    if (is_valid_number) {
        ((MainWindow *) (this->GetParent()))
            ->SetAddressValueAndUpdateTables(this->current_value, value);
    }
}

void ProgramWindow::OnClose(wxCloseEvent &event) {
    event.StopPropagation(); // Não pode fechar as janelas laterais.
}

void ProgramWindow::SetBase(Base new_base) {
    current_base = new_base;
    table->current_base = new_base;
    table->Refresh();
    UpdateLabelAndInputValues();
}

void ProgramWindow::OnItemSelected(wxListEvent &event) {
    auto row = static_cast<std::size_t>(event.GetIndex());
    current_value = row;
    UpdateLabelAndInputValues();
    input->SetFocus();
    input->SetSelection(-1, -1);
}

void ProgramWindow::UpdateLabelAndInputValues() {
    std::int8_t value = cpu->memory[current_value];
    wxString new_label_value;
    wxString new_input_value;
    if (current_base == Base::Decimal) {
        new_label_value.Printf("%ld", current_value);
        new_input_value.Printf("%d", static_cast<std::uint8_t>(value));
    }
    else {
        new_label_value.Printf("%x", static_cast<std::uint16_t>(current_value));
        new_input_value.Printf("%x", static_cast<std::uint8_t>(value));
    }
    label->SetLabelText(new_label_value);
    input->SetValue(new_input_value);
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
    : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER) {
    //: wxDialog(parent, wxID_ANY, title) {

    this->cpu = cpu;
    this->current_base = Base::Decimal;
    this->current_value = 0L;

    table = new DataTable(this, cpu);

    label = new wxStaticText(this, wxID_ANY, wxT("0"), wxDefaultPosition,
        wxSize(60, wxDefaultSize.GetHeight()), wxALIGN_RIGHT);
    label->Wrap(-1);

    input =
        new wxTextCtrl(this, ID_ValueInput, wxEmptyString, wxDefaultPosition,
            wxSize(60, wxDefaultSize.GetHeight()), wxTE_PROCESS_ENTER);

    auto *vbox = new wxBoxSizer(wxVERTICAL);
    auto *hbox = new wxBoxSizer(wxHORIZONTAL);

    hbox->Add(label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
    hbox->Add(input, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);
    vbox->Add(table, 1, wxEXPAND | wxALL, 4);
    vbox->Add(hbox, 0, wxALL | wxALIGN_RIGHT, 4);

    SetSizer(vbox);
    Layout();
    Fit();
    vbox->Fit(this);
}

void DataWindow::OnTextInputEnter(wxCommandEvent &WXUNUSED(event)) {
    std::string input_text{this->input->GetValue().mb_str()};
    bool is_valid_number;
    std::int8_t value =
        TryConvertToByte(input_text, this->current_base, &is_valid_number);
    if (is_valid_number) {
        ((MainWindow *) (this->GetParent()))
            ->SetAddressValueAndUpdateTables(this->current_value, value);
    }
}

void DataWindow::OnClose(wxCloseEvent &event) {
    event.StopPropagation(); // Não pode fechar as janelas laterais.
}

void DataWindow::SetBase(Base new_base) {
    current_base = new_base;
    table->current_base = new_base;
    table->Refresh();
    UpdateLabelAndInputValues();
}

void DataWindow::OnItemSelected(wxListEvent &event) {
    auto row = static_cast<std::size_t>(event.GetIndex());
    current_value = row;
    UpdateLabelAndInputValues();
    input->SetFocus();
    input->SetSelection(-1, -1);
}

void DataWindow::UpdateLabelAndInputValues() {
    std::int8_t value = cpu->memory[current_value];
    wxString new_label_value;
    wxString new_input_value;
    if (current_base == Base::Decimal) {
        new_label_value.Printf("%ld", current_value);
        new_input_value.Printf("%d", static_cast<std::uint8_t>(value));
    }
    else {
        new_label_value.Printf("%x", static_cast<std::uint16_t>(current_value));
        new_input_value.Printf("%x", static_cast<std::uint8_t>(value));
    }
    label->SetLabelText(new_label_value);
    input->SetValue(new_input_value);
}

} // namespace cesar::gui
