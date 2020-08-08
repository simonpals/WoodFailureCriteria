#include "strengthchart.h"

#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>

static int nColorCounter = 5;

StrengthChart::StrengthChart(QWidget *parent)
    : QChartView(parent)
{
    QChart *chart = this->chart();
    chart->setTheme(QChart::ChartThemeDark);
}

void StrengthChart::visualiseGraph(const QMultiMap <float, float> &XY, float sigma1Min, float sigma1Max, float sigma2Min, float sigma2Max, bool invertY)
{
    QChart *chart = this->chart();
    if(chart==nullptr)
        chart = new QChart();

    QScatterSeries *series = new QScatterSeries();
    series->setMarkerSize(6);
    series->setColor(QColor(Qt::GlobalColor(nColorCounter++%Qt::transparent)));

    QMap<float,float>::const_iterator it = XY.begin();
    for(;it != XY.end(); ++it)
    {
        series->append(it.key(), it.value());
    }
    series->setPointLabelsVisible(true);

    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->axisX()->setRange(sigma2Min, sigma2Max);
    chart->axisX()->setTitleText("Тангентальний напрям");
    chart->axisY()->setRange(sigma1Min, sigma1Max);
    chart->axisY()->setReverse(!invertY);
    chart->axisY()->setTitleText("Радіальний напрям");
//    chart->setTitle("Simple line chart example");

    this->setRenderHint(QPainter::Antialiasing);
}

void StrengthChart::resizeEvent(QResizeEvent *event)
{
    QChartView::resizeEvent(event);
    emit plotSizeChanged();
}
