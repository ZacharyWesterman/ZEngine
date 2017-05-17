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
#include <wx/sizer.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/tglbtn.h>
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
        static const long ID_PANEL4;
        static const long ID_NOTEBOOK1;
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_BUTTON3;
        static const long ID_PANEL3;
        static const long ID_TOGGLEBUTTON1;
        static const long ID_PANEL2;
        static const long ID_PANEL1;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(editorFrame)
        wxToggleButton* ToggleButton1;
        wxPanel* Panel1;
        wxStatusBar* StatusBar1;
        wxButton* Button1;
        wxButton* Button2;
        wxButton* Button3;
        wxPanel* Panel2;
        wxPanel* Panel4;
        wxPanel* Panel3;
        wxNotebook* Notebook1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // EDITORMAIN_H
