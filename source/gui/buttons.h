#ifndef BUTTONS_H
#define BUTTONS_H

#include "gui.h"

#include <wx/dc.h>
#include <wx/settings.h>

struct BaseToggleButton : public wxWindow {
    wxColour face_color, hightlight_color, shadow_color, shadow3d_color;

    wxImage images[2];

    BaseToggleButton(wxWindow *parent, long id);

    void Render(wxDC &dc);

    void PaintNow();

    void OnPaint(wxPaintEvent &event);

    wxDECLARE_EVENT_TABLE();
};

#endif // BUTTONS_H
