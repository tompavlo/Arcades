#include "mainmenu.h"
#include "./ui_mainmenu.h"
#include <QFontDatabase>
#include <QtSql>
#include "clickablelabel.h"
#include "usersmanipulation.h"
#include <QMessageBox>
#include <QFormLayout>
#include <QTextEdit>

class InfoDialog : public QDialog{
public:
    InfoDialog(QWidget *parent= nullptr, const QPixmap &image = QPixmap(), const  QString &filePath=QString()) : QDialog(parent){
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
        connect(ok, &QPushButton::clicked, this, &QDialog::close);
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

{
    ui->setupUi(this);
    this->setWindowTitle("Arcades");

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

    launchGameChoosingPage();

}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    dbLogic();
}

void MainMenu::onLabelClicked(){
    UsersManipulation* users = new UsersManipulation(this);
    connect(users, &UsersManipulation::tokenValueChanged, this, &MainMenu::updateTokensView);
    users->setModal(true);
    users->show();
}

void MainMenu::onGame1LabelClicked(){
    QPixmap tiktaktoePhotо(":/icon/icons/tictactoe.png");
    QString textFilePath(":/text/text/tik-tak-toe.txt");
    InfoDialog infoTicTacToe(this, tiktaktoePhotо, textFilePath);
    infoTicTacToe.exec();
}

void MainMenu::onGame2LabelClicked(){
    QPixmap tiktaktoePhotо(":/icon/icons/tictactoe.png");
    QString textFilePath(":/text/text/snake.txt");
    InfoDialog infoTicTacToe(this, tiktaktoePhotо, textFilePath);
    infoTicTacToe.exec();
}

void MainMenu::onGame3LabelClicked(){
    QPixmap tiktaktoePhotо(":/icon/icons/tictactoe.png");
    QString textFilePath(":/text/text/tetris.txt");
    InfoDialog infoTicTacToe(this, tiktaktoePhotо, textFilePath);
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

}


void MainMenu::on_pushButton_PlayOrBuy_2_clicked()
{
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

