#ifndef __GUI_H__
#define __GUI_H__

#include <ofxUI.h>

class GUI
{
    protected:
        ofxUICanvas _canvas;

    public:
        GUI()
            : _canvas(1024, 0, 300, 768)
        {
            _canvas.addWidgetDown(new ofxUILabel("3d brush test!", OFX_UI_FONT_LARGE));
        }

        ~GUI()
        {
            _canvas.saveSettings("settings.xml");
        }
};

#endif
