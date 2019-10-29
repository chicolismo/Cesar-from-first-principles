#ifndef TEXT_DISPLAY_H
#define TEXT_DISPLAY_H

#include <wx/dialog.h>
#include <wx/event.h>
#include <wx/image.h>
#include "../hardware.h"

struct TextDisplay : public wxDialog {
    static const std::size_t images_size{95u};
    static const std::size_t n{36u};
    static const std::size_t char_width{20u};
    static const std::size_t char_height{28u};
    static const std::size_t width = (n * char_width) + (n + 1u);
    static const std::size_t height = char_height + 4u;

    Cpu *cpu;
    wxImage images[images_size];
    std::int8_t *display_start;
    std::int8_t *display_end;

    TextDisplay(wxWindow *parent, Cpu *cpu);

    void OnPaint(wxPaintEvent &event);

    void PaintNow();

    void Render(wxDC &dc);

    wxDECLARE_EVENT_TABLE();
};

#endif//TEXT_DISPLAY_H
