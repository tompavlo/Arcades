#ifndef FIRSTINITIALISATION_H
#define FIRSTINITIALISATION_H

#include <QDialog>

namespace Ui {
class firstInitialisation;
}

class firstInitialisation : public QDialog
{
    Q_OBJECT

public:
    explicit firstInitialisation(QWidget *parent = nullptr);
    ~firstInitialisation();

private slots:
    void on_pushButton_clicked();

private:
    Ui::firstInitialisation *ui;
};

#endif // FIRSTINITIALISATION_H
