#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
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
    QString sql = "create table if not exists user(name text, pswd text)";
    if (!query->exec(sql)) {
        qDebug() << "create table: " << query->lastError().text();
        return;
    }

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_reg_pushButton_clicked()
{
    QString name = ui->lineEdit->text();
    QString pswd = ui->lineEdit_2->text();
    if (name.isEmpty() || pswd.isEmpty()) {
        QMessageBox::warning(this, "注册", "账号与密码都不能为空！");
        return;
    }
    QString sql = QString("insert into user values('%1', '%2')").arg(name, pswd);
    QString sql1 = QString("select * from user where name = '%1'").arg(name);
    if (!query->exec(sql1)){
        qDebug() << "insert table: " << query->lastError().text();
        return;
    }
    while(query->next()){
        if(query->value(0).toString() == name){
            QMessageBox::warning(this, "注册", "账号已存在");
            return;
        }
    }
    if (!query->exec(sql)) {
        qDebug() << "insert table: " << query->lastError().text();
        return;
    }
    QMessageBox::warning(this, "注册", "注册成功！");
}

void Widget::on_login_pushButton_clicked()
{
    if (!ui->lineEdit_2->text().isEmpty()){
        QString sql = QString("select pswd from user where name='" + ui->lineEdit->text() + "'");
        if (!query->exec(sql)) {
            qDebug() << "insert table: " << query->lastError().text();
            return;
        }
        query->next();
        if (ui->lineEdit_2->text() == query->value(0).toString()) {
            MainWindow *mainW = new MainWindow;
            mainW->show();
            this->hide();
        }else{
            QMessageBox::warning(this, "登录","账号或密码错误！");
            //清空输入框
            ui->lineEdit_2->clear();
        }
    }

}
