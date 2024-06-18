#include "usersmanipulation.h"
#include "ui_usersmanipulation.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QFontDatabase>
#include <QMessageBox>
#include <QDebug>
#include <QtPlugin>
#include <QStandardItemModel>
#include <QModelIndex>

UsersManipulation::UsersManipulation(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UsersManipulation)

{
    ui->setupUi(this);
    this->setWindowTitle("Users");

    data = QSqlDatabase::database("MainConnection");

    model = new QSqlTableModel(this, data);
    model->setTable("users");
    model->select();
    model->setHeaderData(1, Qt::Horizontal, tr("Username"));

    ui->tableView->setModel(model);

    int id = QFontDatabase::addApplicationFont(":/font/font/PressStart2P-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont pixelFont(family);
    ui->tableView->setFont(pixelFont);
    ui->label->setFont(pixelFont);
    ui->lineEdit->setFont(pixelFont);

    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(2);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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
    else {
        QSqlQuery query(data);
        query.prepare("INSERT INTO users (user_name) VALUES (:username)");
        query.bindValue(":username", username);
        query.exec();
        model->submitAll();
        model->select();
        ui->lineEdit->clear();

    }
}


void UsersManipulation::on_pushButton_2_clicked()
{
    QModelIndexList selectedIndex = ui->tableView->selectionModel()->selectedIndexes();
    if(!selectedIndex.empty()){
        int userTokens=0;
        int row = selectedIndex.first().row();
        QModelIndex idIndex = model->index(row, 0);
        QString userId = model->data(idIndex).toString();
        QSettings settings("Tompavlo", "Arcades");
        settings.setValue("lastChoosenUser", userId);
        emit tokenValueChanged();
    }
}


void UsersManipulation::on_pushButton_3_clicked()
{
    QModelIndexList selectedIndex = ui->tableView->selectionModel()->selectedIndexes();
    if(!selectedIndex.isEmpty()){
        int row = selectedIndex.first().row();
        if(model->rowCount()==1){
            QMessageBox error;
            error.setText("Cannot delete last user.");
            error.setIcon(QMessageBox::Warning);
            error.setWindowTitle("Error");
            error.exec();
            return;
        }
        else {
            if(QMessageBox::Yes == QMessageBox::question(nullptr, "Delete confirmation", "Are you sure?", QMessageBox::Yes | QMessageBox::No)){
        model->removeRow(row);
        model->submitAll();
        model->select();
        }
        }
    }
    else {
        QMessageBox error;
        error.setText("Select a user to remove.");
        error.setIcon(QMessageBox::Warning);
        error.setWindowTitle("Error");
        error.exec();
    }

}

