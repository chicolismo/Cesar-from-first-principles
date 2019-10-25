#ifndef PANELS_H
#define PANELS_H
#include "../gui.h"

struct DigitalDisplay : public wxPanel {
    static const std::size_t width = 76;
    static const std::size_t height = 25;
    static const std::size_t digit_width = 12;
    static const std::size_t digit_offset = digit_width + 2;
    std::size_t number_of_digits;

    const wxBrush *black_brush = wxBLACK_BRUSH;
    const wxPen highlight_pen = wxPen(wxColour(255, 255, 255, 128));
    const wxPen shadow_pen = wxPen(wxColour(0, 0, 0, 100));

    wxBitmap images[17];
    uint16_t value;
    Base base;

    DigitalDisplay(wxWindow *parent);

    void OnPaint(wxPaintEvent &event);
    void PaintNow();
    void Render(wxDC &dc);
    void SetValue(const uint16_t value);
    void SetBase(Base base);
    wxDECLARE_EVENT_TABLE();
};


struct BinaryDisplay : public wxPanel {
    static const std::size_t width = 80;
    static const std::size_t height = 5;
    static const std::size_t image_size = 5;
    static const std::size_t number_of_digits = 16;
    const wxBrush brush = wxBrush(*wxWHITE);

    wxImage images[2];

    uint16_t value;

    BinaryDisplay(wxWindow *parent);
    void OnPaint(wxPaintEvent &event);
    void PaintNow();
    void Render(wxDC &dc);
    void SetValue(const uint16_t value);
    wxDECLARE_EVENT_TABLE();
};


struct RegisterPanel : public wxPanel {
    DigitalDisplay *digital_display;
    BinaryDisplay *binary_display;
    uint16_t value;
    Base base;

    RegisterPanel(wxWindow *parent, long id, const wxString &title);
    void SetValue(const Word word);
    Word GetValue() const;
};


struct ExecutionPanel : public wxPanel {
    DigitalDisplay *access_display;
    DigitalDisplay *instruction_display;

    ExecutionPanel(wxWindow *parent);
};

#endif//PANELS_H
