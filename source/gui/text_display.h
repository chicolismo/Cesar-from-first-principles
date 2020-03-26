#ifndef CESAR_GUI_TEXT_DISPLAY_H
#define CESAR_GUI_TEXT_DISPLAY_H

#include "gui.h"
#include <wx/dialog.h>
#include <wx/event.h>
#include <wx/image.h>

namespace cesar::gui {

struct TextDisplay : public wxDialog {
  static const std::size_t images_size{95};
  static const std::size_t n{36};
  static const std::size_t char_width{20};
  static const std::size_t char_height{28};
  static const std::size_t width = (n * char_width) + (n + 1);
  static const std::size_t height = char_height + 4;

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

} // namespace cesar::gui

#endif
