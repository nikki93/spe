#ifndef __BALL_H__
#define __BALL_H__

#include <ofGraphics.h>

class Ball
{
    protected:
        ofVec3f _pos;
        ofVec3f _vel;
        ofVec3f _acc;

        ofColor _col;

        float _radius;

    public:
        Ball(ofVec3f pos, ofVec3f vel, ofVec3f acc, ofColor col, float radius)
            : _pos(pos), _vel(vel), _acc(acc), _col(col), _radius(radius)
        {
        }

        void update(float elapsed)
        {
            // integrate
            _vel += _acc * elapsed;
            _pos += _vel * elapsed;

            // bounce
            if (_pos.y <= _radius)
            {
                _pos.y = _radius;
                _vel.y = fabs(_vel.y);
            }
        }

        void draw()
        {
            // ball
            ofSetColor(_col);
            ofSphere(_pos, _radius);
        }
};

#endif
