#ifndef __BRUSH_H__
#define __BRUSH_H__

#include "ofColor.h"
#include "ofVec3f.h"
#include "ofGraphics.h"

#include "Globals.h"

#define BRUSH_DOT_RADIUS 0.2, 1.8
#define BRUSH_POS_OSC_RANGE -1.6, 0.7
#define BRUSH_RADIUS_OSC_RANGE -0.04, 0.015

class Brush
{
    protected:
        struct Dot
        {
            ofVec2f basePos;
            ofVec2f pos;

            ofColor color;

            float baseRadius;
            float radius;
            float dieSpeed;

            Dot (const ofVec2f &pos, ofColor col, float rad, float dSpeed)
                : basePos(pos), pos(pos),
                  color(col), 
                  baseRadius(rad), radius(rad),
                  dieSpeed(dSpeed)
            {
            }

            void sweep(const ofVec2f &newpos)
            {
            }
        };

        typedef std::vector<Dot> DotList;

        DotList _dots;
        ofVec2f _pos;
        ofVec2f _vel;

        float _currDist;
        float _totalDist;

    public:
        // specified dot size
		Brush(const ofVec2f &pos, const ofVec2f &vel, ofColor color, float radius, float density,
            float dist, float fuzziness, float dotRadius = 1)
            : _pos(pos), _vel(vel), _totalDist(dist), _currDist(0)
        {
            // scale density by area
            int count = density * radius*radius;

            float angle;
            while (count--) {
                angle = ofRandom(0, 2 * PI);

                _dots.push_back(Dot(ofRandom(0, radius) * ofVec2f(cos(angle), sin(angle)),
                            color * (1.0 + ofRandom(-0.2, 0.2)),
                            //ofColor::fromHsb(ofRandom(0, 255),
                                //ofRandom(200, 255),
                                //ofRandom(128, 190)),
                            ofRandom(0.4 * dotRadius, 1.6 * dotRadius),
                            ofRandom(1, fuzziness)));
            }
		}

        ofVec2f getPosition()
        {
            return _pos;
        }

        // return false if destroy, true if continue
        bool move(const ofVec2f &force, float dt)
        {
            _vel += force*dt;
            ofVec2f disp = _vel*dt;
            float dispLen = disp.length();

            _currDist += dispLen;
            if (_currDist >= _totalDist) /**/
                return false;

            _pos += disp;

            float alphaDisp = dispLen/_totalDist*255;
            for (DotList::iterator d = _dots.begin();
                    d != _dots.end(); ++d)
                d->color.a =  ofClamp(d->color.a - (d->dieSpeed * alphaDisp), 0, 255);

            return true;
        }

        void draw()
        {
            ofEnableAlphaBlending();

            for (DotList::iterator d = _dots.begin();
                    d != _dots.end(); ++d)
            {
                ofSetColor(d->color);
                ofCircle(_pos + d->pos, d->radius);

                // oscillate dot radius, position
                if (d->radius >= d->baseRadius)
                    d->radius += ofRandom(BRUSH_RADIUS_OSC_RANGE);
                else
                    d->radius -= ofRandom(BRUSH_RADIUS_OSC_RANGE);

                if (d->pos.x >= d->basePos.x)
                    d->pos.x += ofRandom(BRUSH_POS_OSC_RANGE);
                else
                    d->pos.x -= ofRandom(BRUSH_POS_OSC_RANGE);

                if (d->pos.y >= d->basePos.y)
                    d->pos.y += ofRandom(BRUSH_POS_OSC_RANGE);
                else
                    d->pos.y -= ofRandom(BRUSH_POS_OSC_RANGE);
            }

            ofDisableAlphaBlending();
        }
};

#endif
