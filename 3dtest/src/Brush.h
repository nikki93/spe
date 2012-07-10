#ifndef __BRUSH_H__
#define __BRUSH_H__

#include "ofColor.h"
#include "ofVec3f.h"
#include "ofGraphics.h"

#include "Random.h"

#define BRUSH_DOT_RADIUS 0.1, 1.2
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
        };

        typedef std::vector<Dot> DotList;

        DotList _dots;
        ofVec2f _pos;
        ofVec2f _vel;

        float _currDist;
        float _totalDist;

        Random _rand;

    public:
        Brush(const ofVec2f &pos, const ofVec2f &vel, ofColor color, float radius, float dist = 100, 
                float density = 0.1, float fuzziness = 5, float grain = 0.4)
            : _pos(pos), _vel(vel), _totalDist(dist), _currDist(0),
              _rand(pos.x + pos.y)
        {
            // scale density by area
            int count = density * radius*radius;
            color.a = 255;

            while (count--)
                _dots.push_back(Dot(ofVec2f(_rand(-radius, radius), _rand(-radius, radius)),
                            color * (1.0 + _rand(-grain, grain)),
                            _rand(BRUSH_DOT_RADIUS),
                            _rand(1, fuzziness)));
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
            if (_currDist >= _totalDist)
                return false;

            _pos += disp;

            float alphaDisp = dispLen/_totalDist*255;
            for (DotList::iterator d = _dots.begin();
                    d != _dots.end(); ++d)
                d->color.a = ofClamp(d->color.a - (d->dieSpeed * alphaDisp), 0, 255);

            return true;
        }

        void draw()
        {
            for (DotList::iterator d = _dots.begin();
                    d != _dots.end(); ++d)
            {
                ofSetColor(d->color);
                ofCircle(_pos + d->pos, d->radius);

                // oscillate dot radius, position
                if (d->radius >= d->baseRadius)
                    d->radius += _rand(BRUSH_RADIUS_OSC_RANGE);
                else
                    d->radius -= _rand(BRUSH_RADIUS_OSC_RANGE);

                if (d->pos.x >= d->basePos.x)
                    d->pos.x += _rand(BRUSH_POS_OSC_RANGE);
                else
                    d->pos.x -= _rand(BRUSH_POS_OSC_RANGE);

                if (d->pos.y >= d->basePos.y)
                    d->pos.y += _rand(BRUSH_POS_OSC_RANGE);
                else
                    d->pos.y -= _rand(BRUSH_POS_OSC_RANGE);
            }
        }
};

#endif
