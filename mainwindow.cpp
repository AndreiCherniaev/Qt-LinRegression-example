#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QMessageBox>
#include <QFileDialog>

#define goodView ( 2 ) //так как точка очень жирная (ни разу не пиксель), то надо отступить от краёв осей, чтобы точка смогла прорисоваться

void MainWindow::prepareGrafic(){
    chart->setAnimationOptions(QChart::NoAnimation);
    chart->legend()->hide();

    m_axisX->setTickCount(5);
    m_axisX->setTitleText("абсцисса");
    chart->addAxis(m_axisX, Qt::AlignBottom);

    m_axisY->setTickCount(3);
    m_axisY->setTickAnchor(100.0);
    m_axisY->setRange(40, 200);
    m_axisY->setTitleText("ордината");
    chart->addAxis(m_axisY, Qt::AlignLeft);

    chartView->setChart(chart);
    chartView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->verticalLayout->addWidget(chartView);
    chartView->setAcceptDrops(true);
    connect(chartView, &ChartView_t::updateGrafic, this, &MainWindow::on_MakeGrafic, Qt::QueuedConnection);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_axisX(new QValueAxis)
    , m_axisY(new QValueAxis)
    , chart(new QChart())
    , chartView(new ChartView_t(this))
{
    ui->setupUi(this);
    prepareGrafic();

    //to test without file
    p.append({1,115});
    p.append({-50,140});
    p.append({90,115});
    VisualAnalizeFreq();
}

void MainWindow::dropEvent(QDropEvent* event)
{
  const QMimeData* mimeData = event->mimeData();

  // check for our needed mime type, here a file or a list of files
  if (mimeData->hasUrls())
  {
    QStringList pathList;
    QList<QUrl> urlList = mimeData->urls();

    // extract the local paths of the files
    /*for (int i = 0; i < urlList.size() && i < 32; +i)
    {
      pathList.append(urlList.at(i).toLocalFile());
    }

    // call a function to open the files
    openFiles(pathList);*/
  }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::VisualAnalizeFreq(){
        QScatterSeries *dots = new QScatterSeries(this);
        dots->setPointLabelsFormat("(@xPoint g, @yPoint a)");
        chart->addSeries(dots);
        dots->attachAxis(m_axisX);
        dots->attachAxis(m_axisY);
        dots->setColor(QColor("blue"));
        dots->setName("dots");

        qreal maxY= 0, minY= 0;
        qreal maxX= 0, minX= 0;
        for(int i=0;i<p.size();i++){
          dots->append(p.at(i));
          if(p.at(i).y() > maxY) maxY= p.at(i).y();
          if(p.at(i).x() > maxX) maxX= p.at(i).x();
          if(p.at(i).x() < minX) minX= p.at(i).x();
          if(p.at(i).y() < minY) minY= p.at(i).y();
        }
        m_axisX->setRange(minX-goodView, maxX+goodView);
        m_axisY->setRange(minY-goodView, maxY+goodView);

        linReg(p);

        chartView->chart()->legend()->show();
}

void MainWindow::on_MakeGrafic(QString fileName){
  chart->removeAllSeries();
  chart->removeAxis(m_axisX);
  chart->removeAxis(m_axisY);
  p.clear();
  prepareGrafic();

  this->setWindowTitle("analize "+ fileName);

  file = new QFile(fileName);
  QStringList listA;
  int row = 0;

  if(file->open(QIODevice::ReadOnly)){
    file->readLine();
    qDebug("csv loding");
    while(!file->atEnd()){
      QString line = file->readLine();
      listA = line.split(";");
      if(listA.size()!= 3){
        qDebug() << "err csv line size" << listA.size();
        goto NO_LOADING_LABEL;
      }
      int tmpValY = listA.at(2).toInt(); //Y
      //qDebug() << "listA.at(x)" << tmpValY;
      p.append({row, tmpValY});
      row++;
      if(row>= 800){
        qDebug() << "err csv has " << row << "strings. May its too big?";
        break;
      }
    }
  }else qDebug() << "err csv" << fileName << "not found";
  file->close();

  VisualAnalizeFreq();

NO_LOADING_LABEL:;
}

void MainWindow::linReg(QList<QPoint> points){
  qDebug() << "кол-во точек для регрессии" << points.size();;  //кол-во точек для регрессии
  double Sx=0,Sy=0,Sxy=0,Sxx=0;  //Средние значения

  for(int i=0;i < points.size();i++){
      Sx+=points.at(i).x();
      Sy+=points.at(i).y();
      Sxy+=points.at(i).x()*points.at(i).y();
      Sxx+=points.at(i).x()*points.at(i).x();
  }
  /*Sx/=PointsAmo;
  Sy/=PointsAmo;
  Sxy/=PointsAmo;
  Sxx/=PointsAmo;*/
  qreal LinregA= (Sx*Sy-Sxy * points.size())/(Sx*Sx-Sxx * points.size());
  qreal LinregB= (Sxy-LinregA*Sxx)/Sx;
  qDebug() << "y=LinregA * x + LinregB";
  qDebug() << "LinregA=" << LinregA << "LinregB=" << LinregB;

  //Красный график есть прямая y=LinregA * x + LinregB
  QLineSeries *aprox = new QLineSeries;
  aprox->setPointLabelsFormat("(@xPoint g, @yPoint a)");
  chart->addSeries(aprox);
  aprox->attachAxis(m_axisX);
  aprox->attachAxis(m_axisY);
  aprox->setColor(QColor("red"));
  aprox->setName("Linear reg");
  for(int i=points.at(0).x();i<points.at(points.size()-1).x();i++){
    aprox->append(i, LinregA*i+LinregB);
  }
}
