#include "Settings.h"

// defaults

float  Settings::brushStepTime = 0.005;
float  Settings::fieldMagnitude = 100;
float  Settings::brushGridStep = 7;
float  Settings::brushRadius = 5;
float  Settings::brushLength = 60;
float  Settings::brushDensity = 0.3;
float  Settings::brushFuzziness = 7;
float  Settings::brushGrain = 0.4;
float  Settings::blurSize = 5;

ofVec2f  Settings::brushDotRadius = ofVec2f(0.1, 1.2);
ofVec2f  Settings::brushDotPosOscRange = ofVec2f(-1.6, 0.7);
ofVec2f  Settings::brushDotRadOscRange = ofVec2f(-0.04, 0.015);

bool Settings::autoUpdate = false;
float Settings::updateTime = 150;

