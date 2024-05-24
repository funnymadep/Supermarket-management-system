#ifndef SELLERWIDGET_H
#define SELLERWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include "mainwindow.h"
#include "image.h"
namespace Ui {
class sellerWidget;
}

class sellerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit sellerWidget(QWidget *parent = nullptr);
    ~sellerWidget();

private slots:
    void on_cal_pushButton_clicked();

    void on_payedit_clicked();

    void on_returnButton_clicked();

    void on_showbutton_clicked();

    void on_selectbutton_clicked();

    void on_pushButton_clicked();

signals:
    void return_button();

private:
    Ui::sellerWidget *ui;

    QSqlQuery *query;
};

#endif // SELLERWIDGET_H
