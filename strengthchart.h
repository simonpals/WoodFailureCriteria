#ifndef STRENGTHCHART_H
#define STRENGTHCHART_H

#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE

class StrengthChart : public QChartView
{
    Q_OBJECT
public:
    StrengthChart(QWidget *parent=nullptr);
    void visualiseGraph(const QMultiMap <float, float> &XY, float sigma1Min, float sigma1Max, float sigma2Min, float sigma2Max, bool invertY);

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void plotSizeChanged();
};

#endif // STRENGTHCHART_H
