#include "mainmenu.h"
#include "./ui_mainmenu.h"
#include <QFontDatabase>
#include <QtSql>
#include "clickablelabel.h"
#include "usersmanipulation.h"

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenu)
     ,data(QSqlDatabase::database("MainConnection"))

{
    ui->setupUi(this);
    this->setWindowTitle("Arcades");


    int id = QFontDatabase::addApplicationFont(":/font/font/PressStart2P-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont pixelFont(family);
    ui->pushButton->setFont(pixelFont);
    ui->pushButton_2->setFont(pixelFont);
    ui->pushButton_3->setFont(pixelFont);

    QPixmap userIcon(":/icon/icons/profile-user.png");
    ClickableLabel* clickableLabel= new ClickableLabel(ui->page);
    clickableLabel->setGeometry(8, 594, 72, 96);
    userIcon=userIcon.scaled(clickableLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    clickableLabel->setPixmap(userIcon);

    QPixmap token(":/icon/icons/tocken blue.png");
    ui->icon->setPixmap(token);

    ui->tokenValue->setFont(pixelFont);

    connect(clickableLabel, &ClickableLabel::clicked, this, &MainMenu::onLabelClicked);
    updateTokensView();

}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainMenu::onLabelClicked(){
    UsersManipulation* users = new UsersManipulation(this);
    connect(users, &UsersManipulation::tokenValueChanged, this, &MainMenu::updateTokensView);
    users->setModal(true);
    users->show();
}

void MainMenu::updateTokensView(){
    QSettings settings("Tompavlo", "Arcades");
    QString curUser = settings.value("lastChoosenUser", "").toString();
    if(curUser.isEmpty()){
        ui->tokenValue->setText("0");
        return;
    }
    if (!data.isValid()) {
        qDebug() << "Database connection is not valid";
    }
    if (data.isOpen()){
        qDebug()<<"Db is open";
    }
    else{
        qDebug()<<"DB is closed"<<data.lastError();
    }
    QSqlQuery query(data);
    query.prepare("SELECT tokens FROM users where id = :id");
    query.bindValue(":id", curUser);
    if(query.exec()){
        if(query.next()){
            ui->tokenValue->setText(query.value(0).toString());
        }
    }
    data.lastError();
}


