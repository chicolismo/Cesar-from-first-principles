#include "panels.h"

#include "../gui.h"
#include "../images/cesar_0.xpm"
#include "../images/cesar_1.xpm"
#include "../images/cesar_2.xpm"
#include "../images/cesar_3.xpm"
#include "../images/cesar_4.xpm"
#include "../images/cesar_5.xpm"
#include "../images/cesar_6.xpm"
#include "../images/cesar_7.xpm"
#include "../images/cesar_8.xpm"
#include "../images/cesar_9.xpm"
#include "../images/cesar_a.xpm"
#include "../images/cesar_b.xpm"
#include "../images/cesar_c.xpm"
#include "../images/cesar_d.xpm"
#include "../images/cesar_e.xpm"
#include "../images/cesar_f.xpm"
#include "../images/cesar_null.xpm"
#include "../images/mini_led_0.xpm"
#include "../images/mini_led_1.xpm"

wxBEGIN_EVENT_TABLE(DigitalDisplay, wxPanel)
    EVT_PAINT(DigitalDisplay::OnPaint)
wxEND_EVENT_TABLE()

DigitalDisplay::DigitalDisplay(wxWindow *parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(width, height)) {
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
    base = Base::Decimal;
    number_of_digits = 4;
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

    const std::size_t base_value = BASE_VALUE[base];
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

void DigitalDisplay::SetBase(Base base) {
    this->base = base;
    if (base == Decimal) {
        number_of_digits = 4;
    }
    else {
        number_of_digits = 5;
    }
}

void DigitalDisplay::SetValue(const uint16_t value) {
    this->value = value;
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


// Binary Display

wxBEGIN_EVENT_TABLE(BinaryDisplay, wxPanel)
    EVT_PAINT(BinaryDisplay::OnPaint)
wxEND_EVENT_TABLE()

BinaryDisplay::BinaryDisplay(wxWindow *parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(width, height)) {
    value = 0;
    images[0] = wxImage(mini_led_0);
    images[1] = wxImage(mini_led_1);
}

void BinaryDisplay::Render(wxDC &dc) {
    SetBackgroundColour(*wxWHITE);
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

void BinaryDisplay::SetValue(const uint16_t value) {
    this->value = value;
}


// Register Panel

RegisterPanel::RegisterPanel(wxWindow *parent, long id, const wxString &title) : wxPanel(parent, id, wxDefaultPosition, wxDefaultSize) {
    auto *box = new wxStaticBoxSizer(wxVERTICAL, this, title);
    wxPanel *inner_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(80, 32));
    box->Add(inner_panel);
    digital_display = new DigitalDisplay(inner_panel);
    digital_display->SetPosition(wxPoint(2, 0));
    binary_display = new BinaryDisplay(inner_panel);
    binary_display->SetPosition(wxPoint(0, 27));
    SetSizer(box);
    Fit();
    Layout();
}

void RegisterPanel::SetValue(const Word value) {
    this->value = static_cast<uint16_t>(value);
    digital_display->SetValue(this->value);
    digital_display->PaintNow();
    binary_display->SetValue(this->value);
    binary_display->PaintNow();
}

Word RegisterPanel::GetValue() const {
    return value;
}


// Execution panel
ExecutionPanel::ExecutionPanel(wxWindow *parent) : wxPanel(parent, wxID_ANY) {
    access_display = new DigitalDisplay(this);
    access_display->number_of_digits = 5;

    instruction_display = new DigitalDisplay(this);
    instruction_display->number_of_digits = 5;

    auto *sizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Execução:"));
    auto *inner_panel = new wxPanel(this);
    auto *flex_grid = new wxFlexGridSizer(2, 2, 0, 0);
    auto *access_label = new wxStaticText(this, wxID_ANY, wxT("Acessos: "));
    auto *instruction_label = new wxStaticText(this, wxID_ANY, wxT("Instruções: "));

    inner_panel->SetSizer(flex_grid);
    flex_grid->Add(access_label, 1, wxALIGN_CENTER_VERTICAL);
    flex_grid->Add(access_display);
    flex_grid->Add(instruction_label, 1, wxALIGN_CENTER_VERTICAL);
    flex_grid->Add(instruction_display);
    flex_grid->AddGrowableCol(0, 1);
    flex_grid->AddGrowableRow(0, 1);
    flex_grid->AddGrowableRow(1, 1);
    flex_grid->Fit(inner_panel);

    sizer->Add(flex_grid);
    sizer->Fit(this);
    SetSizer(sizer);
    //sizer->Fit(this);
}
