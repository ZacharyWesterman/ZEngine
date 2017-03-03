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
#include <wx/tglbtn.h>
#include <wx/panel.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/button.h>
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
        static const long ID_RICHTEXTCTRL1;
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
        wxNotebook* Notebook1;
        wxPanel* Panel4;
        wxRichTextCtrl* RichTextCtrl1;
        wxButton* Button1;
        wxToggleButton* ToggleButton1;
        wxPanel* Panel1;
        wxButton* Button2;
        wxPanel* Panel3;
        wxButton* Button3;
        wxStatusBar* StatusBar1;
        wxPanel* Panel2;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // EDITORMAIN_H
