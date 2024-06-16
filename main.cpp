#include "firstinitialisation.h"
#include "mainmenu.h"

#include <QApplication>
#include <QtSql>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainMenu w;
    QSqlDatabase data = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = "./../../db/users.db";
    data.setDatabaseName(dbPath);
    qDebug() << "Attempting to open database at path:" << dbPath;
    if (!data.open()) {
        qDebug() << "Database connection error:" << data.lastError().text();
    } else {
        qDebug() << "Database connection successful.";
    }

    QSqlQuery query(data);
    query.exec("SELECT COUNT(*) FROM users;");
    if (query.next() && query.value(0).toInt() == 0) {
        firstInitialisation window;
        window.setModal(true);
        window.exec();
        QSqlDatabase data = QSqlDatabase::addDatabase("QSQLITE");
        QString dbPath = "./../../db/users.db";
        data.setDatabaseName(dbPath);
        qDebug() << "Attempting to open database at path:" << dbPath;
        if (!data.open()) {
            qDebug() << "Database connection error:" << data.lastError().text();
        } else {
            qDebug() << "Database connection successful.";
        }
        QSqlQuery query2(data);
        if (!query2.exec("SELECT COUNT(*) FROM users;")) {
            qDebug() << "SQL query execution error:" << query2.lastError().text();
            return -1;  // Exit if query execution fails
        }
        if (query2.next() && !(query2.value(0).toInt() == 0)){
            w.show();
        }
        else return 0;
    }
    else w.show();
    return a.exec();
}
