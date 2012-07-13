#ifndef __GUI_H__
#define __GUI_H__

#include <ofxUI.h>

#include "Settings.h"

class GUI
{
    protected:
        ofxUICanvas _canvas;

        typedef std::map<std::string, float *> SliderMap;
        SliderMap _sliders; // maps slider name to float variable pointer

        App &_app;

    public:
        GUI(App &app)
            : _canvas(1024, 0, 300, 768),
              _app(app)
        {
            // canvas settings
            _canvas.setColorBack(ofColor::fromHex(0xbbcae8));
            _canvas.setDrawBack(true);
            _canvas.setColorOutline(ofColor::fromHex(0x202020));
            _canvas.setDrawOutline(true);
            ofAddListener(_canvas.newGUIEvent, this, &GUI::event);

            // title label
            ofxUILabel *title = new ofxUILabel("paintpaint", OFX_UI_FONT_LARGE);
            title->setColorFill(ofColor::black);
            _canvas.addWidgetDown(title);

            // brush settings
            addSlider("Brush step time", &Settings::brushStepTime, 0.0005, 0.01, 0.005);
            addSlider("Force field strength", &Settings::fieldMagnitude, 0, 700, 100);

            addSlider("Brush layers", &Settings::brushLevels, 1, 16, 6);
            addSlider("Large brush size", &Settings::brushBigRadius, 1, 160, 20);
            addSlider("Fine brush size", &Settings::brushFineRadius, 1, 16, 2);
            addSlider("Brush size factor", &Settings::brushSizeMultiplier, 0.05, 1, 0.7);

            addSlider("Brush closeness", &Settings::brushCloseness, 0.1, 1, 0.7);
            addSlider("Palette size", &Settings::brushPaletteSize, 1, 128, 28);
            _canvas.addWidgetDown(new ofxUILabelToggle(134, false, "CIE XYZ", OFX_UI_FONT_SMALL, 20));

            addSlider("Brush length", &Settings::brushLength, 5, 200, 60);
            addSlider("Min. brush hair density", &Settings::minBrushDensity, 0.01, 1, 0.1);
            addSlider("Max. brush hair density", &Settings::maxBrushDensity, 0.01, 1, 0.3);
            addSlider("Brush hair fuzziness", &Settings::brushFuzziness, 0, 15, 7);
            addSlider("Brush hair grain", &Settings::brushGrain, 0, 2, 0.4);

            // frame update
            _canvas.addWidgetDown(new ofxUILabelToggle(134, false, "Auto update", OFX_UI_FONT_SMALL, 20));
            _canvas.addWidgetRight(new ofxUILabelButton(134, false, "Next frame", OFX_UI_FONT_SMALL, 20));
            addSlider("Auto update timer", &Settings::updateTime, 0, 10, 1);
           
            // reload
            _canvas.loadSettings("settings.xml");
        }

        ~GUI()
        {
            _canvas.saveSettings("settings.xml");
        }

        void event(ofxUIEventArgs &e)
        {
            std::string name = e.widget->getName();

            if (name == "CIE XYZ")
                Settings::brushPaletteXYZ = dynamic_cast<ofxUIToggle *>(e.widget)->getValue();

            else if (name == "Auto update")
                Settings::autoUpdate = dynamic_cast<ofxUIToggle *>(e.widget)->getValue();

            else if (name == "Next frame")
                _app.newFrame();

            else
            {
                ofxUISlider *slider = dynamic_cast<ofxUISlider *>(e.widget);
                if (slider)
                    *(_sliders[name]) = slider->getScaledValue();
            }
        }

        void addSlider(const std::string &name, float *var, float min, float max, float def, float width = 280)
        {
            ofxUISlider *slider = new ofxUISlider(width, 10, min, max, *var = def, name);
            _canvas.addWidgetDown(slider);
            slider->setLabelPrecision(5);
            slider->setColorFill(ofColor::black);

            _sliders[name] = var;
        }
};

#endif
