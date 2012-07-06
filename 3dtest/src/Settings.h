#ifndef __SETTINGS_H__
#define __SETTINGS_H__

// defaults

#define BRUSH_STEP_TIME 0.005
#define FIELD_MAGNITUDE 100
#define BRUSH_GRID_STEP 7
#define RADIUS 5
#define DIST 60
#define DENSITY 0.3
#define FUZZINESS 7

#define BRUSH_DOT_RADIUS 0.1, 1.2
#define BRUSH_POS_OSC_RANGE -1.6, 0.7
#define BRUSH_RADIUS_OSC_RANGE -0.04, 0.015

// settings

namespace Settings
{
    template <class T>
    class Setting
    {
        std::string description;
        T value;

        Setting(const T &val, const char *desc)
            : description(desc), value(val)
        {
        }
    }

    float brushStepTime = Setting<float>(0.005,
            "Time to step brush simulation by each brush draw frame");
    float fieldMagnitude = Setting<float>(100,
            "Length of force field vectors");
    int brushGridStep = Setting<int>(7,
            "Horizontal and vertical space between brush points");
    float brushRadius = Setting<float>(5,
            "Size of each brush stroke");
    float brushLength = Setting<float>(60,
            "Length of each brush stroke");
    float brushDensity = Setting<float>(0.3,
            "Number of strokes per unit area");
    float brushFuzziness = Setting<float>(7,
            "Fuzziness of brush stroke ends");

    ofVec2f brushDotRadius = Setting<ofVec2f>(ofVec2f(0.1, 1.2),
            "Range of brush hair sizes");
    ofVec2f brushDotPosOscRange = Setting<ofVec2f>(ofVec2f(-1.6, 0.7),
            "Range of brush hair position oscillations");
    ofVec2f brushDotRadOscRange = Setting<ofVec2f(ofVec2f(-0.04, 0.015),
            "Range of brush hair size oscillations");
};

#endif
