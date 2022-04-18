#include "chartview_t.h"

ChartView_t::ChartView_t(QWidget *parent) : QChartView(parent){
}

void ChartView_t::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasFormat("text/plain"))
        event->acceptProposedAction();
}

void ChartView_t::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
        event->acceptProposedAction();
    qDebug() << "file detected";
}
void ChartView_t::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();

   // check for our needed mime type, here a file or a list of files
   if (mimeData->hasUrls())
   {
     QStringList pathList;
     QList<QUrl> urlList = mimeData->urls();

     // extract the local paths of the files
    /* for (int i = 0; i < urlList.size() && i < 32; +i)
     {
       pathList.append(urlList.at(i).toLocalFile());
     }*/
     QString csvFilePath(urlList.at(0).path());
     //on_MakeGrafic_Button_clicked(csvFilePath);
     qDebug() << csvFilePath;
     emit updateGrafic(csvFilePath);
     // call a function to open the files
     //openFiles(pathList);
   }
}
