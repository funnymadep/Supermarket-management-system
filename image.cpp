#include "image.h"
#include "ui_image.h"

image::image(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::image)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");   //指定使用的哪种数据库
    db.setDatabaseName("my.db");  //设置数据库名
    if (!db.open()) {   //打开数据库
        qDebug() << "open sql: " << db.lastError().text();  //获取错误信息
        return;
    }

    query = new QSqlQuery(db);

    //创建表
    QString sql = "create table if not exists goods(id text, name text, price text)";
    if (!query->exec(sql)) {
        qDebug() << "create table: " << query->lastError().text();
        return;
    }

    manager = new QNetworkAccessManager;

    connect(manager, &QNetworkAccessManager::finished, [=](QNetworkReply *reply){
           QByteArray data = reply->readAll();
           //ui->textEdit->setText(data);

           //Json解析
           QJsonDocument doc = QJsonDocument::fromJson(data);
           QJsonObject obj = doc.object();
           if (obj.contains("codes_result")) {  //判读是否包含某个键
               QJsonArray arr = obj.value("codes_result").toArray();
               obj = arr.at(0).toObject();
               if (obj.contains("text")) {
                   id =  obj.value("text").toArray().at(0).toString();
               }
           }
           QString sql = QString("select * from goods where id='%1'").arg(id), text;
           if (!query->exec(sql)) {
               qDebug() << "select id: " << query->lastError().text();
               return;
           }
           while (query->next()) {
               name = query->value(0).toString();
               CLASS = query->value(1).toString();
               num = query->value(2).toString();
               id = query->value(3).toString();
               price = query->value(4).toString();
               text = "名称:" + name + " 类别:" + CLASS + " 数量:" + num + " ID:" +
                       id + " 价格: "  + price;
               ui->resultEdit->setText(text);
           }

           ui->nameEdit->setText(name);

    });
}

image::~image()
{
    delete ui;
}

void image::on_select_button_clicked()
{
    filename = QFileDialog::getOpenFileName(this, "打开图片",
                                "../market_controller", "Images (*.jpg *.png *.bmp)");
    if (filename.isEmpty())
        return;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)){
        qDebug() << "error!";
        return;
    }
    ui->image_label->setPixmap(QPixmap(filename));
    img = file.readAll();
    file.close();
    ui->reg_button->setEnabled(true);
}

void image::on_reg_button_clicked()
{
    ui->resultEdit->clear();
    QString url = "https://aip.baidubce.com/rest/2.0/ocr/v1/qrcode?access_token=24.8f2c527c457ef60d4479c9503a1cdc39.2592000.1697100280.282335-39220756";
    QNetworkRequest req;
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setUrl(QUrl(url));

    QByteArray body = "image=" + img.toBase64().toPercentEncoding();
    manager->post(req, body);

}

void image::on_pay_button_clicked()
{
    QString num1 = ui->numEdit->text(), sql;
    int anum = num1.toInt();
    int num2 = num.toInt();

    if (num2 < anum){
        QMessageBox::warning(this, "支付失败", "货物数量不够,无法支付");
        return;
    }
    num2 -= anum;

    sql = QString("update goods set num='%1' where num='%2'").arg(QString(QString::number(num2)), num);
    if (!query->exec(sql)) {
        qDebug() << "update num: " << query->lastError().text();
        return;
    }

    int aprice = price.toInt();
    aprice *= anum;
    ui->priceEdit->setText(QString::number(aprice) + '￥');
}

void image::on_pushButton_clicked()
{
    sellerWidget *s = new sellerWidget;
    s->show();
    this->hide();
}
