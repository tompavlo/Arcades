#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <QTimer>
#include <qabstractbutton.h>
#include <QMediaPlayer>
#include <QDialog>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class TicTacToe;
}

class TicTacToe : public QDialog
{
    Q_OBJECT

public:
    explicit TicTacToe(QWidget *parent = nullptr);
    ~TicTacToe();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_start_clicked();

    void change_button_grid(int row, int column, QString style, QString text, bool disable);

    void resetGrid();

    void start();

    void onButtonClicked(QAbstractButton *button);

    void configurationGameAreaButtons();

    void onGameAreaButtonClicked();

    void lockPlayer();

    void computerTurn();

    void computerLogic();

    void checkGameEnd();

    void gameEnd();



private:
    Ui::TicTacToe *ui;
    int id;
    QString family;
    QFont pixelFont;
    QMediaPlayer* sound;
    QAudioOutput* audioSound;
    char gameArea[3][3] = {
        {'-', '-', '-'},
        {'-', '-', '-'},
        {'-', '-', '-'}
    };
    char player = 'X';
    int progress = 0;
    bool gameStart = false;
    bool playerLocked = true;
    QTimer* timer;
    bool stop;
    char winner;
    QSettings settings;
    QSqlDatabase db;
    QSqlQuery query;

};

#endif // TICTACTOE_H
