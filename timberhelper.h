#ifndef TIMBERHELPER_H
#define TIMBERHELPER_H

#include <QList>

struct StrengthValues
{
    float xc; // radial compress (MPa or N/mm2)
    float yc; // tangential compress
    float xt; // radial tensile
    float yt; // tangential tensile
    float s45; // shear strength values at 45º
    float beta; //temperature koef
};

struct SWoodParams
{
    QString species;
    float temperature;
    float humidity;
    StrengthValues strength;
    bool isConifers;
};

typedef float (*criteriaCB)(float, float, SWoodParams);

struct SCriteriaParams
{
    QString criteriaName;
    float minSigma1;
    float maxSigma1;
    float minSigma2;
    float maxSigma2;
    criteriaCB criteriaMethod;
};

const QList<SWoodParams> lsWoodParams{
    {"Сосна",20.0,12.0,{5.1f,7.5f,3.23f,2.63f,2.7f,0.44f},true},
    {"Ялина",20.0,12.0,{3.9f,6.0f,5.0f,3.2f,3.0f,0.29f},true},
    {"Береза",20.0,12.0,{12.3f,6.5f,10.9f,6.9f,7.2f,0.59f},false}
};

float goldenblatKopnov(float sigma1, float sigma2, SWoodParams s);
float zakharovCriteria(float sigma1, float sigma2, SWoodParams s);
float misesHillCriteria(float sigma1, float sigma2, SWoodParams s);
float ashkenaziCriteria(float sigma1, float sigma2, SWoodParams s);
float mallmeisterCriteria(float sigma1, float sigma2, SWoodParams s);

const QList<SCriteriaParams> lsCriteriaParams {
    { "Ашкеназі", -10.0, 10.0, -10.0, 10.0, &ashkenaziCriteria },
    { "Гольленблата-Копнова", -10.0, 10.0, -10.0, 10.0, &goldenblatKopnov },
//    { "Мізеса-Хілла", -10.0, 10.0, -10.0, 10.0, &misesHillCriteria },
    { "Захарова", -10.0, 10.0, -10.0, 10.0, &zakharovCriteria },
    { "Малмейстера", -10.0, 10.0, -10.0, 10.0, &mallmeisterCriteria }
};

SWoodParams convertStrength(const SWoodParams &s, float temperature, float humidity, bool isConifers);
SWoodParams stiffnessList(QString species, float temperature, float humidity);

void optimizeCriteria(const SWoodParams &strengthValues, float minG1, float maxG1,
                      float minG2, float maxG2, float delta, criteriaCB criteria,
                      QMultiMap<float,float> &outXY);
void criteriaValuesToGrid(const SWoodParams &strengthValues, float minG1, float maxG1,
                          float minG2, float maxG2, float delta, criteriaCB criteria,
                          QVector<float> &out, int &colCount, int &rowCount);
void findRoots(const SWoodParams &strengthValues, float minG1, float maxG1,
                      float minG2, float maxG2, float delta, criteriaCB criteria,
                      float &G1min, float &G1max, float &G2min, float &G2max);

#endif // TIMBERHELPER_H
