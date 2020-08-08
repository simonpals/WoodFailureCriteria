#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QHeaderView>

const QString csAppTitle = "Межі міцності деревини";

static void showMessage(const QString &str)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(csAppTitle);
    msgBox.setText(str);
    msgBox.exec();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    setFixedSize(width(), height());
    setWindowTitle(csAppTitle);

    for(SWoodParams wood : lsWoodParams)
    {
        ui->cbWoodSpecies->addItem(wood.species);
    }

    for(SCriteriaParams criteria : lsCriteriaParams)
    {
        ui->cbCriteriaName->addItem(criteria.criteriaName);
    }

    ui->cbCriteriaName->setCurrentIndex(0);

    ui->cbHumidity->addItem("5%");
    ui->cbHumidity->addItem("10%");
    ui->cbHumidity->addItem("12%");
    ui->cbHumidity->addItem("15%");
    ui->cbHumidity->addItem("20%");
    ui->cbHumidity->addItem("25%");
    ui->cbHumidity->setCurrentText("12%");

    for(int i=20; i<=40; i++)
        ui->cbTemperature->addItem(QString::number(i)+"°C");
    ui->cbTemperature->setCurrentText("20°C");

    ui->plotOutput->setChecked(true);

    tvCriteriaTable = new QTableWidget(ui->criteriaPlot);
    tvCriteriaTable->horizontalHeader()->setSectionResizeMode (QHeaderView::ResizeToContents);
//    tvCriteriaTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QFont font = tvCriteriaTable->font();
    font.setPointSize(6);
    tvCriteriaTable->setFont(font);

    on_plotOutput_clicked();

    connect(ui->criteriaPlot, &StrengthChart::plotSizeChanged, [=](){
        tvCriteriaTable->setFixedSize(ui->criteriaPlot->size());
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

QMultiMap <float, float> MainWindow::prepareParameters(criteriaCB criteria)
{
    QMultiMap <float, float> XY;
    optimizeCriteria(stiffnessList(ui->cbWoodSpecies->currentText(),ui->cbTemperature->currentText().replace("°C","").toFloat(),ui->cbHumidity->currentText().replace("%","").toFloat()),
                     ui->leSigma1Min->text().toFloat(), ui->leSigma1Max->text().toFloat(), ui->leSigma2Min->text().toFloat(), ui->leSigma2Max->text().toFloat(),
                     ui->leDelta->text().toFloat(), criteria, XY);

    return XY;
}

void MainWindow::on_bShowCriteria_clicked()
{
    int ind = ui->cbCriteriaName->currentIndex();

    if(ui->tableOutput->isChecked())
    {
        outputTableData();
    }
    else
    {
        ui->criteriaPlot->visualiseGraph(prepareParameters(lsCriteriaParams[ind].criteriaMethod),
                                         ui->leSigma1Min->text().toFloat(), ui->leSigma1Max->text().toFloat(),
                                         ui->leSigma2Min->text().toFloat(), ui->leSigma2Max->text().toFloat(),
                                         ui->checkBox->isChecked());
    }
}

void MainWindow::on_bClear_clicked()
{
    ui->criteriaPlot->chart()->removeAllSeries();

    tvCriteriaTable->clear();
    tvCriteriaTable->setRowCount(0);
}

void MainWindow::on_bDescribe_clicked()
{
    int ind = ui->cbCriteriaName->currentIndex();
    describeCriteria(lsCriteriaParams[ind].criteriaMethod, lsCriteriaParams[ind].criteriaName);
}

void MainWindow::on_cbCriteriaName_currentIndexChanged(int index)
{
//    ui->leSigma1Min->setText(QString::number(lsCriteriaParams[index].minSigma1));
//    ui->leSigma1Max->setText(QString::number(lsCriteriaParams[index].maxSigma1));
//    ui->leSigma2Min->setText(QString::number(lsCriteriaParams[index].minSigma2));
//    ui->leSigma2Max->setText(QString::number(lsCriteriaParams[index].maxSigma2));
}

void MainWindow::on_plotOutput_clicked()
{
    on_bClear_clicked();

    if(ui->tableOutput->isChecked())
    {
        tvCriteriaTable->setVisible(true);
        tvCriteriaTable->setFixedSize(ui->criteriaPlot->size());
        tvCriteriaTable->raise();
    }
    else
    {
        tvCriteriaTable->setVisible(false);
    }
}

void MainWindow::on_tableOutput_clicked()
{
    on_plotOutput_clicked();
}

void MainWindow::outputTableData()
{
    on_bClear_clicked();

    int colCnt=0, rowCnt=0;
    QVector<float> vec;
    int ind = ui->cbCriteriaName->currentIndex();

    criteriaValuesToGrid(stiffnessList(ui->cbWoodSpecies->currentText(),ui->cbTemperature->currentText().replace("°C","").toFloat(),ui->cbHumidity->currentText().replace("%","").toFloat()),
                         ui->leSigma1Min->text().toFloat(), ui->leSigma1Max->text().toFloat(), ui->leSigma2Min->text().toFloat(), ui->leSigma2Max->text().toFloat(),
                         ui->leDelta->text().toFloat(), lsCriteriaParams[ind].criteriaMethod, vec, colCnt, rowCnt);

    tvCriteriaTable->setColumnCount(colCnt);
    tvCriteriaTable->setRowCount(rowCnt);

    float delta = ui->leDelta->text().toFloat();

    for(int x2=0; x2<colCnt; x2++)
    {
        float s2min = ui->leSigma2Min->text().toFloat();
        tvCriteriaTable->setHorizontalHeaderItem(x2, new QTableWidgetItem(QString::number(s2min+x2*delta, 'f', 2)));
    }

    bool invertY = ui->checkBox->isChecked();

    for(int x1=0; x1<rowCnt; x1++)
    {
        float s1min = ui->leSigma1Min->text().toFloat();
        tvCriteriaTable->setVerticalHeaderItem((invertY ? rowCnt-x1-1 : x1), new QTableWidgetItem(QString::number(s1min+x1*delta, 'f', 2)));

        for(int x2=0; x2<colCnt; x2++)
        {
            float value = vec[x1*colCnt+x2];
            QString strVal = QString::number(value, 'f', 5);
            QTableWidgetItem *item = new QTableWidgetItem(strVal);
            if(value>0)
                item->setTextColor(QColor(Qt::red));
            tvCriteriaTable->setItem((invertY ? rowCnt-x1-1 : x1), x2, item);
        }
    }
}

void MainWindow::describeCriteria(criteriaCB criteria, const QString &name)
{
    float G1min, G1max, G2min, G2max;
    findRoots(stiffnessList(ui->cbWoodSpecies->currentText(),ui->cbTemperature->currentText().replace("°C","").toFloat(),ui->cbHumidity->currentText().replace("%","").toFloat()),
                     ui->leSigma1Min->text().toFloat(), ui->leSigma1Max->text().toFloat(), ui->leSigma2Min->text().toFloat(), ui->leSigma2Max->text().toFloat(),
                     ui->leDelta->text().toFloat(), criteria, G1min, G1max, G2min, G2max);

    float r = fabs(G1min/G1max);
    float t = fabs(G2min/G2max);

    showMessage(QString("Для критерію "+name+" відношення межі міцності при стиску до межі міцності при розтягу у радіальному напрямі рівне: %1 "+"у тангентальному напрямі рівне: %2").arg(r).arg(t));
}


//void MainWindow::on_goldenblatKopnov_clicked()
//{
//    ui->goldenblatKopnovPlot->visualiseGraph(prepareParameters(&goldenblatKopnov),
//                                             ui->leSigma1Min->text().toFloat(), ui->leSigma1Max->text().toFloat(),
//                                             ui->leSigma2Min->text().toFloat(), ui->leSigma2Max->text().toFloat());
//}

//void MainWindow::on_misesHill_clicked()
//{
//    ui->misesHillPlot->visualiseGraph(prepareParameters(&misesHillCriteria),
//                                             ui->leSigma1Min->text().toFloat(), ui->leSigma1Max->text().toFloat(),
//                                             ui->leSigma2Min->text().toFloat(), ui->leSigma2Max->text().toFloat());
//}

//void MainWindow::on_zakharov_clicked()
//{
//    ui->zakharovPlot->visualiseGraph(prepareParameters(&zakharovCriteria),
//                                             ui->leSigma1Min->text().toFloat(), ui->leSigma1Max->text().toFloat(),
//                                             ui->leSigma2Min->text().toFloat(), ui->leSigma2Max->text().toFloat());
//}

//void MainWindow::on_pbClearGoldenblatKopnov_clicked()
//{
//    ui->goldenblatKopnovPlot->chart()->removeAllSeries();
//}

//void MainWindow::on_pbClearMisesHill_clicked()
//{
//    ui->misesHillPlot->chart()->removeAllSeries();
//}

//void MainWindow::on_pbClearZakharov_clicked()
//{
//    ui->zakharovPlot->chart()->removeAllSeries();
//}


//void MainWindow::on_pbDescribeGoldenblatKopnov_clicked()
//{
//    describeCriteria(&goldenblatKopnov, "Гольленблата-Копнова");
//}

//void MainWindow::on_pbDescribeMisesHill_clicked()
//{
//    describeCriteria(&misesHillCriteria, "Мізеса-Хілла");
//}

//void MainWindow::on_pbDescribeZakharov_clicked()
//{
//    describeCriteria(&zakharovCriteria, "Захарова");
//}

