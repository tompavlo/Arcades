#include "firstinitialisation.h"
#include "mainmenu.h"

#include <QApplication>
#include <QtSql>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainMenu w;
    w.setFixedSize(1000,766);


    QSqlDatabase data = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = "./../../db/users.db";
    data.setDatabaseName(dbPath);
    data.open();

    QSqlQuery query(data);
    query.exec("SELECT COUNT(*) FROM users;");
    if (query.next() && query.value(0).toInt() == 0) {
        firstInitialisation window;
        window.setModal(true);
        window.exec();
        QSqlDatabase data = QSqlDatabase::addDatabase("QSQLITE");
        QString dbPath = "./../../db/users.db";
        data.setDatabaseName(dbPath);
        data.open();
        QSqlQuery query2(data);
        query2.exec("SELECT COUNT(*) FROM users;");
        if (query2.next() && !(query2.value(0).toInt() == 0)){
            w.show();
        }
        else return 0;
    }
    else w.show();
    return a.exec();
}
