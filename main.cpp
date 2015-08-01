#include <gui/main-window.hpp>
#include <settings.hpp>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Settings mgr;
    MainWindow w(nullptr, mgr);
    w.show();

    return a.exec();
}
