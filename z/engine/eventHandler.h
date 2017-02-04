#pragma once
#ifndef EVENTHANDLER_H_INCLUDED
#define EVENTHANDLER_H_INCLUDED

#include <irrlicht.h>

#include <iostream>
using namespace std;

namespace engine
{

    class eventHandler : public irr::IEventReceiver
    {
    private:
        irr::IrrlichtDevice *device;

        bool gui_button_pressed;
        irr::s32 last_gui_id;

    public:
        eventHandler(irr::IrrlichtDevice *Device) : device(Device)
        {
            gui_button_pressed = false;
            last_gui_id = -1;
        }


        virtual bool OnEvent(const irr::SEvent& event)
        {
            bool result = false;

            if (event.EventType == irr::EET_GUI_EVENT)
            {
                irr::s32 id = event.GUIEvent.Caller->getID();
                //irr::gui::IGUIEnvironment *gui = device->getGUIEnvironment();


                switch (event.GUIEvent.EventType)
                {
                case irr::gui::EGET_BUTTON_CLICKED:
                    gui_button_pressed = true;
                    result = true;
                    break;

                default:
                    break;
                }

                last_gui_id = id;
            }


            return result;
        }


        irr::s32 whatId()
        {
            return last_gui_id;
        }

        bool buttonPress()
        {
            return gui_button_pressed;
        }

        void reset()
        {
            last_gui_id = -1;
            gui_button_pressed = false;
        }

    };
}

#endif // EVENTHANDLER_H_INCLUDED
