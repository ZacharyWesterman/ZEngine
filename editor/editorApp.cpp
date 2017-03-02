/***************************************************************
 * Name:      editorApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Zachary Westerman (zacharywesterman@yahoo.com)
 * Created:   2017-03-01
 * Copyright: Zachary Westerman ()
 * License:
 **************************************************************/

#include "wx_pch.h"
#include "editorApp.h"

//(*AppHeaders
#include "editorMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(editorApp);

bool editorApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	editorFrame* Frame = new editorFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
