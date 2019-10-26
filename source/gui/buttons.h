#ifndef BUTTONS_H
#define BUTTONS_H

#include "gui.h"

#include <wx/dc.h>
#include <wx/tglbtn.h>

struct BaseToggleButton : public wxToggleButton {
    wxImage images[2];

    BaseToggleButton(wxWindow *parent, long id);

    void Render(wxDC &dc);

    void PaintNow();

    void OnPaint(wxPaintEvent &event);

    wxDECLARE_EVENT_TABLE();
};

#endif // BUTTONS_H
