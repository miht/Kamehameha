//#include "renderview.h"

//RenderView::RenderView(QWidget *parent)
//    :   QWidget(parent)
//{
//    setWindowTitle("Render output");

//    //Configure menubar
//    menubar->setNativeMenuBar (false);
//    QMenu *file = menubar->addMenu (tr("File"));
//    QAction *exp = file->addAction(tr("Export image"), this, SLOT(export_image()));
//    exp->setStatusTip(tr("Export rendered image to file"));

//    //Configure the output window
//    graphicsView= new QGraphicsView();
//    graphicsView->setFixedSize(512, 512);
//    graphicsView->setMinimumSize(512, 512);
//    graphicsView->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
//}

//void RenderView::export_image () {
//    QString path = "/Users/leifthysellsundqvist/Documents/Kamehameha/res/";
//    QString file;

//    file = QFileDialog::getSaveFileName (this, tr("Save image"), path, tr("Images (*.png *.jpg)"));

//    bool saved;

//    QImage output(512, 512,  QImage::Format_RGB32);
//    QPainter painter(&output);
//    graphicsView->scene ()->render(&painter);
//    saved = output.save(file, "png");

//}
