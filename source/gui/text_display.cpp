#include "text_display.h"

#include "gui.h"

// includes {{{
#include "images/character_00.xpm"
#include "images/character_01.xpm"
#include "images/character_02.xpm"
#include "images/character_03.xpm"
#include "images/character_04.xpm"
#include "images/character_05.xpm"
#include "images/character_06.xpm"
#include "images/character_07.xpm"
#include "images/character_08.xpm"
#include "images/character_09.xpm"
#include "images/character_10.xpm"
#include "images/character_11.xpm"
#include "images/character_12.xpm"
#include "images/character_13.xpm"
#include "images/character_14.xpm"
#include "images/character_15.xpm"
#include "images/character_16.xpm"
#include "images/character_17.xpm"
#include "images/character_18.xpm"
#include "images/character_19.xpm"
#include "images/character_20.xpm"
#include "images/character_21.xpm"
#include "images/character_22.xpm"
#include "images/character_23.xpm"
#include "images/character_24.xpm"
#include "images/character_25.xpm"
#include "images/character_26.xpm"
#include "images/character_27.xpm"
#include "images/character_28.xpm"
#include "images/character_29.xpm"
#include "images/character_30.xpm"
#include "images/character_31.xpm"
#include "images/character_32.xpm"
#include "images/character_33.xpm"
#include "images/character_34.xpm"
#include "images/character_35.xpm"
#include "images/character_36.xpm"
#include "images/character_37.xpm"
#include "images/character_38.xpm"
#include "images/character_39.xpm"
#include "images/character_40.xpm"
#include "images/character_41.xpm"
#include "images/character_42.xpm"
#include "images/character_43.xpm"
#include "images/character_44.xpm"
#include "images/character_45.xpm"
#include "images/character_46.xpm"
#include "images/character_47.xpm"
#include "images/character_48.xpm"
#include "images/character_49.xpm"
#include "images/character_50.xpm"
#include "images/character_51.xpm"
#include "images/character_52.xpm"
#include "images/character_53.xpm"
#include "images/character_54.xpm"
#include "images/character_55.xpm"
#include "images/character_56.xpm"
#include "images/character_57.xpm"
#include "images/character_58.xpm"
#include "images/character_59.xpm"
#include "images/character_60.xpm"
#include "images/character_61.xpm"
#include "images/character_62.xpm"
#include "images/character_63.xpm"
#include "images/character_64.xpm"
#include "images/character_65.xpm"
#include "images/character_66.xpm"
#include "images/character_67.xpm"
#include "images/character_68.xpm"
#include "images/character_69.xpm"
#include "images/character_70.xpm"
#include "images/character_71.xpm"
#include "images/character_72.xpm"
#include "images/character_73.xpm"
#include "images/character_74.xpm"
#include "images/character_75.xpm"
#include "images/character_76.xpm"
#include "images/character_77.xpm"
#include "images/character_78.xpm"
#include "images/character_79.xpm"
#include "images/character_80.xpm"
#include "images/character_81.xpm"
#include "images/character_82.xpm"
#include "images/character_83.xpm"
#include "images/character_84.xpm"
#include "images/character_85.xpm"
#include "images/character_86.xpm"
#include "images/character_87.xpm"
#include "images/character_88.xpm"
#include "images/character_89.xpm"
#include "images/character_90.xpm"
#include "images/character_91.xpm"
#include "images/character_92.xpm"
#include "images/character_93.xpm"
#include "images/character_94.xpm"
// }}}

// ===========================================================================
// TextDisplay
// ===========================================================================

wxBEGIN_EVENT_TABLE(TextDisplay, wxDialog)
    EVT_PAINT(TextDisplay::OnPaint)
wxEND_EVENT_TABLE();

TextDisplay::TextDisplay(wxWindow *parent, Cpu *cpu)
    : wxDialog(
          parent, wxID_ANY, wxT("Visor"), wxDefaultPosition, wxSize(width, height), wxCAPTION) {
    this->cpu = cpu;

    this->SetClientSize(width, height);

    this->display_start = &(cpu->memory[Cpu::BEGIN_DISPLAY_ADDRESS]);
    this->display_end = &(cpu->memory[Cpu::END_DISPLAY_ADDRESS + 1]);

    this->images[0] = wxImage(character_00);
    this->images[1] = wxImage(character_01);
    this->images[2] = wxImage(character_02);
    this->images[3] = wxImage(character_03);
    this->images[4] = wxImage(character_04);
    this->images[5] = wxImage(character_05);
    this->images[6] = wxImage(character_06);
    this->images[7] = wxImage(character_07);
    this->images[8] = wxImage(character_08);
    this->images[9] = wxImage(character_09);
    this->images[10] = wxImage(character_10);
    this->images[11] = wxImage(character_11);
    this->images[12] = wxImage(character_12);
    this->images[13] = wxImage(character_13);
    this->images[14] = wxImage(character_14);
    this->images[15] = wxImage(character_15);
    this->images[16] = wxImage(character_16);
    this->images[17] = wxImage(character_17);
    this->images[18] = wxImage(character_18);
    this->images[19] = wxImage(character_19);
    this->images[20] = wxImage(character_20);
    this->images[21] = wxImage(character_21);
    this->images[22] = wxImage(character_22);
    this->images[23] = wxImage(character_23);
    this->images[24] = wxImage(character_24);
    this->images[25] = wxImage(character_25);
    this->images[26] = wxImage(character_26);
    this->images[27] = wxImage(character_27);
    this->images[28] = wxImage(character_28);
    this->images[29] = wxImage(character_29);
    this->images[30] = wxImage(character_30);
    this->images[31] = wxImage(character_31);
    this->images[32] = wxImage(character_32);
    this->images[33] = wxImage(character_33);
    this->images[34] = wxImage(character_34);
    this->images[35] = wxImage(character_35);
    this->images[36] = wxImage(character_36);
    this->images[37] = wxImage(character_37);
    this->images[38] = wxImage(character_38);
    this->images[39] = wxImage(character_39);
    this->images[40] = wxImage(character_40);
    this->images[41] = wxImage(character_41);
    this->images[42] = wxImage(character_42);
    this->images[43] = wxImage(character_43);
    this->images[44] = wxImage(character_44);
    this->images[45] = wxImage(character_45);
    this->images[46] = wxImage(character_46);
    this->images[47] = wxImage(character_47);
    this->images[48] = wxImage(character_48);
    this->images[49] = wxImage(character_49);
    this->images[50] = wxImage(character_50);
    this->images[51] = wxImage(character_51);
    this->images[52] = wxImage(character_52);
    this->images[53] = wxImage(character_53);
    this->images[54] = wxImage(character_54);
    this->images[55] = wxImage(character_55);
    this->images[56] = wxImage(character_56);
    this->images[57] = wxImage(character_57);
    this->images[58] = wxImage(character_58);
    this->images[59] = wxImage(character_59);
    this->images[60] = wxImage(character_60);
    this->images[61] = wxImage(character_61);
    this->images[62] = wxImage(character_62);
    this->images[63] = wxImage(character_63);
    this->images[64] = wxImage(character_64);
    this->images[65] = wxImage(character_65);
    this->images[66] = wxImage(character_66);
    this->images[67] = wxImage(character_67);
    this->images[68] = wxImage(character_68);
    this->images[69] = wxImage(character_69);
    this->images[70] = wxImage(character_70);
    this->images[71] = wxImage(character_71);
    this->images[72] = wxImage(character_72);
    this->images[73] = wxImage(character_73);
    this->images[74] = wxImage(character_74);
    this->images[75] = wxImage(character_75);
    this->images[76] = wxImage(character_76);
    this->images[77] = wxImage(character_77);
    this->images[78] = wxImage(character_78);
    this->images[79] = wxImage(character_79);
    this->images[80] = wxImage(character_80);
    this->images[81] = wxImage(character_81);
    this->images[82] = wxImage(character_82);
    this->images[83] = wxImage(character_83);
    this->images[84] = wxImage(character_84);
    this->images[85] = wxImage(character_85);
    this->images[86] = wxImage(character_86);
    this->images[87] = wxImage(character_87);
    this->images[88] = wxImage(character_88);
    this->images[89] = wxImage(character_89);
    this->images[90] = wxImage(character_90);
    this->images[91] = wxImage(character_91);
    this->images[92] = wxImage(character_92);
    this->images[93] = wxImage(character_93);
    this->images[94] = wxImage(character_94);
}

void TextDisplay::Render(wxDC &dc) {
    wxBrush brush{ *wxBLACK };
    wxPen pen{ *wxBLACK };
    dc.SetPen(pen);
    dc.SetBrush(brush);
    dc.DrawRectangle(0, 0, width, height);

    int x = 1;
    Byte *byte_address = display_start;
    while (byte_address != display_end) {
        UByte value = static_cast<UByte>(*byte_address) - 32;
        if (value < images_size) {
            dc.DrawBitmap(images[value], x, 2, false);
        }
        else {
            dc.DrawBitmap(images[0], x, 2, false);
        }
        ++byte_address;
        x += (char_width + 1);
    }
}

void TextDisplay::PaintNow() {
    wxClientDC dc{ this };
    Render(dc);
}

void TextDisplay::OnPaint(wxPaintEvent &event) {
    wxPaintDC dc{ this };
    Render(dc);
    event.Skip();
}
