#include "BrushStyler.h"

bool BrushStyler::containsEdge(int x, int y, float threshold, float target) {
    int lower, upper;
    for (int r = 0; r <= target; r++) {
        lower = std::max(0, y - r);
        upper = std::min(y + r, 768);
        // left
        if (x - r >= 0)
            for (int j = lower; j <= upper; j++)
                if (_edges.getColor(x - r, j).r >= threshold)
                    return true;
        // right
        if (x + r <= 1023)
            for (int j = lower; j <= upper; j++)
                if (_edges.getColor(x + r, j).r >= threshold)
                    return true;
        lower = std::max(0, x - r);
        upper = std::min(x + r, 1023);
        // top
        if (y - r >= 0)
            for (int j = lower; j <= upper; j++)
                if (_edges.getColor(j, y - r).r >= threshold)
                    return true;
        // bottom
        if (y + r <= 767)
            for (int j = lower; j <= upper; j++)
                if (_edges.getColor(j, y + r).r >= threshold)
                    return true;
    }
    return false;
}

BrushStyler::BrushStyler(ofImage &img, ofImage &edges, Field &field, Palette &palette)
    : _edges(edges), _img(img), _forceField(field), _palette(palette) {
    _brushInd = -1;
    _iterations = 0;
}

void BrushStyler::deleteBrushes() {
    
    // remove brushes
    int i = _brushes.size() - 1;
    while (!_brushes.empty())
    {
        while (!_brushes[i].empty()) { // OUT OF BOUNDS????
            delete _brushes[i].back();
            _brushes[i].pop_back();
        }
        _brushes.pop_back();
        i--;
    }

    _brushInd = -1;
    _iterations = 0;
}

void BrushStyler::drawBrushes () {
    if (_brushInd >= 0)
        for (BrushList::iterator i = _brushes[_brushInd].begin();
                i != _brushes[_brushInd].end(); ++i)
            (*i)->draw();
}

void BrushStyler::moveBrushes() {
    bool allDead = true;

    // move brushes by force field, remove brush if done
    if (_brushInd < _brushes.size()) {
        for (BrushList::iterator iter = _brushes[_brushInd].begin();
                iter != _brushes[_brushInd].end(); )
        {
            Brush *brush = *iter;
            ofVec2f pos = brush->getPosition();
            int i = ofClamp(pos.x, 0, 1023);
            int j = ofClamp(pos.y, 0, 767);
            if (!brush->move(_forceField.get(i, j), 0.005))
            {
                delete brush;
                iter = _brushes[_brushInd].erase(iter);
            }
            else {
                ++iter;
                allDead = false;
            }
        }
        _iterations++;
        if ((_iterations > 200 || allDead) && _brushInd < _brushes.size() - 1) {
            _brushInd++;
            _iterations = 0;
        }
    }
}

void BrushStyler::styleBrushes() {
    
    // put in the gui
    float minRad = 2, maxRad = 30; 
    int levels = 3;
    float threshold = 240.0;
    float dist = 400;
    float density = 0.1;
    float fuzziness = 2;
    float interceptability = 0.7; // ^_^

    int gridStep = 0.8 * maxRad;
    float brushStep;

    ofPixels &pix = _img.getPixelsRef();

    // get brush step
    if (maxRad - minRad == 0)
        brushStep = 1;
    else
        brushStep = 1.0 * (maxRad - minRad) / (levels - 1);
    _brushes.resize(levels + 1); // + 1 is for the coarse layer

    // filledness array for brush placement
    bool filled[768][1024];
    for (int y = 0; y < 768; y++)
        for (int x = 0; x < 1024; x++)
            filled[y][x] = false;
    
    // coarse layer
    /*int coarseRad = maxRad * 1.2;
    gridStep = coarseRad * interceptability;
    for (int y = 0; y < 768; y += gridStep) {
        for (int x = 0; x < 1024; x += gridStep) {
            brushes[0].push_back(new Brush(ofVec2f(x, y), ofVec2f(0, 0), 
                _palette.getClosest(pix.getColor(x, y)), 
                coarseRad, density, dist * 2, fuzziness));
        }
    }*/
    
    // normal layers
    float target;
    int i;
    int curr;
    for (i = 1; i < levels; i++) {
        curr = (maxRad - (i - 1) * brushStep);
        target = curr * 0.7;
        gridStep = curr * interceptability;
        for (int y = 0; y < 768; y += gridStep) {
            for (int x = 0; x < 1024; x += gridStep) {
                if ((i == 1 || !filled[y][x]) && !containsEdge(x, y, threshold, target)) {
                    _brushes[i].push_back(new Brush(ofVec2f(x, y), ofVec2f(0, 0), 
                        _palette.getClosest(pix.getColor(x, y)), 
                        curr, density, dist, fuzziness));

                    // update filledness
                    int tmp = std::min((int)(y + curr * interceptability), 767);
                    int tmp2;
                    for (int m = std::max((int)(y - curr * interceptability), 0); m <= tmp; m++) {
                        tmp2 = std::min((int)(x + curr * interceptability), 1023);
                        for (int n = std::max((int)(x - curr * interceptability), 0); n <= tmp2; n++)
                            filled[m][n] = true;
                    }
                }
            }
        }
    }

    gridStep = maxRad * interceptability;
    for (int y = 0; y < 768; y += gridStep) {
        for (int x = 0; x < 1024; x += gridStep) {
            if (!filled[y][x])
                _brushes[0].push_back(new Brush(ofVec2f(x, y), ofVec2f(0, 0), 
                    _palette.getClosest(pix.getColor(x, y)), 
                    maxRad, density, dist * 2, fuzziness));
        }
    }


    
    // fine layer
    gridStep = std::max(1, (int)(minRad * interceptability));
    for (int y = 0; y < 768; y += gridStep)
        for (int x = 0; x < 1024; x += gridStep)
            if (_edges.getColor(x, y).r >= threshold)
                _brushes[i].push_back(new Brush(ofVec2f(x, y), ofVec2f(0, 0), 
                    _palette.getClosest(pix.getColor(x, y)), minRad, density*minRad>=1?density:1, 
                    dist * 1.2, fuzziness * 0.5));

    _brushInd = 0;
}
