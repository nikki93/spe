#ifndef __FIELD_H__
#define __FIELD_H__

#include <ofVec2f.h>

class Field
{
    protected:
        size_t _w, _h;
        ofVec2f *_arr;
        float _max; // maximum magnitude

    public:
        ~Field()
        {
            delete _arr;
        }

        // a function (x, y) -> R^2 that gives field at (x, y)
        struct Function { virtual ofVec2f fieldAt(size_t x, size_t y) = 0; };

        // initialize with a function
        Field(size_t w, size_t h, Function *f)
            : _w(w), _h(h), _max(0)
        {
            _arr = new ofVec2f[w*h];

            for (size_t i = 0; i < _w; ++i)
                for (size_t j = 0; j < _h; ++j)
                    set(i, j, f->fieldAt(i, j));
        }

        // uninitialized field
        Field(size_t w, size_t h)
            : _w(w), _h(h), _max(0)
        {
            _arr = new ofVec2f[w*h];
        }

        // set

        void set(size_t x, size_t y, const ofVec2f &v)
        {
            _arr[_w*y + x] = v;
            _max = std::max(_max, v.length());
        }
        void set(const ofVec2f &pos, const ofVec2f &v)
        {
            set(pos.x, pos.y, v);
        }
        void clear()
        {
            for (size_t i = 0; i < _w; ++i)
                for (size_t j = 0; j < _h; ++j)
                    _arr[_w*j + i] = ofVec2f(0, 0);
            _max = 0;
        }

        // get

        ofVec2f get(size_t x, size_t y) const
        {
            return _arr[_w*y + x];
        }
        size_t width()
        {
            return _w;
        }
        size_t height()
        {
            return _h;
        }

        // draw field vector directions as a grid
        void draw(const ofVec2f &pos, size_t gridSize)
        {
            float len = 0.9*gridSize;

            for (size_t i = 0; i < _w; i += gridSize)
                for (size_t j = 0; j < _h; j += gridSize)
                {
                    ofVec2f start(pos.x + i, pos.y + j);

                    ofLine(start, start + len*_arr[_w*j + i]/_max);
                    ofCircle(start, 0.8);
                }
        }
};

#endif
