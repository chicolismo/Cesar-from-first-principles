#include "gui.h"

#ifdef _WIN32
    #define FONT_SIZE 10
    #define LIST_STYLE wxLIST_FORMAT_LEFT
#else
    #define FONT_SIZE 12
    #define LIST_STYLE wxLIST_FORMAT_RIGHT
#endif

/* Program Table */

ProgramTable::ProgramTable(wxWindow *parent, Cpu *cpu) : wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_VIRTUAL | wxLC_REPORT | wxLC_HRULES | wxLC_VRULES) {
    this->cpu = cpu;
    this->data = cpu->memory;
    InsertColumn(0, wxT("PC"), LIST_STYLE, wxLIST_AUTOSIZE);
    InsertColumn(1, wxT("Endereço"), LIST_STYLE, wxLIST_AUTOSIZE);
    InsertColumn(2, wxT("Valor"), LIST_STYLE, wxLIST_AUTOSIZE);
    InsertColumn(3, wxT("Mnemônico"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
    SetScrollbar(wxHORIZONTAL, 0, 0, 0, true);
    SetItemCount(MEM_SIZE);
}

wxString ProgramTable::OnGetItemText(long item, long column) const {
    wxString buffer;
    switch (column) {
        case 0:
            buffer.Printf(" ");
            break;
        case 1:
            buffer.Printf("%ld", item);
            break;
        case 2:
            buffer.Printf("%d", 0xFF & cpu->memory[item]);
            break;
        case 3:
            buffer.Printf("%s", wxT("Não implementado"));
            break;
    }
    return buffer;
}

/* Data Table */

//DataTable::DataTable(wxWindow *parent, Cpu *cpu) : wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_VIRTUAL | wxLC_REPORT | wxLC_HRULES | wxLC_VRULES) {
    //this->cpu = cpu;
    //InsertColumn(0, wxT("Endereço"), LIST_STYLE, wxLIST_AUTOSIZE);
    //InsertColumn(1, wxT("Valor"), LIST_STYLE, wxLIST_AUTOSIZE);
    //SetScrollbar(wxHORIZONTAL, 0, 0, 0, true);
//}

//int DataTable::GetColumnCount() const {
    //return 2;
//}

//wxString DataTable::OnGetItemText(long item, long column) const {
    //wxString buffer;
    //switch (column) {
        //case 0:
            //buffer.Printf("%ld", item);
            //break;
        //case 1:
            //buffer.Printf("%d", 0xFF & GetValue(item));
            //break;
    //}
    //return buffer;
//}

#undef FONT_SIZE
#undef LIST_STYLE
