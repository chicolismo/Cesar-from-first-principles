#include "buttons.h"
#include "images/dechex10.xpm"

wxBEGIN_EVENT_TABLE(BaseToggleButton, wxToggleButton)
    EVT_PAINT(BaseToggleButton::OnPaint)
wxEND_EVENT_TABLE()

BaseToggleButton ::BaseToggleButton(wxWindow *parent, long id) : wxToggleButton(parent, id, wxEmptyString, wxDefaultPosition, wxSize(66, 28)) {
    this->SetSize(0, 0, 66, 28, 0);

    images[0] = wxImage(dechex10);
}

void BaseToggleButton::Render(wxDC &dc) {
    dc.DrawBitmap(images[0], 0, 0, false);
}


void BaseToggleButton::PaintNow() {
    wxClientDC dc(this);
    Render(dc);
}


void BaseToggleButton::OnPaint(wxPaintEvent &WXUNUSED(event)) {
    wxPaintDC dc(this);
    Render(dc);
    //event.Skip();
}
