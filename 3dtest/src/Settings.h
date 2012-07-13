#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <ofVec2f.h>

// settings

class Settings
{
    public:
        static float brushStepTime;
        static float fieldMagnitude;

        static float brushBigRadius;
        static float brushFineRadius;
        static float brushSizeMultiplier;
        static float brushLevels;
        static float brushCloseness;
        static float brushPaletteSize;
        static bool brushPaletteXYZ;

        static float brushLength;
        static float minBrushDensity;
        static float maxBrushDensity;
        static float brushFuzziness;
        static float brushGrain;
        static float blurSize;

        static ofVec2f brushDotRadius;
        static ofVec2f brushDotPosOscRange;
        static ofVec2f brushDotRadOscRange;



        static bool autoUpdate;
        static float updateTime;
};

#endif
