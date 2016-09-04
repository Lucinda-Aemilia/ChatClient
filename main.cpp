#include "mainwindow.h"
#include "signindialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SignInDialog dialog;
    int ret = dialog.exec();
    if (!ret)
        return 0;

    MainWindow w(dialog.getUsername(), dialog.getAddress(), dialog.getPort());
    w.show();

    return a.exec();
}
