/***************************************************************
 * Name:      editorMain.h
 * Purpose:   Defines Application Frame
 * Author:    Zachary Westerman (zacharywesterman@yahoo.com)
 * Created:   2017-03-01
 * Copyright: Zachary Westerman ()
 * License:
 **************************************************************/

#ifndef EDITORMAIN_H
#define EDITORMAIN_H

//(*Headers(editorFrame)
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)

class editorFrame: public wxFrame
{
    public:

        editorFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~editorFrame();

    private:

        //(*Handlers(editorFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        //*)

        //(*Identifiers(editorFrame)
        static const long ID_TEXTCTRL1;
        static const long ID_NOTEBOOK1;
        static const long ID_STATICBOX2;
        static const long ID_PANEL1;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(editorFrame)
        wxNotebook* Notebook1;
        wxPanel* Panel1;
        wxStaticBox* StaticBox2;
        wxStatusBar* StatusBar1;
        wxTextCtrl* TextCtrl1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // EDITORMAIN_H
