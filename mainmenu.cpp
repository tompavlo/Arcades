#include "mainmenu.h"
#include "./ui_mainmenu.h"
#include <QFontDatabase>
#include <QtSql>
#include "clickablelabel.h"
#include "usersmanipulation.h"
#include "tictactoe.h"
#include <QMessageBox>
#include <QFormLayout>
#include <QTextEdit>
#include <QSlider>
#include <QtMultimedia/QtMultimedia>
#include <QAudioOutput>



class InfoDialog : public QDialog{
public:
    InfoDialog(QWidget *parent= nullptr, const QPixmap &image = QPixmap(), const  QString &filePath=QString(), QMediaPlayer *sound = nullptr) : QDialog(parent){
        QFormLayout *layout = new QFormLayout(this);
        QLabel *imageLabel = new QLabel(this);
        QPixmap resizableImage = image;
        imageLabel->setFixedSize(200, 300);
        resizableImage=resizableImage.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imageLabel->setPixmap(resizableImage);
        QTextEdit *textEdit = new QTextEdit(this);
        textEdit->setReadOnly(true);
        textEdit->setFixedSize(400, 300);
        QFile file(filePath);
        file.open(QIODevice::ReadOnly | QIODevice::Text );
        QTextStream in(&file);
        QString fileContent = in.readAll();
        textEdit->setTextColor(QColor(255, 255, 255));
        QFont textEditFont("Fixedsys");
        textEdit->setPlainText(fileContent);
        textEdit->setFont(textEditFont);
        file.close();
        layout->addRow(imageLabel, textEdit);
        QPushButton* ok = new QPushButton("OK", this);

        int id = QFontDatabase::addApplicationFont(":/font/font/PressStart2P-Regular.ttf");
        QString family = QFontDatabase::applicationFontFamilies(id).at(0);
        QFont pixelFont(family);

        ok->setFont(pixelFont);
        ok->setStyleSheet("QPushButton {"
                          "    background-color: #000000;"
                          "    border: 2px solid #1A1A1A;"
                          "    border-radius: 15px;"
                          "    color: #FFFFFF;"
                          "    font-size: 14px;"
                          "    font-weight: 600;"
                          "    margin: 0;"
                          "    min-height: 20px;"
                          "    padding: 16px 24px;"
                          "    text-align: center;"
                          "    transition: all 300ms cubic-bezier(.23, 1, 0.32, 1);"
                          "    width: 100%;"
                          "}"
                          "QPushButton:hover {"
                          "    background-color: #333333;"
                          "    border: 2px solid #555555;"
                          "    box-shadow: 0 8px 15px rgba(0, 0, 0, 0.25);"
                          "    margin: 0 0 6px 0;"
                          "}"
                          "QPushButton:pressed {"
                              "background-color: #1A1A1A;"
                                "border: 2px solid #333333;"
                              "box-shadow: 0 4px 8px rgba(0, 0, 0, 0.5);"
                          "margin: 0 0 6px 0;"
                          "}"
                          "QPushButton {"
                          "    outline: none;"
                          "}");
        connect(ok, &QPushButton::clicked, this, [this, &sound](){
            sound->setPosition(0);
            sound->play();
            this->close();
        });
        layout->addRow(ok);
        setLayout(layout);
    }
};

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenu)
    ,data(QSqlDatabase::database("MainConnection"))
    , id (QFontDatabase::addApplicationFont(":/font/font/PressStart2P-Regular.ttf"))
    ,family(QFontDatabase::applicationFontFamilies(id).at(0))
    ,pixelFont(family)
    ,music(new QMediaPlayer(this))
    ,sound(new QMediaPlayer(this))
    ,audioMusic(new QAudioOutput(this))
    ,audioSound(new QAudioOutput(this))

{
    ui->setupUi(this);
    this->setWindowTitle("Arcades");

    ui->pushButton->setFont(pixelFont);
    ui->pushButton_2->setFont(pixelFont);
    ui->pushButton_3->setFont(pixelFont);

    music->setAudioOutput(audioMusic);
    sound->setAudioOutput(audioSound);
    music->setSource(QUrl("qrc:/sound/sound/Eric Skiff - Underclocked.mp3"));
    sound->setSource(QUrl("qrc:/sound/sound/button-124476.mp3"));



    QSettings settings("Tompavlo", "Arcades");

    audioSound->setVolume((settings.value("soundVolume", 50).toFloat())/100);
    audioMusic->setVolume((settings.value("musicVolume", 50).toFloat())/100);

    music->play();

    QPixmap userIcon(":/icon/icons/profile-user.png");
    ClickableLabel* clickableLabel= new ClickableLabel(ui->page);
    clickableLabel->setGeometry(8, 610, 72, 96);
    userIcon=userIcon.scaled(clickableLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    clickableLabel->setPixmap(userIcon);

    QPixmap token(":/icon/icons/tocken blue.png");
    ui->icon->setPixmap(token);

    ui->tokenValue->setFont(pixelFont);

    connect(clickableLabel, &ClickableLabel::clicked, this, &MainMenu::onLabelClicked);
    connect(music, &QMediaPlayer::mediaStatusChanged, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            music->play();
        }
    });

    updateTokensView();

    launchGameChoosingPage();


}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::on_pushButton_clicked()
{
    sound->setPosition(0);
    sound->play();
    ui->stackedWidget->setCurrentIndex(1);
    dbLogic();
}

void MainMenu::onLabelClicked(){
    sound->setPosition(0);
    sound->play();
    UsersManipulation* users = new UsersManipulation(this);
    connect(users, &UsersManipulation::tokenValueChanged, this, &MainMenu::updateTokensView);
    users->setModal(true);
    users->show();
}

void MainMenu::onGame1LabelClicked(){
    sound->setPosition(0);
    sound->play();
    QPixmap tiktaktoePhotо(":/icon/icons/tictactoeScreenshot.png");
    QString textFilePath(":/text/text/tik-tak-toe.txt");
    InfoDialog infoTicTacToe(this, tiktaktoePhotо, textFilePath, sound);
    infoTicTacToe.exec();
}

void MainMenu::onGame2LabelClicked(){
    sound->setPosition(0);
    sound->play();
    QPixmap tiktaktoePhotо(":/icon/icons/snake.png");
    QString textFilePath(":/text/text/snake.txt");
    InfoDialog infoTicTacToe(this, tiktaktoePhotо, textFilePath, sound);
    infoTicTacToe.exec();
}

void MainMenu::onGame3LabelClicked(){
    sound->setPosition(0);
    sound->play();
    QPixmap tiktaktoePhotо(":/icon/icons/Typical_Tetris_Game.png");
    QString textFilePath(":/text/text/tetris.txt");
    InfoDialog infoTicTacToe(this, tiktaktoePhotо, textFilePath, sound);
    infoTicTacToe.exec();
}

void MainMenu::updateTokensView(){
    QSettings settings("Tompavlo", "Arcades");
    QString curUser = settings.value("lastChoosenUser", "").toString();
    if(curUser.isEmpty()){
        ui->tokenValue->setText("0");
        return;
    }
    QSqlQuery query(data);
    query.prepare("SELECT tokens FROM users where id = :id");
    query.bindValue(":id", curUser);
    if(query.exec()){
        if(query.next()){
            ui->tokenValue->setText(query.value(0).toString());
        }
    }
}

bool MainMenu::isGamePurchased(QString gameId){
    QSettings settings("Tompavlo", "Arcades");
    QSqlQuery query(data);
    query.prepare("SELECT 1 FROM user_games WHERE user_id= :userId AND game_id = :gameId");
    query.bindValue(":userId", settings.value("lastChoosenUser", "").toString());
    query.bindValue(":gameId", gameId);
    if(query.exec()) return query.next();
    else {
        return true;
    }
}


void MainMenu::on_pushButton_toMainMenu_clicked()
{
    sound->setPosition(0);
    sound->play();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainMenu::launchGameChoosingPage(){
    QPixmap infoIcon(":/icon/icons/info gray.png");
    ClickableLabel* clickableInfo1 = new ClickableLabel(ui->page_2);
    ClickableLabel* clickableInfo2 = new ClickableLabel(ui->page_2);
    ClickableLabel* clickableInfo3 = new ClickableLabel(ui->page_2);

    QSize labelSize(48, 49);

    clickableInfo1->setFixedSize(labelSize);
    clickableInfo2->setFixedSize(labelSize);
    clickableInfo3->setFixedSize(labelSize);

    infoIcon=infoIcon.scaled(clickableInfo1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    clickableInfo1->setPixmap(infoIcon);
    clickableInfo2->setPixmap(infoIcon);
    clickableInfo3->setPixmap(infoIcon);

    ui->horizontalLayout_2->addWidget(clickableInfo2);
    ui->horizontalLayout_4->addWidget(clickableInfo1);
    ui->horizontalLayout_5->addWidget(clickableInfo3);
    ui->horizontalLayout_2->setAlignment(Qt::AlignCenter);
    ui->horizontalLayout_4->setAlignment(Qt::AlignCenter);
    ui->horizontalLayout_5->setAlignment(Qt::AlignCenter);

    connect(clickableInfo1, &ClickableLabel::clicked, this, &MainMenu::onGame1LabelClicked);
    connect(clickableInfo2, &ClickableLabel::clicked, this, &MainMenu::onGame2LabelClicked);
    connect(clickableInfo3, &ClickableLabel::clicked, this, &MainMenu::onGame3LabelClicked);

    ui->pushButton_toMainMenu->setFont(pixelFont);
    ui->pushButton_PlayOrBuy->setFont(pixelFont);
    ui->gameName->setFont(pixelFont);
    ui->gameName_2->setFont(pixelFont);
    ui->gameName_3->setFont(pixelFont);
    ui->pushButton_PlayOrBuy_2->setFont(pixelFont);
    ui->pushButton_PlayOrBuy_3->setFont(pixelFont);
}

void MainMenu::dbLogic(){

    if(isGamePurchased("2")){
        ui->pushButton_PlayOrBuy_2->setText("PLAY");
    }
    else ui->pushButton_PlayOrBuy_2->setText("BUY");

    if(isGamePurchased("3")){
        ui->pushButton_PlayOrBuy_3->setText("PLAY");
    }
    else ui->pushButton_PlayOrBuy_3->setText("BUY");

}


void MainMenu::on_pushButton_PlayOrBuy_clicked()
{
    sound->setPosition(0);
    sound->play();
    QThread::msleep(300);
    music->stop();
    this->close();
    TicTacToe* tictactoe = new TicTacToe;
    tictactoe->show();

}


void MainMenu::on_pushButton_PlayOrBuy_2_clicked()
{
    sound->setPosition(0);
    sound->play();
    if(ui->pushButton_PlayOrBuy_2->text()=="BUY"){
        if(QMessageBox::Yes == QMessageBox::question(nullptr, "Buy confirmation",
                                                      "Price of the game is 100 tokens. Confirm please.",
                                                      QMessageBox::Yes | QMessageBox::No)){
            int tokensAmount = ui->tokenValue->text().toInt();
            if(tokensAmount<100){
                QMessageBox::warning(nullptr, "Error", "Not enough tokens.");
                return;
            }
            else {
                QSettings settings("Tompavlo", "Arcades");
                QSqlQuery query(data);
                query.prepare("UPDATE users SET tokens = :newTokens WHERE id = :id");
                query.bindValue(":newTokens", tokensAmount-100);
                query.bindValue(":id", settings.value("lastChoosenUser", ""));
                query.exec();
                query.prepare("INSERT INTO user_games (user_id, game_id) VALUES (:user_id, :game_id)");
                query.bindValue(":user_id", settings.value("lastChoosenUser", ""));
                query.bindValue(":game_id", 2);
                query.exec();
                QMessageBox::information(nullptr,"Purchase Successful", "You have purchased the game. Have fun!");
                ui->pushButton_PlayOrBuy->setText("PLAY");
                updateTokensView();
            }
        }
    }
    else {

    }
}


void MainMenu::on_pushButton_PlayOrBuy_3_clicked()
{
    sound->setPosition(0);
    sound->play();
    if(ui->pushButton_PlayOrBuy_3->text()=="BUY"){
        if(QMessageBox::Yes == QMessageBox::question(nullptr, "Buy confirmation",
                                                      "Price of the game is 500 tokens. Confirm please.",
                                                      QMessageBox::Yes | QMessageBox::No)){
            int tokensAmount = ui->tokenValue->text().toInt();
            if(tokensAmount<500){
                QMessageBox::warning(nullptr, "Error", "Not enough tokens.");
                return;
            }
            else {
                QSettings settings("Tompavlo", "Arcades");
                QSqlQuery query(data);
                query.prepare("UPDATE users SET tokens = :newTokens WHERE id = :id");
                query.bindValue(":newTokens", tokensAmount-500);
                query.bindValue(":id", settings.value("lastChoosenUser", ""));
                query.exec();
                query.prepare("INSERT INTO user_games (user_id, game_id) VALUES (:user_id, :game_id)");
                query.bindValue(":user_id", settings.value("lastChoosenUser", ""));
                query.bindValue(":game_id", 3);
                query.exec();
                QMessageBox::information(nullptr,"Purchase Successful", "You have purchased the game. Have fun!");
                ui->pushButton_PlayOrBuy->setText("PLAY");
                updateTokensView();
            }
        }
    }
    else {

    }
}


void MainMenu::on_pushButton_2_clicked()
{
    sound->setPosition(0);
    sound->play();

    QDialog volumeMaster(this);
    QFont pixelFontV2("Fixedsys");
    QSettings settings("Tompavlo", "Arcades");
    QVBoxLayout *layout = new QVBoxLayout;
    QLabel *soundVolume = new QLabel;
    QLabel *musicVolume = new QLabel;
    QSlider *soundSlider = new QSlider(Qt::Horizontal);
    QSlider *musicSlider = new QSlider(Qt::Horizontal);
    QPushButton *close = new QPushButton;
    QLabel *soundLevel = new QLabel;
    QLabel *musicLevel = new QLabel;
    QHBoxLayout *sliderLayout_1 = new QHBoxLayout;
    QHBoxLayout *sliderLayout_2= new QHBoxLayout;

    musicVolume->setText("MUSIC VOLUME");
    soundVolume->setText("SOUND VOLUME");
    musicVolume->setFont(pixelFont);
    soundVolume->setFont(pixelFont);
    musicVolume->setStyleSheet("QLabel { color : white; }");
    soundVolume->setStyleSheet("QLabel { color : white; }");

    soundLevel->setText(settings.value("soundVolume", 50).toString() + " %");
    musicLevel->setText(settings.value("musicVolume", 50).toString() + " %");
    soundLevel->setFont(pixelFontV2);
    musicLevel->setFont(pixelFontV2);
    soundLevel->setStyleSheet("QLabel { color : white; }");
    musicLevel->setStyleSheet("QLabel { color : white; }");
    musicLevel->setMinimumWidth(40);
    soundLevel->setMinimumWidth(40);

    soundSlider->setValue(settings.value("soundVolume", 50).toInt());
    musicSlider->setValue(settings.value("musicVolume", 50).toInt());

    musicSlider->setRange(0, 100);
    soundSlider->setRange(0, 100);

    sliderLayout_1->addWidget(musicSlider);
    sliderLayout_1->addWidget(musicLevel);
    sliderLayout_2->addWidget(soundSlider);
    sliderLayout_2->addWidget(soundLevel);

    layout->addWidget(musicVolume);
    layout->addLayout(sliderLayout_1);
    layout->addWidget(soundVolume);
    layout->addLayout(sliderLayout_2);
    layout->addWidget(close);
    close->setFont(pixelFont);

    close->setText("CLOSE");

    volumeMaster.resize(300,150);
    close->setStyleSheet(("QPushButton {"
                          "    background-color: #000000;"
                          "    border: 2px solid #1A1A1A;"
                          "    border-radius: 15px;"
                          "    color: #FFFFFF;"
                          "    font-size: 14px;"
                          "    font-weight: 600;"
                          "    margin: 0;"
                          "    min-height: 16px;"
                          "    padding: 16px 24px;"
                          "    text-align: center;"
                          "    transition: all 300ms cubic-bezier(.23, 1, 0.32, 1);"
                          "    width: 100%;"
                          "}"
                          "QPushButton:hover {"
                          "    background-color: #333333;"
                          "    border: 2px solid #555555;"
                          "    box-shadow: 0 8px 15px rgba(0, 0, 0, 0.25);"
                          "    margin: 0 0 4px 0;"
                          "}"
                          "QPushButton:pressed {"
                          "background-color: #1A1A1A;"
                          "border: 2px solid #333333;"
                          "box-shadow: 0 4px 8px rgba(0, 0, 0, 0.5);"
                          "margin: 0 0 2px 0;"
                          "}"
                          "QPushButton {"
                          "    outline: none;"
                          "}"));
    volumeMaster.setLayout(layout);
    volumeMaster.setWindowTitle("Sound settings");

    connect(close, &QPushButton::clicked, &volumeMaster, [this, &volumeMaster](){
        sound->setPosition(0);
        sound->play();
        volumeMaster.close();
    });

    connect(musicSlider, &QSlider::valueChanged, musicLevel, [this, musicLevel, &settings](int value){
        musicLevel->setText(QString::number(value) +" %");
        settings.setValue("musicVolume", value);
        audioMusic->setVolume((settings.value("musicVolume", 50).toFloat())/100);
    });

    connect(soundSlider, &QSlider::valueChanged, soundLevel, [this, soundLevel, &settings](int value){
        soundLevel->setText(QString::number(value) + " %");
        settings.setValue("soundVolume", value);
        audioSound->setVolume((settings.value("soundVolume", 50).toFloat())/100);
    });


    volumeMaster.exec();

}


void MainMenu::on_pushButton_3_clicked()
{
    sound->setPosition(0);
    sound->play();
    QThread::msleep(300);
    this->close();
}

