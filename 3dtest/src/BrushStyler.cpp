#include "BrushStyler.h"

#include "Settings.h"
#include "Palette.h"

// red value lower than this means edge
#define EDGE_THRESHOLD 128

// whether there's an edge in a square of side 2*radius centered at x, y
bool BrushStyler::containsEdge(int x, int y, float radius) {
    for (int r = 0; r <= radius; r++) {
        int lower = std::max(0, y - r), upper = std::min(y + r, 767);
        // left
        if (x - r >= 0)
            for (int j = lower; j <= upper; j++)
                if (_edges.getColor(x - r, j).r <= EDGE_THRESHOLD)
                    return true;
        // right
        if (x + r <= 1023)
            for (int j = lower; j <= upper; j++)
                if (_edges.getColor(x + r, j).r <= EDGE_THRESHOLD)
                    return true;

        lower = std::max(0, x - r);
        upper = std::min(x + r, 1023);
        // top
        if (y - r <= 0)
            for (int j = lower; j <= upper; j++)
                if (_edges.getColor(j, y - r).r <= EDGE_THRESHOLD)
                    return true;
        // bottom
        if (y + r <= 767)
            for (int j = lower; j <= upper; j++)
                if (_edges.getColor(j, y + r).r <= EDGE_THRESHOLD)
                    return true;
    }

    return false;
}

BrushStyler::BrushStyler(ofPixels &img, ofPixels &edges, Field &field)
    : _edges(edges), _img(img), _forceField(field), _brushInd(-1),
      _iterations(0) {
}

void BrushStyler::clear() {
    // remove brushes
    while (!_brushes.empty())
    {
        BrushList &l = _brushes.back();
        while (!l.empty()) {
            delete l.back();
            l.pop_back();
        }
        _brushes.pop_back();
    }

    _brushInd = -1;
    _iterations = 0;
}

void BrushStyler::draw() {
    /*
    int total = _brushes.size();
    ofColor start(0xff0000), end(0x0000ff);
    for (int i = 0; i < total; ++i)
        for (BrushList::iterator j = _brushes[i].begin();
                j != _brushes[i].end(); ++j)
        {
            ofSetColor(start.getLerped(end, ((float) i) / total));
            ofCircle((*j)->getPosition(), 1);
        }
    return;
    */

    if (_brushInd >= 0 && _brushInd < _brushes.size())
        for (BrushList::iterator i = _brushes[_brushInd].begin();
                i != _brushes[_brushInd].end(); ++i)
            (*i)->draw();
}

bool BrushStyler::move() {
    //return true;
    bool allDead = true;

    // move brushes by force field, remove brush if done
    if (_brushInd < _brushes.size()) {
        for (BrushList::iterator iter = _brushes[_brushInd].begin();
                iter != _brushes[_brushInd].end(); ) {
            Brush *brush = *iter;
            ofVec2f pos = brush->getPosition();
            int i = ofClamp(pos.x, 0, 1023);
            int j = ofClamp(pos.y, 0, 767);
            if (!brush->move(_forceField.get(i, j), Settings::brushStepTime)) {
                delete brush;
                iter = _brushes[_brushInd].erase(iter);
            }
            else {
                ++iter;
                allDead = false;
            }
        }

        _iterations++;
        if (_iterations > 200 || allDead) {
            _brushInd++;
            _iterations = 0;
        }

        return true;
    }
    else
        return false;
}

void BrushStyler::generate() {
    int levels = Settings::brushLevels;

    // seed
    int seed = 0; // FIXME: use a more legitimate way to generate seeds

    // coarse layer is extra
    _brushInd = 0;
    _brushes.resize(levels + 1);

    // generate palette
    PaletteBase *palette;
    if (Settings::brushPaletteXYZ)
        palette = new Palette<ColorXYZ, float>(_img, Settings::brushPaletteSize);
    else
        palette = new Palette<ofColor, unsigned char>(_img, Settings::brushPaletteSize);

    // filledness array for brush intersection testing
    bool filled[768][1024] = { false };

    // normal layers
    float curr = Settings::brushBigRadius;
    ofColor col;
    for (int i = 0; i < levels; i++) {
        float target = curr * Settings::brushCloseness;

        for (int y = 0; y < 768; ++y)
            for (int x = 0; x < 1024; ++x)
                if (!filled[y][x] // not previously filled
                        && (col = _img.getColor(x, y)).a < 200 // not high-depth (background)
                        && !containsEdge(x, y, target)) { // doesn't contain edge
                    _brushes[i].push_back(new Brush(ofVec2f(x, y), ofVec2f(0, 0), 
                                palette->getClosest(col), curr, seed++,
                                Settings::brushLength, 
                                Settings::minBrushDensity + 
                                        (((float) i)/levels)*(Settings::maxBrushDensity - Settings::minBrushDensity), 
                                Settings::brushFuzziness, Settings::brushGrain));

                    // update filledness
                    int tmp = std::min((int)(y + target), 767);
                    for (int m = std::max((int)(y - target), 0); m <= tmp; m++) {
                        int tmp2 = std::min((int)(x + target), 1023);
                        for (int n = std::max((int)(x - target), 0); n <= tmp2; n++)
                            filled[m][n] = true;
                    }
                }

        curr *= Settings::brushSizeMultiplier; // next one's smaller
    }

    // fine
    for (int y = 0; y < 768; y++)
        for (int x = 0; x < 1024; x++)
            filled[y][x] = false;

    for (int y = 0; y < 768; ++y)
        for (int x = 0; x < 1024; ++x)
            if (!filled[y][x] && _edges.getColor(x, y).r <= EDGE_THRESHOLD)
            {
                _brushes[levels].push_back(new Brush(ofVec2f(x, y), ofVec2f(0, 0), 
            /* darker */    ofColor(128, 128, 128, 1)*palette->getClosest(_img.getColor(x, y)), 
                            Settings::brushFineRadius, seed++,
                            Settings::brushLength, 
                            0.1 + 1/(Settings::brushFineRadius*Settings::brushFineRadius),
                            Settings::brushFuzziness, Settings::brushGrain,
            /* smaller */   ofVec2f(0.1, 0.8)));

                // update filledness
                int tmp = std::min((int)(y + 3*Settings::brushFineRadius), 767);
                for (int m = std::max((int)(y - 3*Settings::brushFineRadius), 0); m <= tmp; m++) {
                    int tmp2 = std::min((int)(x + 3*Settings::brushFineRadius), 1023);
                    for (int n = std::max((int)(x - 3*Settings::brushFineRadius), 0); n <= tmp2; n++)
                        filled[m][n] = true;
                }
            }

    delete palette;
}

