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

namespace cesar::gui {

// ===========================================================================
// TextDisplay
// ===========================================================================

wxBEGIN_EVENT_TABLE(TextDisplay, wxDialog)
    EVT_PAINT(TextDisplay::OnPaint)
wxEND_EVENT_TABLE();

TextDisplay::TextDisplay(wxWindow *parent, Cpu *cpu)
        : wxDialog(parent, wxID_ANY, wxT("Visor"), wxDefaultPosition,
                  wxSize(width, height), wxCAPTION | wxRESIZE_BORDER),
          cpu(cpu) {
    SetClientSize(width, height);
    SetMaxSize(GetSize());

    display_start = &(cpu->memory[Cpu::BEGIN_DISPLAY_ADDRESS]);
    display_end = &(cpu->memory[Cpu::END_DISPLAY_ADDRESS + 1]);

    images[0] = wxImage(character_00);
    images[1] = wxImage(character_01);
    images[2] = wxImage(character_02);
    images[3] = wxImage(character_03);
    images[4] = wxImage(character_04);
    images[5] = wxImage(character_05);
    images[6] = wxImage(character_06);
    images[7] = wxImage(character_07);
    images[8] = wxImage(character_08);
    images[9] = wxImage(character_09);
    images[10] = wxImage(character_10);
    images[11] = wxImage(character_11);
    images[12] = wxImage(character_12);
    images[13] = wxImage(character_13);
    images[14] = wxImage(character_14);
    images[15] = wxImage(character_15);
    images[16] = wxImage(character_16);
    images[17] = wxImage(character_17);
    images[18] = wxImage(character_18);
    images[19] = wxImage(character_19);
    images[20] = wxImage(character_20);
    images[21] = wxImage(character_21);
    images[22] = wxImage(character_22);
    images[23] = wxImage(character_23);
    images[24] = wxImage(character_24);
    images[25] = wxImage(character_25);
    images[26] = wxImage(character_26);
    images[27] = wxImage(character_27);
    images[28] = wxImage(character_28);
    images[29] = wxImage(character_29);
    images[30] = wxImage(character_30);
    images[31] = wxImage(character_31);
    images[32] = wxImage(character_32);
    images[33] = wxImage(character_33);
    images[34] = wxImage(character_34);
    images[35] = wxImage(character_35);
    images[36] = wxImage(character_36);
    images[37] = wxImage(character_37);
    images[38] = wxImage(character_38);
    images[39] = wxImage(character_39);
    images[40] = wxImage(character_40);
    images[41] = wxImage(character_41);
    images[42] = wxImage(character_42);
    images[43] = wxImage(character_43);
    images[44] = wxImage(character_44);
    images[45] = wxImage(character_45);
    images[46] = wxImage(character_46);
    images[47] = wxImage(character_47);
    images[48] = wxImage(character_48);
    images[49] = wxImage(character_49);
    images[50] = wxImage(character_50);
    images[51] = wxImage(character_51);
    images[52] = wxImage(character_52);
    images[53] = wxImage(character_53);
    images[54] = wxImage(character_54);
    images[55] = wxImage(character_55);
    images[56] = wxImage(character_56);
    images[57] = wxImage(character_57);
    images[58] = wxImage(character_58);
    images[59] = wxImage(character_59);
    images[60] = wxImage(character_60);
    images[61] = wxImage(character_61);
    images[62] = wxImage(character_62);
    images[63] = wxImage(character_63);
    images[64] = wxImage(character_64);
    images[65] = wxImage(character_65);
    images[66] = wxImage(character_66);
    images[67] = wxImage(character_67);
    images[68] = wxImage(character_68);
    images[69] = wxImage(character_69);
    images[70] = wxImage(character_70);
    images[71] = wxImage(character_71);
    images[72] = wxImage(character_72);
    images[73] = wxImage(character_73);
    images[74] = wxImage(character_74);
    images[75] = wxImage(character_75);
    images[76] = wxImage(character_76);
    images[77] = wxImage(character_77);
    images[78] = wxImage(character_78);
    images[79] = wxImage(character_79);
    images[80] = wxImage(character_80);
    images[81] = wxImage(character_81);
    images[82] = wxImage(character_82);
    images[83] = wxImage(character_83);
    images[84] = wxImage(character_84);
    images[85] = wxImage(character_85);
    images[86] = wxImage(character_86);
    images[87] = wxImage(character_87);
    images[88] = wxImage(character_88);
    images[89] = wxImage(character_89);
    images[90] = wxImage(character_90);
    images[91] = wxImage(character_91);
    images[92] = wxImage(character_92);
    images[93] = wxImage(character_93);
    images[94] = wxImage(character_94);

    DoLayout();
}

void TextDisplay::Render(wxDC &dc) {
    wxBrush brush{*wxBLACK};
    wxPen pen{*wxBLACK};
    dc.SetPen(pen);
    dc.SetBrush(brush);
    dc.DrawRectangle(0, 0, width, height);

    int x = 1;
    std::int8_t *byte_address = display_start;
    while (byte_address != display_end) {
        std::uint8_t value = static_cast<std::uint8_t>(*byte_address) - 32;
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
    wxClientDC dc{this};
    Render(dc);
}

void TextDisplay::OnPaint(wxPaintEvent &event) {
    wxPaintDC dc{this};
    Render(dc);
    event.Skip();
}

} // namespace cesar::gui
