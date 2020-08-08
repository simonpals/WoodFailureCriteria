#include "timberhelper.h"

float goldenblatKopnov(float sigma1, float sigma2, SWoodParams s)
{
    float srt = s.strength.xt;
    float src = s.strength.xc;
    float stt = s.strength.yt;
    float stc = s.strength.yc;
    float srt45 = s.strength.s45;
    float srt_45 = 1./((1./stt-1./stc)-(1./srt-1./src)+1./srt45);

    return powf(0.5*(1./srt-1./src)*sigma1+0.5*(1./stt-1./stc)*sigma2,1.0) + powf(0.25*powf(1./srt+1./src,2.0)*powf(sigma1,2.0)+0.25*powf(1./stt+1./stc,2.0)*powf(sigma2,2.0) + 0.125*(powf(1./srt+1./src,2.0)+powf(1./stt+1./stc,2.0)-powf(1./srt45+1./srt_45,2.0))*sigma1*sigma2,0.5) - 1.0; // GK
}

float zakharovCriteria(float sigma1, float sigma2, SWoodParams s)
{
    float srt = s.strength.xt;
    float src = s.strength.xc;
    float stt = s.strength.yt;
    float stc = s.strength.yc;
    float srt45 = s.strength.s45;

    return powf((1./srt-1./src)*sigma1+(1./stt-1./stc)*sigma2,1.0) + powf(1./(srt*src)*powf(sigma1,2.0)+1./(stt*stc)*powf(sigma2,2.0) + 2.0 * 0.5*(1./(srt*src)+1./(stt*stc) + (-1./(srt45))*(1./srt-1./src-1./stt+1./stc)-1./powf(srt45,2.0))*sigma1*sigma2,1.0) - 1.0; // Zah
}

float misesHillCriteria(float sigma1, float sigma2, SWoodParams s)
{
    float srt = s.strength.xt;
    float src = s.strength.xc;
        float stt = s.strength.yt;
        float stc = s.strength.yc;
    float srt45 = s.strength.s45;
    float srt_45 = 1./((1./srt-1./src)-(1./stt-1./stc)+1./srt45);

    return powf(0.25*powf(1./srt+1./src,2.0)*powf(sigma1,2.0)+0.25*powf(1./stt+1./stc,2.0)*powf(sigma2,2.0) + 0.125*(powf(1./srt+1./src,2.0)+powf(1./stt+1./stc,2.0)-powf(1./srt45+1./srt_45,2.0))*sigma1*sigma2,1.0) - 1.0; // MH
}

float mallmeisterCriteria(float sigma1, float sigma2, SWoodParams s)
{
    float srt = s.strength.xt;
    float src = s.strength.xc;
    float stt = s.strength.yt;
    float stc = s.strength.yc;
    float srt45 = s.strength.s45;
    float srt_45 = 1./((1./stt-1./stc)-(1./srt-1./src)+1./srt45);

    return powf(0.5*(1./srt-1./src)*sigma1+0.5*(1./stt-1./stc)*sigma2,1.0) + powf(0.25*powf(1./srt+1./src,2.0)*powf(sigma1,2.0)+0.25*powf(1./stt+1./stc,2.0)*powf(sigma2,2.0) + 0.125*(powf(1./srt+1./src,2.0)+powf(1./stt+1./stc,2.0)-powf(1./srt45+1./srt_45,2.0))*sigma1*sigma2,1.0) - 1.0; // ML
}

float ashkenaziCriteria(float sigma1, float sigma2, SWoodParams s)
{
    float srt = s.strength.xt;
    float src = s.strength.xc;
    float stt = s.strength.yt;
    float stc = s.strength.yc;
    float srt45 = s.strength.s45;

    float sig90 = 2.0;
    float sig45 = 4.8;
    float sig0 = 4.8;
    float t0 = 3.0;

    if(s.species!="Сосна")
    {
        sig0 = srt;
        sig90 = stt;
    }

    return powf(sigma1,4.0)/powf(sig0,2.0) + powf(sigma2,4.0)/powf(sig90,2.0) + powf(sigma1,2.0)*powf(sigma2,2.0) * (2.0/(sig0*sig90) + powf((4.0/sig45 - 1.0/sig0 - 1.0/sig90 - 1.0/t0),2.0)) + 2.0*sigma1*sigma2 * (powf(sigma1,2.0)/sig0 + powf(sigma2,2.0)/sig0) * (4.0/sig45 - 1.0/sig0 - 1.0/sig90 - 1.0/t0) - powf(sigma1,2.0) - powf(sigma2,2.0) - sigma1 * sigma2;
    //return 1./srt*powf(sigma1,2.0)+1./stt*powf(sigma2,2.0) + (1./srt+1./stt-1./srt45)*sigma1*sigma2 - powf(sigma1*sigma1+sigma2*sigma2+sigma1*sigma2,0.5); // Ash
}

SWoodParams convertStrength(const SWoodParams &s, float temperature, float humidity, bool isConifers)
{
    SWoodParams sn=s;

    float c=1.0;
    float t=1.0;

    switch ((int)humidity) { //Ugolev
    case 5:
        c = (isConifers ? 1.24 : 1.305);
        t = (isConifers ? 1.04 : 1.11);
        break;
    case 10:
        c = (isConifers ? 1.07 : 1.08);
        t = (isConifers ? 1.015 : 1.035);
        break;
    case 15:
        c = (isConifers ? 0.9 : 0.895);
        t = (isConifers ? 0.98 : 0.955);
        break;
    case 20:
        c = (isConifers ? 0.735 : 0.75);
        t = (isConifers ? 0.93 : 0.885);
        break;
    case 25:
        c = (isConifers ? 0.57 : 0.655);
        t = (isConifers ? 0.855 : 0.815);
        break;
    }

    sn.strength.xc = (s.strength.xc*10.1972 - s.strength.beta*(temperature-20.0)) / 10.1972 * c;
    sn.strength.yc = (s.strength.yc*10.1972 - s.strength.beta*(temperature-20.0)) / 10.1972 * c;

    sn.strength.xt = (s.strength.xt*10.1972 - s.strength.beta*(temperature-20.0)) / 10.1972 * t;
    sn.strength.yt = (s.strength.yt*10.1972 - s.strength.beta*(temperature-20.0)) / 10.1972 * t;

    sn.strength.s45 = (s.strength.s45*10.1972 - s.strength.beta*(temperature-20.0)) / 10.1972 * t;

    return sn;
}

SWoodParams stiffnessList(QString species, float temperature, float humidity)
{
    bool standartExist = false;
    SWoodParams s;
    bool isConifers=true;

    for(SWoodParams wood : lsWoodParams)
    {
        if(wood.species==species)
        {
            if(wood.temperature==temperature && wood.humidity==humidity)
                return wood;
            else if(wood.temperature==20 && wood.humidity==12) // standart values
            {
                standartExist = true;
                s = wood;
                isConifers = wood.isConifers;
            }
        }
    }

    if(standartExist)
    {
        return convertStrength(s, temperature, humidity, isConifers);
    }

    Q_ASSERT(0);
    return SWoodParams();
}

void optimizeCriteria(const SWoodParams &strengthValues, float minG1, float maxG1,
                      float minG2, float maxG2, float delta, criteriaCB criteria,
                      QMultiMap<float,float> &outXY)
{
    Q_ASSERT(strengthValues.strength.xc>0.0);

    for(float x1=minG1; x1<=maxG1; x1+=delta)
    {
        float f1 = criteria(x1, minG2, strengthValues);
        for(float x2=minG2+delta; x2<=maxG2; x2+=delta)
        {
            float f2 = criteria(x1, x2, strengthValues);

            if(signbit(f1)!=signbit(f2) || std::min(fabs(f1),fabs(f2))==0.0)
            {
                outXY.insert(x2-(fabs(f1)<fabs(f2)?delta:0), x1);
            }

            f1 = f2;
        }
    }
}

void findRoots(const SWoodParams &strengthValues, float minG1, float maxG1,
                      float minG2, float maxG2, float delta, criteriaCB criteria,
                      float &G1min, float &G1max, float &G2min, float &G2max)
{
    Q_ASSERT(strengthValues.strength.xc>0.0);

    QVector<float> roots;
    float x1=0.0;
    float f1 = criteria(x1, minG2, strengthValues);
    for(float x2=minG2+delta; x2<=maxG2; x2+=delta)
    {
        float f2 = criteria(x1, x2, strengthValues);

        if(signbit(f1)!=signbit(f2) && std::min(fabs(f1),fabs(f2))!=0.0)
        {
            roots.push_back(x2-(fabs(f1)<fabs(f2)?delta:0));
        }

        f1 = f2;
    }

    G2min = std::min(roots[0], roots[1]);
    G2max = std::max(roots[0], roots[1]);
    roots.clear();

    float x2=0.0;
    f1 = criteria(minG1, x2, strengthValues);
    for(float x1=minG1+delta; x1<=maxG1; x1+=delta)
    {
        float f2 = criteria(x1, x2, strengthValues);

        if(signbit(f1)!=signbit(f2) && std::min(fabs(f1),fabs(f2))!=0.0)
        {
            roots.push_back(x1-(fabs(f1)<fabs(f2)?delta:0));
        }

        f1 = f2;
    }

    G1min = std::min(roots[0], roots[1]);
    G1max = std::max(roots[0], roots[1]);
    roots.clear();
}

void criteriaValuesToGrid(const SWoodParams &strengthValues, float minG1, float maxG1, float minG2, float maxG2, float delta, criteriaCB criteria, QVector<float> &out, int &colCount, int &rowCount)
{
    Q_ASSERT(strengthValues.strength.xc>0.0);

    for(float x1=minG1; x1<=maxG1; x1+=delta)
    {
        for(float x2=minG2; x2<=maxG2; x2+=delta)
        {
            float f = criteria(x1, x2, strengthValues);
            out.push_back(f);
        }
    }

    colCount = (maxG2-minG2) / delta + 1;
    rowCount = (maxG1-minG1) / delta + 1;
}
