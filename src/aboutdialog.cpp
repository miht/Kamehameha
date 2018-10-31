#include "aboutdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QImage>
#include <QDebug>
#include <QLabel>
#include <QFontDatabase>
#include <QString>
#include <QSpacerItem>

AboutDialog::AboutDialog(QWidget *parent)
    :   QDialog(parent)
{
    QHBoxLayout *hLayout = new QHBoxLayout();

    QLabel *lbl_image = new QLabel();
    QImage image(":/about/sun_icon");
    image = image.scaled (128, 128);
    lbl_image->setPixmap(QPixmap::fromImage (image));

    hLayout->addWidget (lbl_image);

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setSpacing (0);

    //Load font
    int id = QFontDatabase::addApplicationFont(":/general/Merriweather-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font_headline(family, 42);
    QFont font_version(family, 12);
    QFont font_text(family, 12);

    QHBoxLayout *headline_layout = new QHBoxLayout();
    QLabel *headline = new QLabel("Helios");
    headline->setAlignment (Qt::AlignTop);
    QLabel *copyright = new QLabel("©");
    copyright->setAlignment (Qt::AlignTop);

    headline_layout->addWidget(headline);
    headline_layout->addWidget(copyright);

    QLabel *version = new QLabel("Version 0.1 2018");
    QLabel *text = new QLabel("Anders Eriksson\nLeif T. Sundkvist");
    text->setMaximumWidth (256);
    headline->setFont (font_headline);
    version->setFont (font_version);
    text->setFont (font_text);

    QSpacerItem *spacer = new QSpacerItem(16, 16);

    vLayout->addLayout (headline_layout);
    vLayout->addWidget(version);
//    layout->addWidget(spacer);
    vLayout->addSpacing (16);
    vLayout->addWidget (text);

    hLayout->addLayout (vLayout);

    setLayout (hLayout);
}
