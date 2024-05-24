#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#include <QTimer>
#include <QDateTimeEdit>
#include "sellerwidget.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_add_goods_clicked();

    void on_delete_goods_clicked();

    void on_show_allgoods_clicked();

    void on_change_goods_clicked();

    void on_select_goods_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;

    QSqlQuery *query;
    QTimer *timer;
};

#endif // MAINWINDOW_H
