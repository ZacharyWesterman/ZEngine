/***************************************************************
 * Name:      editorMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Zachary Westerman (zacharywesterman@yahoo.com)
 * Created:   2017-03-01
 * Copyright: Zachary Westerman ()
 * License:
 **************************************************************/

#include "wx_pch.h"
#include "editorMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(editorFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(editorFrame)
const long editorFrame::ID_TEXTCTRL1 = wxNewId();
const long editorFrame::ID_NOTEBOOK1 = wxNewId();
const long editorFrame::ID_PANEL3 = wxNewId();
const long editorFrame::ID_PANEL2 = wxNewId();
const long editorFrame::ID_PANEL1 = wxNewId();
const long editorFrame::idMenuQuit = wxNewId();
const long editorFrame::idMenuAbout = wxNewId();
const long editorFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(editorFrame,wxFrame)
    //(*EventTable(editorFrame)
    //*)
END_EVENT_TABLE()

editorFrame::editorFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(editorFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxFlexGridSizer* FlexGridSizer2;
    wxMenu* Menu1;
    wxStaticBoxSizer* StaticBoxSizer3;
    wxBoxSizer* BoxSizer1;
    wxMenuBar* MenuBar1;
    wxFlexGridSizer* FlexGridSizer1;
    wxMenu* Menu2;

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(1080,607));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    FlexGridSizer1 = new wxFlexGridSizer(1, 2, 0, 0);
    FlexGridSizer1->AddGrowableCol(1);
    FlexGridSizer1->AddGrowableRow(0);
    FlexGridSizer2 = new wxFlexGridSizer(2, 1, 0, 0);
    FlexGridSizer2->AddGrowableRow(0);
    Notebook1 = new wxNotebook(Panel1, ID_NOTEBOOK1, wxDefaultPosition, wxSize(300,-1), 0, _T("ID_NOTEBOOK1"));
    TextCtrl1 = new wxTextCtrl(Notebook1, ID_TEXTCTRL1, _("Text"), wxPoint(27,51), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    Notebook1->AddPage(TextCtrl1, _("Page name"), false);
    FlexGridSizer2->Add(Notebook1, 1, wxALL|wxEXPAND, 5);
    Panel3 = new wxPanel(Panel1, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    FlexGridSizer2->Add(Panel3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer2, 0, wxALIGN_LEFT, 0);
    StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _("Label"));
    Panel2 = new wxPanel(Panel1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    StaticBoxSizer3->Add(Panel2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(StaticBoxSizer3, 1, wxALL|wxEXPAND, 5);
    Panel1->SetSizer(FlexGridSizer1);
    FlexGridSizer1->Fit(Panel1);
    FlexGridSizer1->SetSizeHints(Panel1);
    BoxSizer1->Add(Panel1, 1, wxALL|wxEXPAND, 0);
    SetSizer(BoxSizer1);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    SetSizer(BoxSizer1);
    Layout();

    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&editorFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&editorFrame::OnAbout);
    //*)
}

editorFrame::~editorFrame()
{
    //(*Destroy(editorFrame)
    //*)
}

void editorFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void editorFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}
