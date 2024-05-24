#ifndef IMAGE_H
#define IMAGE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "sellerwidget.h"

namespace Ui {
class image;
}

class image : public QWidget
{
    Q_OBJECT

public:
    explicit image(QWidget *parent = nullptr);
    ~image();

private slots:


    void on_select_button_clicked();

    void on_reg_button_clicked();

    void on_pay_button_clicked();

    void on_pushButton_clicked();

private:
    Ui::image *ui;

    QSqlQuery *query;
    QNetworkAccessManager *manager;
    QString filename, name, id, CLASS, num, price;
    QByteArray img;
};

#endif // IMAGE_H
