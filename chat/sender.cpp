#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>

#include "sender.h"
#include "receiver.h"

#include <iostream>

using namespace jrtplib;

Sender::Sender(Receiver *receiver, QLineEdit *lineEditName, QTextEdit *textEditMessages, QTextEdit *textEditMessage, QPushButton *buttonSend, QPushButton *buttonCall, QComboBox *comboBoxReceiver)
{
    timer = new QTimer(this);
    timer->setInterval(2000);
// Создаём таймер на каждые 2 секунды
    this->receiver = receiver;
    this->lineEditName = lineEditName;
    this->textEditMessages = textEditMessages;
    this->textEditMessage = textEditMessage;
    this->buttonSend = buttonSend;
    this->buttonCall = buttonCall;
    this->comboBoxReceiver = comboBoxReceiver;

    udpSocket = new QUdpSocket(this);
// Создаём Сокет
    connect(buttonSend, &QPushButton::clicked, this, &Sender::sendMessage);
    // Описываем событие при нажатии на кнопку отправить сообщение
    connect(buttonCall, &QPushButton::clicked, this, &Sender::call);
    // Описываем событие при нажатии на кнопку позвонить
    connect(timer, &QTimer::timeout, this, &Sender::sendAlive);
    // Описываем событие для проверки наличия в сети собеседника
}

bool Sender::isEmpty()
{
    if ((lineEditName->text().isEmpty() && lineEditName->isEnabled() == true) || (textEditMessage->toPlainText().isEmpty() && textEditMessage->isEnabled() == true)) {
        return true;
    }
   // Проверяем, первый раз ли мы зашли в программу
    return false;
}

void Sender::clearMessage()
{
    textEditMessage->clear();
}

void Sender::sendMessage()
{
    if (!isEmpty())
    {
        if (firstTime == true) {
            sendGreeting();
            firstTime = false;
            lineEditName->setEnabled(firstTime);
            textEditMessage->setEnabled(!firstTime);
            comboBoxReceiver->setEnabled(!firstTime);
            buttonSend->setText("Отправить");
            timer->start();
            //Делаем окно для написания сообщения, окно выбора собеседника активным,
            //окно написание имени наооборот не активным, меняем текст кнопки на Отправить
        }
        else {
            QString message = textEditMessage->toPlainText();
            //Значению message присваеваем значение текста сообщения
            int len = message.toUtf8().size();
            //Значению len присваеваем длинну сообщения
            if (comboBoxReceiver->currentIndex() == 0)
            //Проверяем, какой элемент списка выбран, 0 - отправка всем
            {
                QByteArray datagram = (P_SENDMESSAGE + QString::number(len) + P_DIVIDER + message).toUtf8();
                udpSocket->writeDatagram(datagram, QHostAddress("172.27.24.255"), 14000); //172.27.24.255 25.29.255.255
                clearMessage();
                //Отправляем сообщения всем, после чистим сообщение
            }
            else
            {
                QByteArray datagram = (P_PRIVATEMESSAGE + QString::number(len) + P_DIVIDER + message).toUtf8();
                udpSocket->writeDatagram(datagram, receiver->userList.key(comboBoxReceiver->currentText()), 14000);
                textEditMessages->setTextColor(QColor(Qt::black));
                textEditMessages->append(tr("Приватное сообщение для %1: %2").arg(comboBoxReceiver->currentText(), message));
                clearMessage();
                //Отправляем сообщение конкретному человеку, после чистим сообщение
            }
        }
    }
}

void Sender::sendGreeting()
{
    QString name = lineEditName->text();
    int len = name.toUtf8().size();
    QByteArray datagram = (P_CONNECT + QString::number(len) + P_DIVIDER + name).toUtf8();
    udpSocket->writeDatagram(datagram, QHostAddress("172.27.24.255"), 14000);
    //Описываем подключение к сессии
}

void Sender::sendAlive()
{
    QString name = lineEditName->text();
    int len = name.toUtf8().size();
    QByteArray datagram = (P_ALIVE + QString::number(len) + P_DIVIDER + name).toUtf8();
    udpSocket->writeDatagram(datagram, QHostAddress("172.27.24.255"), 14000);
    // Проверка каждые 2 секунды нахождения в сети собеседника
}

void Sender::checkRet(bool ret,const MIPErrorBase &obj)
{
    if (!ret)
    {
        std::cerr << obj.getErrorString() << std::endl;
        exit(-1);
    }
}

void Sender::call()
{
    if (buttonCall->text() == "Позвонить")
    {
        //#ifdef WIN32
        //    WSADATA dat;
        //      WSAStartup(MAKEWORD(2,2),&dat);
        //#endif // WIN32
        //#ifdef MIPCONFIG_SUPPORT_PORTAUDIO
        //    std::string errStr;
        //
        //    if (!MIPPAInputOutput::initializePortAudio(errStr))
        //    {
        //        std::cerr << "Can't initialize PortAudio: " << errStr << std::endl;
        //        return -1;
        //    }
        //#endif // MIPCONFIG_SUPPORT_PORTAUDIO

        buttonCall->setText("Завершить");
        MIPAudioSessionParams Aparams;
        // задаём параметр
        bool ret;

        int audioPort = 14002;

        Aparams.setPortbase(audioPort);
        //Задаём порт для аудиосессии
    //	Aparams.setCompressionType(MIPAudioSessionParams::Speex);
        //Aparams.setAcceptOwnPackets(true);
    //	Aparams.setInputDevice("/dev/dsp1");
    //	Aparams.setOutputDevice("/dev/dsp");
    //	Aparams.setSpeexEncoding(MIPAudioSessionParams::UltraWideBand);
    //	Aparams.setSpeexOutgoingPayloadType(97);
        Aparams.setSpeexIncomingPayloadType(97);
        Aparams.setOpusIncomingPayloadType(98);

        qDebug() << "Starting audio session at portbase 14002";

        ret = audioSess.init(&Aparams);
        checkRet(ret, audioSess);

        foreach (QHostAddress add, receiver->userList.keys())
        {
            uint32_t ipLocal = add.toIPv4Address();
            //uint8_t ipLocal[4] = { 172, 27, 24, 103 };
            ret = audioSess.addDestination(RTPIPv4Address(ipLocal, audioPort));
        }

        //uint8_t ipLocal[4] = { 172, 27, 24, 106 };
        //ret = audioSess.addDestination(RTPIPv4Address(ipLocal, audioPort));

    }
    else
    {
        buttonCall->setText("Позвонить");
        audioSess.destroy();
        //#ifdef MIPCONFIG_SUPPORT_PORTAUDIO
        //    MIPPAInputOutput::terminatePortAudio();
        //#endif // MIPCONFIG_SUPPORT_PORTAUDIO
        //#ifdef WIN32
        //    WSACleanup();
        //#endif // WIN32
    }
}
