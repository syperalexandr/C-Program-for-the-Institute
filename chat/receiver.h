#ifndef RECEIVER_H
#define RECEIVER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTextEdit;
class QListWidget;
class QUdpSocket;
class QComboBox;
class QLineEdit;
class QHostAddress;
QT_END_NAMESPACE


class Receiver : public QWidget
{
public:
    Receiver(QLineEdit *lineEditName, QTextEdit *textEditMessages = nullptr, QListWidget *listWidgetUsers = nullptr, QComboBox *comboBoxReceiver = nullptr);
    QHash<QHostAddress, QString> userList;
    QHash<QHostAddress, int> timerList;
private slots:
    void processPendingDatagrams();
private:
    QTextEdit *textEditMessages = nullptr;
    QListWidget *listWidgetUsers = nullptr;
    QLineEdit *lineEditName = nullptr;
    QComboBox *comboBoxReceiver = nullptr;
    QUdpSocket *udpSocket = nullptr;

    const QString P_TYPE = "EVMp";
    const QString P_CONNECT = "CONNECT";
    const QString P_ALIVE = "ALIVE";
    const QString P_SENDMESSAGE = "SENDMSG";
    const QString P_PRIVATEMSG = "PRIVATEMSG";

    QTimer *timer;

    void refreshListWidget();
    void refreshComboBox();
    void checkAlive();
    void addMessage(QString text, bool isChatMessage);
};

#endif // RECEIVER_H
