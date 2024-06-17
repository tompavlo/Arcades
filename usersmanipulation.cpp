#include "usersmanipulation.h"
#include "ui_usersmanipulation.h"
#include <QtSql>
#include <QFontDatabase>
#include <QMessageBox>

UsersManipulation::UsersManipulation(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UsersManipulation)
{
    ui->setupUi(this);
    this->setWindowTitle("Users");

    QSqlDatabase data = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = "./../../db/users.db";
    data.setDatabaseName(dbPath);
    qDebug() << "Attempting to open database at path:" << dbPath;
    if (!data.open()) {
        qDebug() << "Database connection error:" << data.lastError().text();
    } else {
        qDebug() << "Database connection successful.";
    }

    QSqlTableModel* model = new QSqlTableModel(this, data);
    model->setTable("users");
    model->select();

    ui->listView->setModel(model);

    int id = QFontDatabase::addApplicationFont(":/font/font/PressStart2P-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont pixelFont(family);
    ui->listView->setFont(pixelFont);
    ui->label->setFont(pixelFont);
    ui->lineEdit->setFont(pixelFont);
}

UsersManipulation::~UsersManipulation()
{
    delete ui;
}

void UsersManipulation::on_pushButton_clicked()
{
    QString username = ui->lineEdit->text().trimmed();
    if(username.isEmpty()){
        QMessageBox error;
        error.setText("Username cannot be empty.");
        error.setIcon(QMessageBox::Warning);
        error.setWindowTitle("Error");
        error.exec();
    }
    else ;
}


void UsersManipulation::on_pushButton_2_clicked()
{

}


void UsersManipulation::on_pushButton_3_clicked()
{
    QModelIndexList selectedIndex = ui->listView->selectionModel()->selectedIndexes();
    if(!selectedIndex.isEmpty()){
        int row = selectedIndex.first().row();
        model->removeRow(row);
        model->submitAll();
    }
    else {
        QMessageBox error;
        error.setText("Select a user to remove.");
        error.setIcon(QMessageBox::Warning);
        error.setWindowTitle("Error");
        error.exec();
    }
}

