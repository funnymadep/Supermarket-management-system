#include "sellerwidget.h"
#include "ui_sellerwidget.h"



sellerWidget::sellerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sellerWidget)
{
    ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("my.db");  //设置数据库名
    if (!db.open()) {   //打开数据库
        qDebug() << "open sql: " << db.lastError().text();  //获取错误信息
        return;
    }
    query = new QSqlQuery(db);
    //创建表
    QString sql = "create table if not exists goods(name text, class text, num text, "
                  "ID text, price text)";
    if (!query->exec(sql)) {
        qDebug() << "create table: " << query->lastError().text();
        return;
    }
}

sellerWidget::~sellerWidget()
{
    delete ui;
}

void sellerWidget::on_cal_pushButton_clicked()
{
    QString num1 = ui->numedit->text();
    QString num2 = ui->numedit->text();
    QString name = ui->nameedit->text();
    QString id = ui->idedit->text(), sql, aprice;
    int price, num, anum;
    if(name.isEmpty() && id.isEmpty()){
        QMessageBox::warning(this, "结算", "结算时名称、编号其中之一必须存在！");
        return;
    }

    if(num1.isEmpty()){
        QMessageBox::warning(this, "结算", "结算时数量必须存在！");
        return;
    }
    anum = num1.toInt();

    if (!name.isEmpty()){
        sql = QString("select * from goods where name='%1'").arg(name);
        if (!query->exec(sql)) {
            qDebug() << "select name: " << query->lastError().text();
            return;
        }

        while (query->next()) {
            num = query->value(2).toInt();
            price = query->value(4).toInt();
        }

        if(num < anum){
            QMessageBox::warning(this, "结算", "该商品数量不够！");
            return;
        }

        price *= anum;
        aprice = QString(QString::number(price)) + "元";
        ui->priceEdit->setText(aprice);

    }

    if(name.isEmpty() && !id.isEmpty()){
        sql = QString("select * from goods where id='%1'").arg(id);
        if (!query->exec(sql)) {
            qDebug() << "select name: " << query->lastError().text();
            return;
        }

        while (query->next()) {
            num = query->value(2).toInt();
            price = query->value(4).toInt();
        }

        if(num2.isEmpty()){
            QMessageBox::warning(this, "结算", "结算时数量必须存在！");
            return;
        }
        anum = num2.toInt();

        if(num < anum){
            QMessageBox::warning(this, "结算", "该商品数量不够！");
            return;
        }


        price *= anum;
        aprice = QString(QString::number(price)) + "元";
        ui->priceEdit->setText(aprice);

    }

}

void sellerWidget::on_payedit_clicked()
{
    QString num1 = ui->numedit->text();
    QString num2 = ui->numedit->text();
    QString name = ui->nameedit->text();
    QString id = ui->idedit->text(), sql, aprice;
    int price, num, anum;
    if(name.isEmpty() && id.isEmpty()){
        QMessageBox::warning(this, "结算", "结算时名称、编号其中之一必须存在！");
        return;
    }

    anum = num1.toInt();

    if (!name.isEmpty()){
        sql = QString("select * from goods where name='%1'").arg(name);
        if (!query->exec(sql)) {
            qDebug() << "select name: " << query->lastError().text();
            return;
        }

        while (query->next()) {
            num = query->value(2).toInt();
            price = query->value(4).toInt();
        }

        anum = num - anum;

        QString newnum = QString("%1").arg(anum);
        QString oldnum = QString("%1").arg(num);
        sql = QString("update goods set num='%1' where num='%2'").arg(newnum, oldnum);
        if (!query->exec(sql)) {
            qDebug() << "update num: " << query->lastError().text();
            return;
        }
    }

    if(name.isEmpty() && !id.isEmpty()){
        sql = QString("select * from goods where id='%1'").arg(id);
        if (!query->exec(sql)) {
            qDebug() << "select name: " << query->lastError().text();
            return;
        }

        anum = num2.toInt();

        while (query->next()) {
            num = query->value(2).toInt();
            price = query->value(4).toInt();
        }

        anum = num - anum;

        QString newnum = QString("%1").arg(anum);
        QString oldnum = QString("%1").arg(num);
        sql = QString("update goods set num='%1' where num='%2'").arg(newnum, oldnum);
        if (!query->exec(sql)) {
            qDebug() << "update num: " << query->lastError().text();
            return;
        }

    }
    QMessageBox::warning(this, "支付", "支付成功！");
    return;

}

void sellerWidget::on_returnButton_clicked()
{
    MainWindow *mainW = new MainWindow;
    mainW->show();
    this->hide();
}

void sellerWidget::on_showbutton_clicked()
{
    ui->textEdit->clear();
    QString sql = QString("select * from goods");
    if (!query->exec(sql)) {
        qDebug() << "insert table: " << query->lastError().text();
        return;
    }
    while (query->next()) {
        QString name = query->value(0).toString();
        QString class1 = query->value(1).toString();
        QString num = query->value(2).toString();
        QString id = query->value(3).toString();
        QString price = query->value(4).toString();
        QString text = name +'|'+ class1+'|' + num +'|'+
                id +'|' + price;
        ui->textEdit->append(text);
    }
}

void sellerWidget::on_selectbutton_clicked()
{
    QString name = ui->nameedit->text();
    QString num = ui->numedit->text();
    QString id = ui->idedit->text();
    QString class1 ,price, text, sql;

    if (name.isEmpty() && id.isEmpty()){
        QMessageBox::warning(this, "查询商品", "查询商品信息时名称、编号其中之一必须存在！");
        return;
    }

    ui->textEdit->clear();
    if (!name.isEmpty()){
        sql = QString("select * from goods where name='%1'").arg(name);
        if (!query->exec(sql)) {
            qDebug() << "select name: " << query->lastError().text();
            return;
        }

        while (query->next()) {
            name = query->value(0).toString();
            class1 = query->value(1).toString();
            num = query->value(2).toString();
            id = query->value(3).toString();
            price = query->value(4).toString();
            text = name +'|'+ class1+'|' + num +'|'+
                    id +'|' + price;
            ui->textEdit->append(text);
            return;
        }
    }

    if(!id.isEmpty()){
        sql = QString("select * from goods where id='%1'").arg(id);
        if (!query->exec(sql)) {
            qDebug() << "select name: " << query->lastError().text();
            return;
        }

        while (query->next()) {
            name = query->value(0).toString();
            class1 = query->value(1).toString();
            num = query->value(2).toString();
            id = query->value(3).toString();
            price = query->value(4).toString();
            text = name +'|'+ class1+'|' + num +'|'+
                    id +'|' + price;
            ui->textEdit->append(text);
            return;
        }
    }


}

void sellerWidget::on_pushButton_clicked()
{
    image *i = new image;
    i->show();
    this->hide();
}
