#include "firstinitialisation.h"
#include "ui_firstinitialisation.h"
#include <QFontDatabase>
#include <QtSql>
#include <QMessageBox>



firstInitialisation::firstInitialisation(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::firstInitialisation)
{
    ui->setupUi(this);

    int id = QFontDatabase::addApplicationFont(":/font/font/PressStart2P-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont pixelFont(family);
    ui->label->setFont(pixelFont);
    ui->label_2->setFont(pixelFont);
    ui->lineEdit->setFont(pixelFont);
    ui->pushButton->setFont(pixelFont);
    this->setWindowTitle("Arcades");

}

firstInitialisation::~firstInitialisation()
{
    delete ui;
}

void firstInitialisation::on_pushButton_clicked()
{
    QString username = ui->lineEdit->text().trimmed();
    if(username.isEmpty()){
        QMessageBox error;
        error.setText("Username cannot be empy!");
        error.setIcon(QMessageBox::Critical);
        error.setWindowTitle("Error");
        error.exec();
        return;
    }
    QSqlDatabase data = QSqlDatabase::addDatabase("QSQLITE");
    data.setDatabaseName("./../../db/users.db");
    QSqlQuery query(data);
    data.open();
    query.prepare("INSERT INTO users (user_name) VALUES (:username);");
    query.bindValue(":username", username);
    query.exec();

    this->close();
}

