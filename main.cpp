#include <gui/main-window.hpp>
#include <settings-manager.hpp>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SettingsManager mgr;
    MainWindow w(nullptr, mgr);
    w.show();

    return a.exec();
}
