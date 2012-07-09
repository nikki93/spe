#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <ofVec2f.h>

// settings

class Settings
{
    public:
        static float brushStepTime;
        static float fieldMagnitude;
        static float brushGridStep;
        static float brushRadius;
        static float brushLength;
        static float brushDensity;
        static float brushFuzziness;

        static ofVec2f brushDotRadius;
        static ofVec2f brushDotPosOscRange;
        static ofVec2f brushDotRadOscRange;

        static bool autoUpdate;
        static float updateTime;
};

#endif
