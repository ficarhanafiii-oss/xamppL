#include <QApplication>
#include <QStyleFactory>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("MyXAMPP");
    app.setApplicationVersion(MYXAMPP_VERSION);
    app.setStyle(QStyleFactory::create("Fusion"));
    QPalette dark;
    dark.setColor(QPalette::Window,        QColor(40,44,52));
    dark.setColor(QPalette::WindowText,    QColor(220,220,220));
    dark.setColor(QPalette::Base,          QColor(30,33,40));
    dark.setColor(QPalette::AlternateBase, QColor(45,50,60));
    dark.setColor(QPalette::Text,          QColor(220,220,220));
    dark.setColor(QPalette::Button,        QColor(53,58,70));
    dark.setColor(QPalette::ButtonText,    QColor(220,220,220));
    dark.setColor(QPalette::Highlight,     QColor(42,130,218));
    dark.setColor(QPalette::HighlightedText, Qt::white);
    app.setPalette(dark);
    MainWindow w;
    w.show();
    return app.exec();
}
