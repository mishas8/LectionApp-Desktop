#include <QApplication>
#include <QQmlApplicationEngine>
#include <QSqlDatabase>
#include <iostream>
#include <QTreeView>
#include <imageprovider.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ImageProvider model("DB_Lectures", nullptr);
    QTreeView view;
    view.setModel(&model);
    view.show();

    /*QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));*/

    return app.exec();
}
