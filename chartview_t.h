#ifndef CHARTVIEW_T_H
#define CHARTVIEW_T_H

#include <QtDebug>
#include <QChartView>
#include <QMimeData>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ChartView_t : public QChartView
{
    Q_OBJECT
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);

signals:
    void updateGrafic(QString csvPath);

public:
    ChartView_t(QWidget *parent = nullptr);
};

#endif // CHARTVIEW_T_H
