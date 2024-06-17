#include "mainmenu.h"
#include "./ui_mainmenu.h"
#include <QFontDatabase>
#include <QtSql>
#include "clickablelabel.h"
#include "usersmanipulation.h"

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenu)

{
    ui->setupUi(this);
    this->setWindowTitle("Arcades");

    QSqlDatabase data = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = "./../../db/users.db";
    data.setDatabaseName(dbPath);
    qDebug() << "Attempting to open database at path:" << dbPath;
    if (!data.open()) {
        qDebug() << "Database connection error:" << data.lastError().text();
    } else {
        qDebug() << "Database connection successful.";
    }

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
    users->setModal(true);
    users->show();
}


