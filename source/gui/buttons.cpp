#include "buttons.h"

#include "images/dechex10.xpm"

wxBEGIN_EVENT_TABLE(BaseToggleButton, wxWindow)
    EVT_PAINT(BaseToggleButton::OnPaint)
wxEND_EVENT_TABLE()

/*
wxSYS_COLOUR_BTNFACE
wxSYS_COLOUR_BTNHIGHLIGHT
wxSYS_COLOUR_BTNSHADOW
wxSYS_COLOUR_3DDKSHADOW
*/
BaseToggleButton ::BaseToggleButton(wxWindow *parent, long id)
    : wxWindow(parent, id, wxDefaultPosition) {

    //face_color = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
    face_color = wxColour(0xcccccc);
    //hightlight_color = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT);
    hightlight_color = wxColour(0xdddddd);
    //hightlight_color = wxColour(*wxWHITE);
    //shadow_color = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW);
    shadow_color = wxColour(0xaaaaaa);
    //shadow3d_color = wxSystemSettings::GetColour(wxSYS_COLOUR_3DDKSHADOW);
    shadow3d_color = wxColour(0x999999);

    const wxSize size = wxSize(66, 28);
    this->SetSize(size);
    this->SetMinSize(size);

    images[0] = wxImage(dechex10);
}

void BaseToggleButton::Render(wxDC &dc) {
    wxCoord xmax{ 65 };
    wxCoord ymax{ 23 };

    wxBrush brush(face_color);
    wxPen pen(face_color);
    dc.SetPen(pen);
    dc.SetBrush(brush);
    dc.DrawRectangle(2, 2, 29, 21);
    dc.DrawRectangle(34, 1, 30, 22);

    dc.DrawBitmap(images[0], 0, 0, true);

    pen.SetColour(shadow3d_color);
    dc.SetPen(pen);
    dc.DrawLine(0, 0, 31, 0);
    dc.DrawLine(0, 0, 0, ymax - 1);
    dc.DrawLine(xmax, 0, xmax, ymax);
    dc.DrawLine(32, ymax, xmax, ymax);

    pen.SetColour(shadow_color);
    dc.SetPen(pen);
    dc.DrawLine(1, 1, 30, 1);
    dc.DrawLine(1, 1, 1, ymax - 2);
    dc.DrawLine(xmax - 1, 1, xmax - 1, ymax - 1);
    dc.DrawLine(33, ymax - 1, xmax - 1, ymax - 1);

    pen.SetColour(hightlight_color);
    dc.SetPen(pen);
    dc.DrawLine(0, ymax, 32, ymax);

    dc.DrawLine(32, 0, xmax - 1, 0);
    dc.DrawLine(32, 0, xmax - 1, 0);
    dc.DrawLine(32, 0, 32, ymax);
    dc.DrawLine(33, 0, 33, ymax - 1);

}


void BaseToggleButton::PaintNow() {
    wxClientDC dc(this);
    Render(dc);
}


void BaseToggleButton::OnPaint(wxPaintEvent &WXUNUSED(event)) {
    wxPaintDC dc(this);
    Render(dc);
    // event.Skip();
}
