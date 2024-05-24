#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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

    ui->imagelabel->setPixmap(QPixmap(":/pic/1.jpg"));
    ui->imagelabel->setScaledContents(true);

    QString dt = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss A ddd");
    ui->timeEdit->setText(dt);

    timer = new QTimer;
    timer->start(1000);  //启动定时器，设置超时时间1s， 单位毫秒
    connect(timer, &QTimer::timeout, [=](){
        QString dt = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss A ddd");
        ui->timeEdit->setText(dt);
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_add_goods_clicked()
{
    QString name = ui->nameEdit->text(),name1;
    QString num = ui->numEdit->text();
    QString id = ui->idEdit->text(),id1;
    QString class1 = ui->classEdit->text();
    QString price = ui->priceEdit->text();


    if (!(name.isEmpty() || num.isEmpty() || id.isEmpty() || class1.isEmpty()
          || price.isEmpty())){
        QString sql = QString("select * from goods where name='%1'").arg(name);
        if(!query->exec(sql)){
            qDebug() << "select table: " << query->lastError().text();
            return;
        }
        while (query->next()) {
            name1 = query->value(0).toString();
            if(name1 == name){
                QMessageBox::warning(this, "增加商品", "商品名称已经存在，请重新创建");
                return;
            }
        }

        sql = QString("select * from goods where id='%1'").arg(id);
        if(!query->exec(sql)){
            qDebug() << "select table: " << query->lastError().text();
            return;
        }
        while (query->next()) {
            id1 = query->value(3).toString();
            if(id1 == id){
                QMessageBox::warning(this, "增加商品", "商品id已经存在，请重新创建");
                return;
            }
        }

        sql = QString("insert into goods values('%1','%2','%3','%4','%5')").arg(name,class1,num,id,price);
        if(!query->exec(sql)){
            qDebug() << "insert table: " << query->lastError().text();
            return;
        }
        QMessageBox::warning(this, "增加商品信息", "增加商品成功！");
        return;
    }else{
        QMessageBox::warning(this, "增加商品", "请输入完整信息！所有信息都必须全部输入！");
        return;
    }
}

void MainWindow::on_delete_goods_clicked()
{
    QString name,id, sql;
    if(ui->nameEdit->text().isEmpty() && ui->idEdit->text().isEmpty()){
        QMessageBox::warning(this, "删除商品", "删除商品时名称、编号其中之一必须存在！");
        return;
    }

    if(!(ui->nameEdit->text().isEmpty())){
        name = ui->nameEdit->text();
        sql = QString("select from goods where name='%1'").arg(name);
        query->exec(sql);
        if (!query->isNull(NULL)){
            QMessageBox::warning(this, "删除", "没有该商品，请输入正确的待删除信息！");
            return;
        }
        sql = QString("delete from goods where name='%1'").arg(name);
        if (!query->exec(sql)) {
            qDebug() << "delete table: " << query->lastError().text();
            return;
        }

        QMessageBox::warning(this, "删除数据", "删除商品成功！");
        return;
    }else if(!(ui->idEdit->text().isEmpty())){
        id = ui->idEdit->text();
        sql = QString("delete from goods where id='%1'").arg(id);
        if (!query->exec(sql)) {
            qDebug() << "delete table: " << query->lastError().text();
            return;
        }
        QMessageBox::warning(this, "删除数据", "删除商品成功！");
        return;
    }


}

void MainWindow::on_show_allgoods_clicked()
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

void MainWindow::on_change_goods_clicked()
{

    QString name = ui->nameEdit->text();
    QString id = ui->idEdit->text();

    QString newnum = ui->numEdit->text();
    QString newclass = ui->classEdit->text();
    QString newprice = ui->priceEdit->text(), sql;
    QString num, price, class1;
    if(ui->nameEdit->text().isEmpty() && ui->idEdit->text().isEmpty()){
        QMessageBox::warning(this, "修改商品", "修改商品信息时名称、编号其中之一必须存在！");
        return;
    }

    if(!name.isEmpty()){
        sql = QString("select * from goods where name='%1'").arg(name);
        if (!query->exec(sql)) {
            qDebug() << "update class: " << query->lastError().text();
            return;
        }
        while (query->next()) {
            class1 = query->value(1).toString();
            num = query->value(2).toString();
            price = query->value(4).toString();
        }

        if(!newclass.isEmpty())
        {
            sql = QString("update goods set class='%1' where class='%2'").arg(newclass,class1);
            if (!query->exec(sql)) {
                qDebug() << "update class: " << query->lastError().text();
                return;
            }
        }

        if(!newnum.isEmpty())
        {
            sql = QString("update goods set num='%1' where num='%2'").arg(newnum, num);
            if (!query->exec(sql)) {
                qDebug() << "update num: " << query->lastError().text();
                return;
            }
        }

        if(!newprice.isEmpty())
        {
            sql = QString("update goods set price='%1' where price='%2'").arg(newprice, price);
            if (!query->exec(sql)) {
                qDebug() << "select price: " << query->lastError().text();
                return;
            }
        }

        QMessageBox::warning(this, "修改数据", "修改商品数据成功！");
        return;
    }else if(!id.isEmpty()){
        sql = QString("select * from goods where id='%1'").arg(id);
        if (!query->exec(sql)) {
            qDebug() << "update class: " << query->lastError().text();
            return;
        }
        while (query->next()) {
            class1 = query->value(1).toString();
            num = query->value(2).toString();
            price = query->value(4).toString();
        }

        if(!newclass.isEmpty())
        {
            sql = QString("update goods set class='%1' where class='%2'").arg(newclass,class1);
            if (!query->exec(sql)) {
                qDebug() << "update class: " << query->lastError().text();
                return;
            }
        }

        if(!newnum.isEmpty())
        {
            sql = QString("update goods set num='%1' where num='%2'").arg(newnum, num);
            if (!query->exec(sql)) {
                qDebug() << "update num: " << query->lastError().text();
                return;
            }
        }

        if(!newprice.isEmpty())
        {
            sql = QString("update goods set price='%1' where price='%2'").arg(newprice, price);
            if (!query->exec(sql)) {
                qDebug() << "select price: " << query->lastError().text();
                return;
            }
        }

        QMessageBox::warning(this, "修改数据", "修改商品数据成功！");
        return;
    }

}


void MainWindow::on_select_goods_clicked()
{
    QString name = ui->nameEdit->text();
    QString num = ui->numEdit->text();
    QString id = ui->idEdit->text();
    QString class1 = ui->classEdit->text();
    QString price = ui->priceEdit->text(), text, sql;

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

    if(!class1.isEmpty()){
        sql = QString("select * from goods where class='%1'").arg(class1);
        if (!query->exec(sql)) {
            qDebug() << "select class: " << query->lastError().text();
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

        }
        return;
    }

    if(!price.isEmpty()){
        sql = QString("select * from goods where price='%1'").arg(price);
        if (!query->exec(sql)) {
            qDebug() << "select class: " << query->lastError().text();
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
        }
        return;
    }

    if(!num.isEmpty()){
        sql = QString("select * from goods where num='%1'").arg(num);
        if (!query->exec(sql)) {
            qDebug() << "select class: " << query->lastError().text();
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
        }
        return;
    }

}

void MainWindow::on_pushButton_clicked()
{
    ui->nameEdit->clear();
    ui->idEdit->clear();
    ui->classEdit->clear();
    ui->numEdit->clear();
    ui->priceEdit->clear();
}

void MainWindow::on_pushButton_2_clicked()
{
    sellerWidget *s = new sellerWidget;
    s->show();
    this->hide();
}
