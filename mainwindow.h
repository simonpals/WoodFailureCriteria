#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "strengthchart.h"
#include "timberhelper.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_bShowCriteria_clicked();
    void on_bClear_clicked();
    void on_bDescribe_clicked();
    void on_cbCriteriaName_currentIndexChanged(int index);
    void on_plotOutput_clicked();
    void on_tableOutput_clicked();

protected:
    void outputTableData();
    void describeCriteria(criteriaCB criteria, const QString &name);
    QMultiMap <float, float> prepareParameters(criteriaCB criteria);

private:
    QTableWidget *tvCriteriaTable;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
