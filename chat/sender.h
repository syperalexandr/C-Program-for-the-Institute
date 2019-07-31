#ifndef SENDER_H
#define SENDER_H

#include <QWidget>
#include <mipconfig.h>
#include <mipavcodecencoder.h>
#include <mipaudiosession.h>
#include <mippainputoutput.h>
#include <jrtplib3/rtpipv4address.h>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QUdpSocket;
class QComboBox;
class QTextEdit;
class Receiver;
QT_END_NAMESPACE

class Sender : public QWidget
{
public:
    Sender(Receiver *receiver, QLineEdit *lineEditName, QTextEdit *textEditMessages, QTextEdit *textEditMessage, QPushButton *buttonSend, QPushButton *buttonCall, QComboBox *comboBoxReceiver);
private:
    Receiver *receiver = nullptr;
    QLineEdit *lineEditName = nullptr;
    QTextEdit *textEditMessages = nullptr;
    QTextEdit *textEditMessage = nullptr;
    QPushButton *buttonSend = nullptr;
    QPushButton *buttonCall = nullptr;
    QComboBox *comboBoxReceiver = nullptr;

    QUdpSocket *udpSocket = nullptr;
    QTimer *timer = nullptr;
    MIPAudioSession audioSess;

    void sendMessage();
    void sendGreeting();
    void sendAlive();
    void call();

    void clearMessage();
    bool isEmpty();
    void checkRet(bool ret,const MIPErrorBase &obj);

    bool firstTime = true;

    const QString P_CONNECT = "EVMp_CONNECT_";
    const QString P_ALIVE = "EVMp_ALIVE_";
    const QString P_SENDMESSAGE = "EVMp_SENDMSG_";
    const QString P_PRIVATEMESSAGE = "EVMp_PRIVATEMSG_";
    const QString P_CALL = "EVMp_CALL";
    const QString P_DIVIDER = "_";
};

#endif // SENDER_H
