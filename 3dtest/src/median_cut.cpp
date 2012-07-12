/* Copyright (c) 2012 the authors listed at the following URL, and/or
   the authors of referenced articles or incorporated external code:
http://en.literateprograms.org/Median_cut_algorithm_(C_Plus_Plus)?action=history&offset=20080309133934

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Retrieved from: http://en.literateprograms.org/Median_cut_algorithm_(C_Plus_Plus)?oldid=12754
*/

#ifndef __MEDIAN_CUT_CPP__
#define __MEDIAN_CUT_CPP__

#include <limits>
#include <queue>
#include <algorithm>
#include "median_cut.h"

template<typename Color, typename ColorElement>
Block<Color, ColorElement>::Block(Color *points, int pointsLength)
{
    this->points = points;
    this->pointsLength = pointsLength;
    for(int i=0; i < 3; i++)
    {
        minCorner.v[i] = std::numeric_limits<ColorElement>::min();
        maxCorner.v[i] = std::numeric_limits<ColorElement>::max();
    }
}

template<typename Color, typename ColorElement>
Color *Block<Color, ColorElement>::getPoints()
{
    return points;
}

template<typename Color, typename ColorElement>
int Block<Color, ColorElement>::numPoints() const
{
    return pointsLength;
}

template<typename Color, typename ColorElement>
int Block<Color, ColorElement>::longestSideIndex() const
{
    ColorElement m = maxCorner.v[0] - minCorner.v[0];
    int maxIndex = 0;
    for(int i=1; i < 3; i++)
    {
        ColorElement diff = maxCorner.v[i] - minCorner.v[i];
        if (diff > m)
        {
            m = diff;
            maxIndex = i;
        }
    }
    return maxIndex;
}

template<typename Color, typename ColorElement>
ColorElement Block<Color, ColorElement>::longestSideLength() const
{
    int i = longestSideIndex();
    return maxCorner.v[i] - minCorner.v[i];
}

template<typename Color, typename ColorElement>
bool Block<Color, ColorElement>::operator<(const Block& rhs) const
{
    return this->longestSideLength() < rhs.longestSideLength();
}

template<typename Color, typename ColorElement>
void Block<Color, ColorElement>::shrink()
{
    int i,j;
    for(j=0; j<3; j++)
    {
        minCorner.v[j] = maxCorner.v[j] = points[0].v[j];
    }
    for(i=1; i < pointsLength; i++)
    {
        for(j=0; j<3; j++)
        {
            minCorner.v[j] = min(minCorner.v[j], points[i].v[j]);
            maxCorner.v[j] = max(maxCorner.v[j], points[i].v[j]);
        }
    }
}

template<typename Color, typename ColorElement>
std::vector<Color> medianCut(Color *image, int numPoints, unsigned int desiredSize)
{
    std::priority_queue<Block<Color, ColorElement> > blockQueue;
    Block<Color, ColorElement> initialBlock(image, numPoints);
    initialBlock.shrink();
    blockQueue.push(initialBlock);
    while (blockQueue.size() < desiredSize && blockQueue.top().numPoints() > 1)
    {
        Block<Color, ColorElement> longestBlock = blockQueue.top();

        blockQueue.pop();
        Color *begin  = longestBlock.getPoints();
        Color *median = longestBlock.getPoints() + (longestBlock.numPoints()+1)/2;
        Color *end    = longestBlock.getPoints() + longestBlock.numPoints();
        switch(longestBlock.longestSideIndex())
        {
            case 0: std::nth_element(begin, median, end, CoordinatePointComparator<Color, 0>()); break;
            case 1: std::nth_element(begin, median, end, CoordinatePointComparator<Color, 1>()); break;
            case 2: std::nth_element(begin, median, end, CoordinatePointComparator<Color, 2>()); break;
        }

        Block<Color, ColorElement> block1(begin, median-begin), block2(median, end-median);
        block1.shrink();
        block2.shrink();
        blockQueue.push(block1);
        blockQueue.push(block2);
    }
    std::vector<Color> result;
    while(!blockQueue.empty())
    {
        Block<Color, ColorElement> block = blockQueue.top();
        blockQueue.pop();
        Color *points = block.getPoints();

        double sum[3] = {0};
        for(int i=0; i < block.numPoints(); i++)
        {
            for(int j=0; j < 3; j++)
            {
                sum[j] += points[i].v[j];
            }
        }

        Color averagePoint;
        for(int j=0; j < 3; j++)
        {
            averagePoint.v[j] = sum[j] / block.numPoints();
        }

        result.push_back(averagePoint);
    }
    return result;
}

#endif
