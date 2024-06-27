#include "tictactoe.h"
#include "ui_tictactoe.h"
#include <QFontDatabase>
#include "mainmenu.h"
#include <QButtonGroup>
#include <QGridLayout>
#include "stylehelper.h"
#include <ctime>

TicTacToe::TicTacToe(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TicTacToe)
    , id (QFontDatabase::addApplicationFont(":/font/font/PressStart2P-Regular.ttf"))
    ,family(QFontDatabase::applicationFontFamilies(id).at(0))
    ,pixelFont(family)
    ,sound(new QMediaPlayer(this))
    ,audioSound(new QAudioOutput)
    ,settings("Tompavlo", "Arcades")
    ,db(QSqlDatabase::database("MainConnection"))
    ,query(db)

{
    srand(time(nullptr));
    ui->setupUi(this);
    ui->label_gameName->setFont(pixelFont);
    ui->pushButton_start->setFont(pixelFont);
    ui->pushButton->setFont(pixelFont);
    ui->pushButton_0->setFont(pixelFont);
    ui->pushButton_x->setFont(pixelFont);
    ui->pushButton_1->setFont(pixelFont);
    ui->pushButton_2->setFont(pixelFont);
    ui->pushButton_3->setFont(pixelFont);
    ui->pushButton_4->setFont(pixelFont);
    ui->pushButton_5->setFont(pixelFont);
    ui->pushButton_6->setFont(pixelFont);
    ui->pushButton_7->setFont(pixelFont);
    ui->pushButton_7->setFont(pixelFont);
    ui->pushButton_8->setFont(pixelFont);
    ui->pushButton_9->setFont(pixelFont);
    ui->label_whichTurn->setFont(pixelFont);

    ui->pushButton_1->setDisabled(true);



    ui->pushButton_0->setCheckable(true);
    ui->pushButton_x->setCheckable(true);
    ui->pushButton_x->setChecked(true);

    sound->setAudioOutput(audioSound);
    sound->setSource(QUrl("qrc:/sound/sound/button-124476.mp3"));
    QSettings settings("Tompavlo", "Arcades");
    audioSound->setVolume((settings.value("soundVolume", 50).toFloat())/100);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);
    buttonGroup->addButton(ui->pushButton_0);
    buttonGroup->addButton(ui->pushButton_x);

    connect(buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onButtonClicked(QAbstractButton*)));
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TicTacToe::computerLogic);
    configurationGameAreaButtons();
    this->setWindowTitle("Tic-tac-toe");
}


TicTacToe::~TicTacToe()
{
    delete ui;
}

void TicTacToe::on_pushButton_clicked()
{
    sound->setPosition(0);
    sound->play();
    this->close();
    MainMenu *w = new MainMenu;
    w->show();

}


void TicTacToe::on_pushButton_start_clicked()
{
    sound->setPosition(0);
    sound->play();
    if(gameStart){
        playerLocked=true;
        timer->stop();
        ui->pushButton_start->setText("START");
        ui->pushButton_0->setDisabled(false);
        ui->pushButton_x->setDisabled(false);
        gameStart=false;
        ui->label_whichTurn->setText("YOU LOSE");

    }
    else{
        start();
        lockPlayer();
        ui->pushButton_start->setText("SURRENDER");
        ui->pushButton_0->setDisabled(true);
        ui->pushButton_x->setDisabled(true);
    }
}

void TicTacToe::change_button_grid(int row, int column, QString style, QString text, bool disable)
{
    QPushButton *btn = qobject_cast<QPushButton*>( ui->gridLayout->itemAtPosition(row, column)->widget());
    btn->setDisabled(disable);
    btn->setStyleSheet(style);
    btn->setText(text);

}

void TicTacToe::resetGrid()
{
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            change_button_grid(i, j, StyleHelper::getFielDefaultStyle(), "", false);
        }
    }
}

void TicTacToe::start()
{
    resetGrid();
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            gameArea[i][j]='-';
        }
    }
    progress = 0;
    gameStart = true;
    stop=false;
    winner='-';
    if(player=='O'){
        computerTurn();
    }
    else ui->label_whichTurn->setText("YOUR TURN");

}

void TicTacToe::onButtonClicked(QAbstractButton *button)
{

    if (button == ui->pushButton_0) {
        player = 'O';

    } else if (button == ui->pushButton_x) {
        player = 'X';
    }
}

void TicTacToe::configurationGameAreaButtons()
{
    for(int row=0; row<3; row++){
        for(int column=0; column<3; column++){
            QPushButton *btn = qobject_cast<QPushButton*>(ui->gridLayout->itemAtPosition(row, column)->widget());
            btn->setProperty("row", row);
            btn->setProperty("column", column);
            connect(btn, &QPushButton::clicked, this, &TicTacToe::onGameAreaButtonClicked);
        }
    }
}

void TicTacToe::onGameAreaButtonClicked()
{   if(!playerLocked){
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    int row = btn->property("row").toInt();
    int column = btn->property("column").toInt();
    if(player=='X'){
        change_button_grid(row, column, StyleHelper::getFielDefaultStyle(),"X", true);
        gameArea[row][column]='X';
    }
    else {
        change_button_grid(row, column, StyleHelper::getFielDefaultStyle(),"O", true);
        gameArea[row][column]='O';
    }
    playerLocked = true;
    progress++;
    checkGameEnd();
    gameEnd();
    computerTurn();
    }
}

void TicTacToe::lockPlayer()
{
    if(player=='X'){
        playerLocked=false;
    }
    else playerLocked=true;
}

void TicTacToe::computerTurn()
{
    if(stop) return;
    ui->label_whichTurn->setText("AI TURN");
    timer->start(2000);


}

void TicTacToe::computerLogic()
{
    timer->stop();
    QString comp;
    if(player=='X') comp ="O";
    else comp = "X";
    while(true){
        int r = rand()%3;
        int c = rand()%3;
            if(gameArea[r][c]=='-'){
                if(player=='X') gameArea[r][c]='O';
                else gameArea[r][c]='X';
                change_button_grid(r, c, StyleHelper::getFielDefaultStyle(), comp, true);
                ui->label_whichTurn->setText("YOUR TURN");
                progress++;
                checkGameEnd();
                gameEnd();
                playerLocked=false;
                return;
            }
    }
}

void TicTacToe::checkGameEnd()
{
    char symbols[2] = {'X', 'O'};
    for (int i = 0; i < 2; i++) {
        for (int row = 0; row < 3; row++) {
            if ((gameArea[row][0] == symbols[i]) && (gameArea[row][1] == symbols[i]) && (gameArea[row][2] == symbols[i])) {
                stop = true;
                QString symbol=QChar(symbols[i]);
                winner = symbols[i];
                if(winner==player){
                    query.prepare("UPDATE users SET tokens = 5 WHERE id = :id");
                    query.bindValue(":id", settings.value("lastChoosenUser", ""));
                    query.exec();
                    change_button_grid(row,0,StyleHelper::getFieldWinStyle(),symbol, true);
                    change_button_grid(row,1,StyleHelper::getFieldWinStyle(),symbol, true);
                    change_button_grid(row,2,StyleHelper::getFieldWinStyle(),symbol, true);
                }
                else{
                    change_button_grid(row,0,StyleHelper::getFieldLostStyle(),symbol, true);
                    change_button_grid(row,1,StyleHelper::getFieldLostStyle(),symbol, true);
                    change_button_grid(row,2,StyleHelper::getFieldLostStyle(),symbol, true);
                }
                return;
            }
        }
        for (int col = 0; col < 3; col++) {
            if ((gameArea[0][col] == symbols[i]) && (gameArea[1][col] == symbols[i]) && (gameArea[2][col] == symbols[i])) {
                stop = true;
                QString symbol=QChar(symbols[i]);
                winner = symbols[i];
                if(winner==player){
                    query.prepare("UPDATE users SET tokens = 5 WHERE id = :id");
                    query.bindValue(":id", settings.value("lastChoosenUser", ""));
                    query.exec();
                    change_button_grid(0,col,StyleHelper::getFieldWinStyle(),symbol, true);
                    change_button_grid(1,col,StyleHelper::getFieldWinStyle(),symbol, true);
                    change_button_grid(2,col,StyleHelper::getFieldWinStyle(),symbol, true);
                }
                else{
                    change_button_grid(0,col,StyleHelper::getFieldLostStyle(),symbol, true);
                    change_button_grid(1,col,StyleHelper::getFieldLostStyle(),symbol, true);
                    change_button_grid(2,col,StyleHelper::getFieldLostStyle(),symbol, true);
                }
                return;
            }
        }
        if ((gameArea[0][0] == symbols[i]) && (gameArea[1][1] == symbols[i]) && (gameArea[2][2] == symbols[i])) {
            stop = true;
            QString symbol=QChar(symbols[i]);
            winner = symbols[i];
            if(winner==player){
                query.prepare("UPDATE users SET tokens = 5 WHERE id = :id");
                query.bindValue(":id", settings.value("lastChoosenUser", ""));
                query.exec();
                change_button_grid(0,0,StyleHelper::getFieldWinStyle(),symbol, true);
                change_button_grid(1,1,StyleHelper::getFieldWinStyle(),symbol, true);
                change_button_grid(2,2,StyleHelper::getFieldWinStyle(),symbol, true);
            }
            else{
                change_button_grid(0,0,StyleHelper::getFieldLostStyle(),symbol, true);
                change_button_grid(1,1,StyleHelper::getFieldLostStyle(),symbol, true);
                change_button_grid(2,2,StyleHelper::getFieldLostStyle(),symbol, true);
            }
            return;
        }
        if ((gameArea[0][2] == symbols[i]) && (gameArea[1][1] == symbols[i]) && (gameArea[2][0] == symbols[i])) {
            stop = true;
            QString symbol=QChar(symbols[i]);
            winner = symbols[i];
            if(winner==player){
                query.prepare("UPDATE users SET tokens = 5 WHERE id = :id");
                query.bindValue(":id", settings.value("lastChoosenUser", ""));
                query.exec();
                change_button_grid(0,2,StyleHelper::getFieldWinStyle(),symbol, true);
                change_button_grid(1,1,StyleHelper::getFieldWinStyle(),symbol, true);
                change_button_grid(2,0,StyleHelper::getFieldWinStyle(),symbol, true);
            }
            else{
                change_button_grid(0,2,StyleHelper::getFieldLostStyle(),symbol, true);
                change_button_grid(1,1,StyleHelper::getFieldLostStyle(),symbol, true);
                change_button_grid(2,0,StyleHelper::getFieldLostStyle(),symbol, true);
            }
            return;
        }
    }
    if(progress==9){
        stop = true;
        query.prepare("UPDATE users SET tokens = 2 WHERE id = :id");
        query.bindValue(":id", settings.value("lastChoosenUser", ""));
        query.exec();
    }
}

void TicTacToe::gameEnd()
{
    if(stop){
        if(winner==player){
            ui->label_whichTurn->setText("WINNER");
        }
        else if(winner=='-') {
            ui->label_whichTurn->setText("DRAW");
        }
        else{
            ui->label_whichTurn->setText("LOSER");
        }
        ui->pushButton_start->setText("START");
        ui->pushButton_0->setDisabled(false);
        ui->pushButton_x->setDisabled(false);
        gameStart=false;
    }
}
