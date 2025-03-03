#include "usersmanipulation.h"
#include "ui_usersmanipulation.h"
#include "mainmenu.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QFontDatabase>
#include <QMessageBox>
#include <QDebug>
#include <QtPlugin>
#include <QStandardItemModel>
#include <QModelIndex>

class ReadOnlyDelegate : public QStyledItemDelegate
{
public:
    ReadOnlyDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent){}
    QWidget *createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override{
        return nullptr;
    }
};

UsersManipulation::UsersManipulation(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UsersManipulation)
    ,sound(new QMediaPlayer)
    ,audioSound(new QAudioOutput)

{
    ui->setupUi(this);
    this->setWindowTitle("Users");
    this->resize(425, 300);

    data = QSqlDatabase::database("MainConnection");

    model = new QSqlTableModel(this, data);
    model->setTable("users");
    model->select();
    model->setHeaderData(1, Qt::Horizontal, tr("Username"));

    sound->setAudioOutput(audioSound);
    sound->setSource(QUrl("qrc:/sound/sound/button-124476.mp3"));
    QSettings settings("Tompavlo", "Arcades");
    audioSound->setVolume((settings.value("soundVolume", 50).toFloat())/100);

    ui->tableView->setModel(model);

    int id = QFontDatabase::addApplicationFont(":/font/font/PressStart2P-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont pixelFont(family);
    ui->tableView->setFont(pixelFont);
    ui->label->setFont(pixelFont);
    ui->lineEdit->setFont(pixelFont);

    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(2);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setItemDelegateForColumn(1, new ReadOnlyDelegate(this));
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

UsersManipulation::~UsersManipulation()
{
    delete ui;
}

void UsersManipulation::on_pushButton_clicked()
{
    sound->setPosition(0);
    sound->play();
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
    sound->setPosition(0);
    sound->play();
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
    sound->setPosition(0);
    sound->play();
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
        else if(QMessageBox::Yes == QMessageBox::question(nullptr, "Delete confirmation", "Are you sure?", QMessageBox::Yes | QMessageBox::No)){
            QSettings settings("Tompavlo", "Arcades");
            QModelIndex idIndex = model->index(row, 0);
            QString userId = model->data(idIndex).toString();
            QSqlQuery query(data);
            if(userId==settings.value("lastChoosenUser", "")) {
                query.exec("SELECT id FROM users ORDER BY id");
                QString newUserId;
                QString previousUserId;
                while(query.next()){
                    newUserId=query.value(0).toString();
                    if(newUserId==userId){
                        if(query.next()){
                            newUserId = query.value(0).toString();
                            previousUserId = newUserId;
                            break;
                        }
                        else break;
                    }
                    previousUserId = newUserId;
                }
                settings.setValue("lastChoosenUser", previousUserId);
                emit tokenValueChanged();
            }
            model->removeRow(row);
            model->submitAll();
            model->select();
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

