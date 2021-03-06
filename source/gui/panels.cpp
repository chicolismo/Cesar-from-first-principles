#include "panels.h"

#include "images/cesar_0.xpm"
#include "images/cesar_1.xpm"
#include "images/cesar_2.xpm"
#include "images/cesar_3.xpm"
#include "images/cesar_4.xpm"
#include "images/cesar_5.xpm"
#include "images/cesar_6.xpm"
#include "images/cesar_7.xpm"
#include "images/cesar_8.xpm"
#include "images/cesar_9.xpm"
#include "images/cesar_a.xpm"
#include "images/cesar_b.xpm"
#include "images/cesar_c.xpm"
#include "images/cesar_d.xpm"
#include "images/cesar_e.xpm"
#include "images/cesar_f.xpm"
#include "images/cesar_null.xpm"
#include "images/config.xpm"
#include "images/light_off.xpm"
#include "images/light_on.xpm"
#include "images/mini_led_0.xpm"
#include "images/mini_led_1.xpm"
#include "images/tools.xpm"
#include "main_window.h"
#include <wx/anybutton.h>

namespace cesar::gui {

// ===========================================================================
// DigitalDisplay
// ===========================================================================

wxBEGIN_EVENT_TABLE(DigitalDisplay, wxPanel)
    EVT_PAINT(DigitalDisplay::OnPaint)
wxEND_EVENT_TABLE()

DigitalDisplay::DigitalDisplay(wxWindow *parent)
        : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(width, height)) {
    images[0] = wxImage(cesar_0);
    images[1] = wxImage(cesar_1);
    images[2] = wxImage(cesar_2);
    images[3] = wxImage(cesar_3);
    images[4] = wxImage(cesar_4);
    images[5] = wxImage(cesar_5);
    images[6] = wxImage(cesar_6);
    images[7] = wxImage(cesar_7);
    images[8] = wxImage(cesar_8);
    images[9] = wxImage(cesar_9);
    images[10] = wxImage(cesar_a);
    images[11] = wxImage(cesar_b);
    images[12] = wxImage(cesar_c);
    images[13] = wxImage(cesar_d);
    images[14] = wxImage(cesar_e);
    images[15] = wxImage(cesar_f);
    images[16] = wxImage(cesar_null);

    value = 0;
    current_base = Base::Decimal;
    number_of_digits = 5;
}

void DigitalDisplay::Render(wxDC &dc) {
    dc.SetBrush(*black_brush);
    dc.DrawRectangle(wxPoint(1, 1), wxSize(width - 2, height - 2));

    dc.SetPen(shadow_pen);
    dc.DrawLine(0, 0, width - 2, 0);
    dc.DrawLine(0, 1, 0, height - 2);

    dc.SetPen(highlight_pen);
    dc.DrawLine(0, height - 1, width - 1, height - 1);
    dc.DrawLine(width - 1, 0, width - 1, height - 1);

    const std::size_t y = 4;
    std::size_t x = 60;
    std::size_t current_digit = 0;
    uint16_t n = this->value;

    const std::size_t base_value = BASE_VALUE[current_base];
    do {
        const std::size_t digit = n % base_value;
        dc.DrawBitmap(images[digit], x, y, false);
        x -= digit_offset;
        ++current_digit;
        n /= base_value;
    } while (n > 0);

    while (current_digit < number_of_digits) {
        dc.DrawBitmap(images[16], x, y, false);
        x -= digit_offset;
        ++current_digit;
    }
}

void DigitalDisplay::SetBase(Base new_base) {
    if (current_base != new_base) {
        current_base = new_base;
        if (new_base == Decimal) {
            number_of_digits = 5;
        }
        else {
            number_of_digits = 4;
        }
        PaintNow();
    }
}

void DigitalDisplay::SetValue(const uint16_t unsigned_word) {
    this->value = unsigned_word;
    PaintNow();
}

void DigitalDisplay::PaintNow() {
    wxClientDC dc(this);
    Render(dc);
}

void DigitalDisplay::OnPaint(wxPaintEvent &event) {
    wxPaintDC dc(this);
    Render(dc);
    event.Skip();
}

// ===========================================================================
// BinaryDisplay
// ===========================================================================

wxBEGIN_EVENT_TABLE(BinaryDisplay, wxPanel)
    EVT_PAINT(BinaryDisplay::OnPaint)
wxEND_EVENT_TABLE()

BinaryDisplay::BinaryDisplay(wxWindow *parent)
        : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(width, height)) {
    value = 0;
    images[0] = wxImage(mini_led_0);
    images[1] = wxImage(mini_led_1);
}

void BinaryDisplay::Render(wxDC &dc) {
    const std::size_t y = 0;
    std::size_t x = 5 * 15;
    uint16_t n = this->value;
    for (std::size_t i = 0; i < number_of_digits; ++i) {
        int bit = (n & 1);
        dc.DrawBitmap(images[bit], x, y, true);
        x -= image_size;
        n >>= 1;
    }
}

void BinaryDisplay::PaintNow() {
    wxClientDC dc(this);
    Render(dc);
}

void BinaryDisplay::OnPaint(wxPaintEvent &event) {
    wxPaintDC dc(this);
    Render(dc);
    event.Skip();
}

void BinaryDisplay::SetValue(const uint16_t unsigned_word) {
    this->value = unsigned_word;
    PaintNow();
}

// ===========================================================================
// RegisterPanel
// ===========================================================================

RegisterPanel::RegisterPanel(
        wxWindow *parent, int number, const wxString &title)
        : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize),
          register_number(number) {
    auto *box = new wxStaticBoxSizer(wxVERTICAL, this, title);

    current_value = 0;
    current_base = Base::Decimal;

    digital_display = new DigitalDisplay(this);
    binary_display = new BinaryDisplay(this);

    box->Add(digital_display, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 4);
    box->Add(binary_display, 0,
            wxALIGN_CENTER_HORIZONTAL | wxLEFT | wxBOTTOM | wxRIGHT, 4);

    digital_display->Bind(
            wxEVT_LEFT_DCLICK, &RegisterPanel::OnDoubleClick, this);

    binary_display->Bind(
            wxEVT_LEFT_DCLICK, &RegisterPanel::OnDoubleClick, this);

    SetSizerAndFit(box);
}

void RegisterPanel::Update() {
    const std::uint16_t word = static_cast<std::uint16_t>(GetValue());
    digital_display->SetValue(word);
    binary_display->SetValue(word);
}

void RegisterPanel::SetValue(const std::int16_t word) {
    if (current_value != word) {
        current_value = word;
        std::uint16_t value = static_cast<uint16_t>(word);
        digital_display->SetValue(value);
        binary_display->SetValue(value);
    }
}

std::int16_t RegisterPanel::GetValue() const { return current_value; }

void RegisterPanel::SetBase(Base new_base) {
    if (current_base != new_base) {
        current_base = new_base;
        digital_display->SetBase(new_base);
    }
}

void RegisterPanel::OnDoubleClick(wxMouseEvent &event) {
    auto parent = static_cast<MainWindow *>(GetParent());
    parent->OnRegisterPanelDoubleClick(register_number);
    event.Skip();
}

// ===========================================================================
// ExecutionPanel
// ===========================================================================

ExecutionPanel::ExecutionPanel(wxWindow *parent) : wxPanel(parent, wxID_ANY) {
    access_display = new DigitalDisplay(this);
    access_display->number_of_digits = 5;

    instruction_display = new DigitalDisplay(this);
    instruction_display->number_of_digits = 5;

    auto *access_label = new wxStaticText(this, wxID_ANY, wxT("Acessos:"));
    auto *instruction_label =
            new wxStaticText(this, wxID_ANY, wxT("Instruções:"));

    auto *flex_grid = new wxFlexGridSizer(2, 2, 4, 4);
    flex_grid->Add(access_label, 1, wxALIGN_CENTER_VERTICAL);
    flex_grid->Add(access_display);
    flex_grid->Add(instruction_label, 1, wxALIGN_CENTER_VERTICAL);
    flex_grid->Add(instruction_display);
    flex_grid->AddGrowableCol(0, 1);
    flex_grid->AddGrowableRow(0, 1);
    flex_grid->AddGrowableRow(1, 1);

    auto *sizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Execução:"));
    sizer->Add(flex_grid, 0, wxALL, 4);
    SetSizerAndFit(sizer);
}

// ===========================================================================
// Led
// ===========================================================================

wxBEGIN_EVENT_TABLE(Led, wxPanel)
    EVT_PAINT(Led::OnPaint)
wxEND_EVENT_TABLE()

Led::Led(wxWindow *parent)
        : wxWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(15, 15)),
          turned_on(false) {
    images[0] = wxImage(light_off);
    images[1] = wxImage(light_on);
}

void Led::Render(wxDC &dc) {
    dc.DrawBitmap(images[turned_on ? 1 : 0], 0, 0, true);
}

void Led::PaintNow() {
    wxClientDC dc(this);
    Render(dc);
}

void Led::OnPaint(wxPaintEvent &event) {
    wxPaintDC dc(this);
    Render(dc);
    event.Skip();
}

void Led::SetTurnedOn(bool should_turn_on) {
    turned_on = should_turn_on;
    PaintNow();
}

ConditionPanel::ConditionPanel(wxWindow *parent, const wxString &label)
        : wxPanel(parent) {
    led_display = new Led(this);
    auto sizer = new wxStaticBoxSizer(wxVERTICAL, this, label);
    sizer->Add(led_display, 1,
            wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL, 5);
    SetSizerAndFit(sizer);
}

// ===========================================================================
// ButtonPanel
// ===========================================================================

wxBEGIN_EVENT_TABLE(ButtonPanel, wxPanel)
    EVT_TOGGLEBUTTON(ID_Decimal, ButtonPanel::OnChangeBase)
    EVT_TOGGLEBUTTON(ID_Hexadecimal, ButtonPanel::OnChangeBase)
wxEND_EVENT_TABLE()

ButtonPanel::ButtonPanel(wxWindow *parent) : wxPanel(parent, wxID_ANY) {
    btn_decimal = new wxToggleButton(this, ID_Decimal, wxT("Dec"),
            wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    btn_hexadecimal = new wxToggleButton(this, ID_Hexadecimal, wxT("Hex"),
            wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

    btn_run = new wxBitmapToggleButton(this, ID_Run, wxImage(config));
    btn_next = new wxBitmapButton(this, ID_Next, wxImage(tools));

    btn_decimal->SetValue(true);
    btn_hexadecimal->SetValue(false);

    auto *hbox = new wxBoxSizer(wxHORIZONTAL);
    hbox->Add(btn_decimal, 0, wxALIGN_CENTER_VERTICAL);
    hbox->Add(btn_hexadecimal, 0, wxALIGN_CENTER_VERTICAL);
    hbox->AddStretchSpacer();
    // hbox->Add(btn_run, 0, wxALIGN_RIGHT);
    // hbox->Add(btn_next, 0, wxALIGN_RIGHT);
    hbox->Add(btn_run, 0);
    hbox->Add(btn_next, 0);
    SetSizerAndFit(hbox);
}

void ButtonPanel::OnChangeBase(wxCommandEvent &event) {
    auto *parent = static_cast<MainWindow *>(GetParent());

    switch (event.GetId()) {
        case ID_Decimal:
            btn_decimal->SetValue(true);
            btn_hexadecimal->SetValue(false);
            parent->SetBase(Base::Decimal);
            break;
        case ID_Hexadecimal:
            btn_hexadecimal->SetValue(true);
            btn_decimal->SetValue(false);
            parent->SetBase(Base::Hexadecimal);
            break;
    }

    event.Skip();
}

} // namespace cesar::gui
