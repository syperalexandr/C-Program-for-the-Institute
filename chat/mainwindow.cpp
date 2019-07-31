#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    receiver = new Receiver(ui->lineEditName, ui->textEditMessages, ui->listWidgetUsers, ui->comboBoxReceiver);
    sender = new Sender(receiver, ui->lineEditName, ui->textEditMessages, ui->textEditMessage, ui->pushButtonSend, ui->pushButtonCall, ui->comboBoxReceiver);
//    ui->listWidgetUsers->setIconSize(QSize(30, 30));
//    foreach (const QString& item, LIST_ITEMS) {
//        QListWidgetItem* listItem = new QListWidgetItem(item);
//        listItem->setIcon(QPixmap("icon_online.jpeg"));
//        // Включаем возможность редактирования
//        listItem->setFlags( Qt::ItemIsEditable | Qt::ItemIsEnabled );
//        ui->listWidgetUsers->addItem(listItem);
//    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
