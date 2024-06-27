#include "firstinitialisation.h"
#include "ui_firstinitialisation.h"
#include <QFontDatabase>
#include <QtSql>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QAudioOutput>


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
    QMediaPlayer *sound = new QMediaPlayer(this);
    QAudioOutput *levelSound = new QAudioOutput(this);
    QThread::msleep(300);
    sound->setAudioOutput(levelSound);
    levelSound->setVolume(0.5);
    sound->setSource(QUrl("qrc:/sound/sound/button-124476.mp3"));
    sound->setPosition(0);
    sound->play();
    QString username = ui->lineEdit->text().trimmed();
    if(username.isEmpty()){
        QMessageBox error;
        error.setText("Username cannot be empy!");
        error.setIcon(QMessageBox::Critical);
        error.setWindowTitle("Error");
        error.exec();
        return;
    }
    QSqlDatabase data = QSqlDatabase::database("MainConnection");
    QSqlQuery query(data);
    query.prepare("INSERT INTO users (user_name) VALUES (:username);");
    query.bindValue(":username", username);
    query.exec();

    QVariant userIdVariant = query.lastInsertId();
    QSettings settings("Tompavlo", "Arcades");
    settings.setValue("lastChoosenUser", userIdVariant.toInt());

    this->close();
}

