#ifndef USERSMANIPULATION_H
#define USERSMANIPULATION_H

#include <QtMultimedia/QtMultimedia>
#include <QDialog>
#include <QtSql>

namespace Ui {
class UsersManipulation;
}

class UsersManipulation : public QDialog
{
    Q_OBJECT

public:
    explicit UsersManipulation(QWidget *parent = nullptr);
    ~UsersManipulation();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

signals:
    void tokenValueChanged();

private:
    Ui::UsersManipulation *ui;
    QSqlTableModel* model;
    QSqlDatabase data;
    QMediaPlayer* sound;
    QAudioOutput* audioSound;

};

#endif // USERSMANIPULATION_H
