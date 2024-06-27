#include "firstinitialisation.h"
#include "mainmenu.h"

#include <QApplication>
#include <QtSql>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QSqlDatabase data = QSqlDatabase::addDatabase("QSQLITE", "MainConnection");
    QString dbPath = "./../../db/users.db";
    data.setDatabaseName(dbPath);
    data.open();

    QSettings settings("Tompavlo", "Arcades");

    MainMenu w;
    w.setFixedSize(1100,766);

    QSqlQuery query(data);
    query.exec("SELECT COUNT(*) FROM users;");
    if (query.next() && query.value(0).toInt() == 0) {
        firstInitialisation window;
        window.setModal(true);
        window.exec();
        QSqlDatabase data = QSqlDatabase::database("MainConnection");
        QSqlQuery query(data);
        query.exec("SELECT COUNT(*) FROM users;");
        if (query.next() && !(query.value(0).toInt() == 0)){
            w.show();
        }
        else return 0;
    }
    else w.show();
    return a.exec();
}
