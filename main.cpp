#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QDebug>



int main(int argc, char *argv[])
{



    QApplication a(argc, argv);
    // qApp->setStyle(QStyleFactory::create("Fusion"));
    // // qApp->setStyle(QStyleFactory::create("Windows"));
    // QPalette newPalette;
    // newPalette.setColor(QPalette::Window,          QColor( 37,  37,  37));
    // newPalette.setColor(QPalette::WindowText,      QColor(212, 212, 212));
    // newPalette.setColor(QPalette::Base,            QColor( 60,  60,  60));
    // newPalette.setColor(QPalette::AlternateBase,   QColor( 45,  45,  45));
    // newPalette.setColor(QPalette::PlaceholderText, QColor(127, 127, 127));
    // newPalette.setColor(QPalette::Text,            QColor(212, 212, 212));
    // newPalette.setColor(QPalette::Button,          QColor( 45,  45,  45));
    // newPalette.setColor(QPalette::ButtonText,      QColor(212, 212, 212));
    // newPalette.setColor(QPalette::BrightText,      QColor(240, 240, 240));
    // newPalette.setColor(QPalette::Highlight,       QColor( 38,  79, 120));
    // newPalette.setColor(QPalette::HighlightedText, QColor(240, 240, 240));

    // newPalette.setColor(QPalette::Light,           QColor( 60,  60,  60));
    // newPalette.setColor(QPalette::Midlight,        QColor( 52,  52,  52));
    // newPalette.setColor(QPalette::Dark,            QColor( 30,  30,  30));
    // newPalette.setColor(QPalette::Mid,             QColor( 37,  37,  37));
    // newPalette.setColor(QPalette::Shadow,          QColor( 0,    0,   0));
    // newPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
    // qApp->setPalette(newPalette);

    MainWindow w;
    w.setWindowIcon(QIcon(":/icon/data/icon2.ico"));
    w.show();


    return a.exec();
}
