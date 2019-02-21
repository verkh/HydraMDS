#include "MainWindow.h"
#include <QApplication>
#include <QSettings>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSettings s("settings.ini", QSettings::IniFormat);

    s.beginGroup("AvaliableTypes");
    s.beginWriteArray("Type");
    s.setArrayIndex(0);
    s.setValue("Name"  , "Player");
    s.setValue("Unique", true    );
    s.setValue("Pixmap", "player.png");
    s.endArray();
    s.endGroup();

    MainWindow w;
    w.show();

    return a.exec();
}
