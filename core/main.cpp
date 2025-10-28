#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    try {
        AppContext context;
        MainWindow w(context);
        w.show();
        return app.exec();
    } catch (const std::exception &ex) {
        qCritical() << "Exception:" << ex.what();
        return 3;
    }
}
