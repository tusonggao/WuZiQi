#ifndef PLAYBOARD_H
#define PLAYBOARD_H

#include "../commonHeaderFiles/commonwidgetheader.h"
#include "wuziChessBoardClient.h"
#include "connectserverdlg.h"
#include <QHostInfo>
#include <QTcpSocket>

class WuziPlayBoardClient : public QDialog
{
    Q_OBJECT

public:
    explicit WuziPlayBoardClient(QWidget *parent = 0);
    ~WuziPlayBoardClient();

signals:
    void connectServerSuccess(QString &title);

private slots:
    void cancelLast();
    void restart();
    void connectServer();
    void chessFinished(int winner);
    void chessUnfinished();
    void connectServerSuccess(QString user, QString opponent, bool colorIsBlack);
    void disconnectFromServer();

protected:
//    void paintEvent(QPaintEvent *);
//    void mousePressEvent(QMouseEvent *);
//    void keyPressEvent(QKeyEvent *);

private:
    WuZiChessBoardClient *m_chessBoard;
    ConnectServerDlg *m_connectServerDlg;

    QPushButton *m_connectServer;
    QPushButton *m_cancelLast;
    QPushButton *m_restart;
    QLabel *m_labelOutcome;

    int m_port;
    QHostAddress *m_serverIP;
    QTcpSocket *m_tcpSocket;
};


#endif // PLAYBOARD_H
