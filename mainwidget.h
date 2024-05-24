#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>
#include <QObject>

class mainwidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit mainwidget(QWidget *parent = nullptr);

signals:

};

#endif // MAINWIDGET_H
