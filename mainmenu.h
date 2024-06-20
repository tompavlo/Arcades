#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainMenu;
}
QT_END_NAMESPACE

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private slots:
    void on_pushButton_clicked();
    void onLabelClicked();
    void updateTokensView();
    bool isGamePurchased(QString gameId);
    void launchGameChoosingPage();
    void on_pushButton_toMainMenu_clicked();
    void dbLogic();
    void onGame1LabelClicked();
    void onGame2LabelClicked();
    void onGame3LabelClicked();


    void on_pushButton_PlayOrBuy_clicked();

    void on_pushButton_PlayOrBuy_2_clicked();

    void on_pushButton_PlayOrBuy_3_clicked();

private:
    Ui::MainMenu *ui;
    QSqlDatabase data;
    int id;
    QString family;
    QFont pixelFont;
};
#endif // MAINMENU_H
