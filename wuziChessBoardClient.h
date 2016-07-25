#ifndef WUZICHESS_H
#define WUZICHESS_H

#include "../commonHeaderFiles/commonwidgetheader.h"
#include "../commonHeaderFiles/commonFunc.h"
#include "wuzichessboarddesktop.h"
#include <QHostInfo>
#include <QTcpSocket>

class WuZiChessBoardClient : public QWidget
{
    Q_OBJECT

public:
    explicit WuZiChessBoardClient(QWidget *parent = 0);
    ~WuZiChessBoardClient();

    bool checkFinished();
    bool isFinished();
    void initChessBoard();
    void drawChessPiece(int x, int y);
    void eraseChessPiece(int x, int y);
    void clearAll();
    void cancelLast();
    void restart();

    enum
    {
        ROWS = 15
    };

public slots:
    void slotConnected();
    void slotDisconnected();
    void connectServer(QString userName, QString opponentName, int port, bool colorIsBlack);
    void dataReceived();
    void setUserName(QString str);
    void checkNetworkStatus();

protected:
    void mousePressEvent(QMouseEvent *);
    void sendStepToServer(int x, int y);
    void sendRestartSignalToServer();

signals:
    void chessFinished(int winner);
    void chessUnfinished();
    void connectServerSuccess(QString user, QString opponent, bool colorIsBlack);
    void disconnectFromServer();

private:
    bool isValidStepInfo(QString &msg) const;
    Step translateStepInfo(QString &msg) const;

private:
    QPixmap m_pixmap;
    QLabel *m_label;
    std::vector<Step> m_vecSteps;

    int m_chessSideLen;
    int m_rows;
    int m_rowWidth;
    int m_radius;
    int m_delta;
    double m_penWidth;

    int m_chessPieces[ROWS+1][ROWS+1];
    int m_winner; // -1 for black; 1 for white; 0 for unfinished.
    bool m_bFinished;
    int m_turn; // -1 for black; 1 for white;    

    int m_port;
    QHostAddress *m_serverIP;
    QString m_userName;
    QString m_opponentName;
    QTcpSocket *m_tcpSocket;


    QTimer *m_networkStatusCheckTimer;

    QMutex m_mutex;
    bool m_bServerConnected;
    bool m_bOpponentOnline;
    bool m_bServerConnectedLatest;
    bool m_bOpponentOnlineLatest;

    bool m_bColorIsBlack;
};


#endif // WUZICHESS_H
