#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "receiver.h"
#include "sender.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Receiver *receiver;
    Sender *sender;
};

#endif // MAINWINDOW_H
