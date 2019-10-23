#include "gui.h"

wxBEGIN_EVENT_TABLE(ProgramWindow, wxDialog)
    EVT_CLOSE(ProgramWindow::OnClose)
wxEND_EVENT_TABLE()

ProgramWindow::ProgramWindow(
    wxWindow *parent, Cpu *cpu, const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxDialog(parent, wxID_ANY, title, pos, size, wxCAPTION | wxRESIZE_BORDER) {

    table = new ProgramTable(this, cpu);
    label = new wxStaticText(this, wxID_ANY, wxT("Endereço"), wxDefaultPosition, wxDefaultSize, 0);
    label->Wrap(-1);
    input = new wxTextCtrl(this, wxID_ANY);

    auto *vbox = new wxBoxSizer(wxVERTICAL);

    //auto *hbox1 = new wxBoxSizer(wxHORIZONTAL);
    //hbox1->Add(table, wxEXPAND);

    auto *hbox2 = new wxBoxSizer(wxHORIZONTAL);
    hbox2->Add(label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
    hbox2->Add(input, 0, wxALL, 0);

    vbox->Add(table, 1, wxEXPAND | wxALL, 4);
    //vbox->Add(hbox1, 1, wxEXPAND, 4);
    //vbox->AddSpacer(4);
    vbox->Add(hbox2, 0, wxALIGN_RIGHT | wxALL, 4);

    SetSizer(vbox);
    Layout();
    Center(wxBOTH);
}

void ProgramWindow::OnClose(wxCloseEvent &event) {
    event.StopPropagation(); // Não pode fechar as janelas laterais.
    return;
}

//wxBEGIN_EVENT_TABLE(DataWindow, wxDialog)
    //EVT_CLOSE(DataWindow::OnClose)
//wxEND_EVENT_TABLE()

//DataWindow::DataWindow(
    //wxWindow *parent, Cpu *cpu, const wxString &title, const wxPoint &pos, const wxSize &size)
    //: wxDialog(parent, wxID_ANY, title, pos, size, wxCAPTION | wxRESIZE_BORDER) {

    //table = new DataTable(this, cpu);
    //label = new wxStaticText(this, wxID_ANY, wxT("Endereço"), wxDefaultPosition, wxDefaultSize, 0);
    //label->Wrap(-1);
    //input = new wxTextCtrl(this, wxID_ANY);

    //auto *vbox = new wxBoxSizer(wxVERTICAL);

    //auto *hbox1 = new wxBoxSizer(wxHORIZONTAL);
    //hbox1->Add(table, wxEXPAND);

    //auto *hbox2 = new wxBoxSizer(wxHORIZONTAL);
    //hbox2->Add(label, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
    //hbox2->Add(input, 0, wxALL, 0);

    //vbox->Add(hbox1, 1, wxEXPAND, 4);
    //// vbox->AddSpacer(4);
    //vbox->Add(hbox2, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 4);

    //SetSizer(vbox);
    //Layout();
    //Center(wxBOTH);
//}

//void DataWindow::OnClose(wxCloseEvent &event) {
    //event.StopPropagation(); // Não pode fechar as janelas laterais.
    //return;
//}
