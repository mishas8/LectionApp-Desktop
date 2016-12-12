#include <QApplication>
#include <QQmlApplicationEngine>
#include <QSqlDatabase>
#include <iostream>
#include <QTreeView>
#include <imageprovider.h>
#include <mainwindow.h>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ImageProvider model("DB_Lectures");
    QTreeView view;
    view.setModel(&model);
    view.setWindowTitle(QObject::tr("Lectures"));
    view.show();

    /*QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));*/

    /*MainWindow *window = new MainWindow();
    window->show();*/

    /*QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("DB_Lectures");

        db.open();
        if (!db.open()) {
                qDebug() << "Что-то пошло не так!";
                return -1;
            }

        QSqlQuery query;
        bool ok = query.exec("SELECT * FROM images");

        query.next();
        qDebug() << query.executedQuery();

        if (!ok) {
            qDebug() << "It's bad!";
            return -2;
        }

        //std::cout << ok << std::endl;*/
        //db.close();

    return app.exec();
}
