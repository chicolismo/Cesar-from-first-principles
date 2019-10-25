#include "tables.h"

#include "windows.h"

#ifdef _WIN32
#define FONT_SIZE 10
#define LIST_STYLE wxLIST_FORMAT_LEFT
#else
#define FONT_SIZE 12
#define LIST_STYLE wxLIST_FORMAT_RIGHT
#endif


//----------------------------------------------------------------------------//
// Program Table                                                              //
//----------------------------------------------------------------------------//
ProgramTable::ProgramTable(wxWindow *parent, Cpu *cpu)
    : wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                 wxLC_VIRTUAL | wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_SINGLE_SEL) {
    this->cpu = cpu;
    this->data = cpu->memory;
    program_counter = 0;
    SetFont(wxFont(FONT_SIZE, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    InsertColumn(0, wxT("PC"), LIST_STYLE, 36);
    InsertColumn(1, wxT("Endereço"), LIST_STYLE, 64);
    InsertColumn(2, wxT("Valor"), LIST_STYLE, 60);
    InsertColumn(3, wxT("Mnemônico"), wxLIST_FORMAT_LEFT, 140);
    SetScrollbar(wxHORIZONTAL, 0, 0, 0, true);
    SetItemCount(MEM_SIZE);
}


wxString ProgramTable::OnGetItemText(long item, long column) const {
    wxString buffer;
    switch (column) {
    case 0:
        if (static_cast<uint16_t>(item) == program_counter) {
            buffer.Printf("->");
        }
        else {
            buffer.Printf("");
        }
        break;
    case 1:
        buffer.Printf("%ld", item);
        break;
    case 2:
        buffer.Printf("%d", static_cast<uint8_t>(cpu->memory[item]));
        break;
    case 3:
        buffer.Printf("%s", wxT("Não implementado"));
        break;
    }
    return buffer;
}


void ProgramTable::SetProgramCounter(Word pc) {
    auto old_value = program_counter;
    program_counter = static_cast<uint16_t>(pc);
    RefreshItem(old_value);
    RefreshItem(program_counter);
}


//----------------------------------------------------------------------------//
// Data Table                                                                 //
//----------------------------------------------------------------------------//
DataTable::DataTable(wxWindow *parent, Cpu *cpu)
    : wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                 wxLC_VIRTUAL | wxLC_REPORT | wxLC_HRULES | wxLC_VRULES | wxLC_SINGLE_SEL) {
    this->cpu = cpu;
    this->data = cpu->memory;
    SetFont(wxFont(FONT_SIZE, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    InsertColumn(0, wxT("Endereço"), LIST_STYLE, 64);
    InsertColumn(1, wxT("Valor"), LIST_STYLE, 60);
    SetScrollbar(wxHORIZONTAL, 0, 0, 0, true);
    SetItemCount(MEM_SIZE);
}


wxString DataTable::OnGetItemText(long item, long column) const {
    wxString buffer;
    switch (column) {
    case 0:
        buffer.Printf("%ld", item);
        break;
    case 1:
        buffer.Printf("%d", static_cast<uint8_t>(data[item]));
        break;
    }
    return buffer;
}


#undef FONT_SIZE
#undef LIST_STYLE
