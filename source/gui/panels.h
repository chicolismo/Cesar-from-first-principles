#ifndef CESAR_GUI_PANELS_H
#define CESAR_GUI_PANELS_H

#include "gui.h"
#include <wx/tglbtn.h>

namespace cesar::gui {

struct DigitalDisplay : public wxWindow {
    constexpr static int BASE_VALUE[] = {[Decimal] = 10, [Hexadecimal] = 16};
    static const std::size_t width = 76;
    static const std::size_t height = 25;
    static const std::size_t digit_width = 12;
    static const std::size_t digit_offset = digit_width + 2;
    std::size_t number_of_digits;

    const wxBrush *black_brush = wxBLACK_BRUSH;
    const wxPen highlight_pen = wxPen(wxColour(255, 255, 255, 128));
    const wxPen shadow_pen = wxPen(wxColour(0, 0, 0, 100));

    wxBitmap images[17];
    std::uint16_t value;
    Base current_base;

    explicit DigitalDisplay(wxWindow *parent);

    void OnPaint(wxPaintEvent &event);

    void PaintNow();

    void Render(wxDC &dc);

    void SetValue(std::uint16_t unsigned_word);

    void SetBase(Base new_base);

    wxDECLARE_EVENT_TABLE();
};

struct BinaryDisplay : public wxWindow {
    static const std::size_t width = 80;
    static const std::size_t height = 5;
    static const std::size_t image_size = 5;
    static const std::size_t number_of_digits = 16;

    wxImage images[2];

    uint16_t value;

    explicit BinaryDisplay(wxWindow *parent);

    void OnPaint(wxPaintEvent &event);

    void PaintNow();

    void Render(wxDC &dc);

    void SetValue(uint16_t unsigned_word);

    wxDECLARE_EVENT_TABLE();
};

struct RegisterPanel : public wxPanel {
    DigitalDisplay *digital_display;
    BinaryDisplay *binary_display;
    Base current_base;
    std::int16_t current_value;
    const int register_number;

    RegisterPanel(wxWindow *parent, int number, const wxString &title);

    void Update();

    void SetValue(std::int16_t word);

    std::int16_t GetValue() const;

    void SetBase(Base new_base);

    void OnDoubleClick(wxMouseEvent &event);
};

struct ExecutionPanel : public wxPanel {
    DigitalDisplay *access_display;
    DigitalDisplay *instruction_display;

    explicit ExecutionPanel(wxWindow *parent);
};

struct Led : public wxWindow {
    bool turned_on;
    wxImage images[2];

    explicit Led(wxWindow *parent);

    void PaintNow();

    void OnPaint(wxPaintEvent &event);

    void Render(wxDC &dc);

    void SetTurnedOn(bool should_turn_on);

    wxDECLARE_EVENT_TABLE();
};

struct ConditionPanel : public wxPanel {
    Led *led_display;

    ConditionPanel(wxWindow *parent, const wxString &label);
};


struct ButtonPanel : public wxPanel {
    wxToggleButton *btn_decimal;
    wxToggleButton *btn_hexadecimal;
    wxBitmapToggleButton *btn_run;
    wxBitmapButton *btn_next;

    ButtonPanel(wxWindow *parent);

    void OnChangeBase(wxCommandEvent &event);

    wxDECLARE_EVENT_TABLE();
};

} // namespace cesar::gui

#endif
