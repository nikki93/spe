#ifndef __RANDOM_H__
#define __RANDOM_H__

class Random
{
    protected:
        unsigned int s1, s2, s3, s4;

    public:
        Random(unsigned int s)
        {
            if (s <= 127)
                s += 234;
            s1 = s2 = s3 = s4 = s;
        }

        float unit()
        {
           unsigned int b;
           b  = ((s1 << 6) ^ s1) >> 13;
           s1 = ((s1 & 4294967294UL) << 18) ^ b;
           b  = ((s2 << 2) ^ s2) >> 27;
           s2 = ((s2 & 4294967288UL) << 2) ^ b;
           b  = ((s3 << 13) ^ s3) >> 21;
           s3 = ((s3 & 4294967280UL) << 7) ^ b;
           b  = ((s4 << 3) ^ s4) >> 12;
           s4 = ((s4 & 4294967168UL) << 13) ^ b;
           return (s1 ^ s2 ^ s3 ^ s4) * 2.3283064365386963e-10;
        }

        float operator()()
        {
            return unit();
        }
        float operator()(float x, float y)
        {
            return x + unit()*(y - x);
        }
};

#endif
