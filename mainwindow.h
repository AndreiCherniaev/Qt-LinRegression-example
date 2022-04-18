#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chartview_t.h"
#include <QValueAxis>
#include <QScatterSeries>
#include <QLineEdit>
#include <QChart>
#include <QtCharts/QLineSeries>
#include <QChartView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
using namespace QtCharts;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_MakeGrafic(QString fileName);
    void dropEvent(QDropEvent* event);
private:
    Ui::MainWindow *ui;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    QChart *chart = nullptr;
    ChartView_t *chartView  = nullptr;
    QTextStream *csvStream;
    QFile *file;
    QList<QPoint> p;
    void VisualAnalizeFreq(void);
    void prepareGrafic(void);
    void linReg(QList<QPoint> points);
};
#endif // MAINWINDOW_H
