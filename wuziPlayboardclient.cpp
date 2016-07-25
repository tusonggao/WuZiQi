#include "wuziPlayboardclient.h"

WuziPlayBoardClient::WuziPlayBoardClient(QWidget *parent)
{
    setStyleSheet("background: rgb(200, 150, 100)");

    m_chessBoard = new WuZiChessBoardClient;
    m_connectServerDlg = NULL;

    m_labelOutcome = new QLabel(ChineseToUTF8_TSG("比赛结果：未分胜负"));
    m_connectServer = new QPushButton(ChineseToUTF8_TSG("连接服务器"));
    m_cancelLast = new QPushButton(ChineseToUTF8_TSG("撤销上一步"));
    m_cancelLast->setEnabled(false);
    m_restart = new QPushButton(ChineseToUTF8_TSG("重新开始"));
    m_restart->setEnabled(false);

    QVBoxLayout *vRightLayout = new QVBoxLayout;
    vRightLayout->addWidget(m_labelOutcome);
    vRightLayout->addWidget(m_connectServer);
    vRightLayout->addWidget(m_cancelLast);
    vRightLayout->addWidget(m_restart);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(m_chessBoard);
    hLayout->addLayout(vRightLayout);

    setLayout(hLayout);
    setWindowTitle(ChineseToUTF8_TSG("五子棋游戏"));
    setFixedSize(sizeHint());

    connect(m_connectServer, SIGNAL(clicked(bool)), this, SLOT(connectServer()));
    connect(m_cancelLast, SIGNAL(clicked(bool)), this, SLOT(cancelLast()));
    connect(m_restart, SIGNAL(clicked(bool)), this, SLOT(restart()));

    connect(m_chessBoard, SIGNAL(chessFinished(int)), this, SLOT(chessFinished(int)));
    connect(m_chessBoard, SIGNAL(chessUnfinished()), this, SLOT(chessUnfinished()));
    connect(m_chessBoard, SIGNAL(connectServerSuccess(QString, QString, bool)),
            this, SLOT(connectServerSuccess(QString, QString, bool)));
    connect(m_chessBoard, SIGNAL(disconnectFromServer()), this, SLOT(disconnectFromServer()));
}

WuziPlayBoardClient::~WuziPlayBoardClient()
{
}

void WuziPlayBoardClient::connectServer()
{
    if(m_connectServerDlg==NULL)
    {
        m_connectServerDlg = new ConnectServerDlg;
        connect(m_connectServerDlg, SIGNAL(connectServerSignal(QString, QString, int, bool)),
                m_chessBoard, SLOT(connectServer(QString, QString, int, bool)));
    }

    m_connectServerDlg->exec();
}

void WuziPlayBoardClient::connectServerSuccess(QString user, QString opponent, bool colorIsBlack)
{
    QString title = ChineseToUTF8_TSG("五子棋游戏 ") + ChineseToUTF8_TSG("用户名：")
                    + user + ChineseToUTF8_TSG(" 对手名：") + opponent
                    + (colorIsBlack ? ChineseToUTF8_TSG(" 执黑子") : ChineseToUTF8_TSG(" 执白子"));
    emit connectServerSuccess(title);

    m_connectServer->setText(ChineseToUTF8_TSG("连接服务器成功"));
}

void WuziPlayBoardClient::disconnectFromServer()
{
    m_connectServer->setText(ChineseToUTF8_TSG("已从服务器断开"));
}

void WuziPlayBoardClient::cancelLast()
{
    m_chessBoard->cancelLast();

    if(m_chessBoard->isFinished()==false)
    {
        m_labelOutcome->setText(ChineseToUTF8_TSG("比赛结果：未分胜负"));
    }
}

void WuziPlayBoardClient::restart()
{
    m_chessBoard->restart();
}

void WuziPlayBoardClient::chessFinished(int winner)
{
    QString outcome;

    if(winner==-1)
    {
        outcome = ChineseToUTF8_TSG("比赛结果：黑方胜");
    }
    else
    {
        outcome = ChineseToUTF8_TSG("比赛结果：白方胜");
    }

    m_labelOutcome->setText(outcome);
    m_restart->setEnabled(true);
}

void WuziPlayBoardClient::chessUnfinished()
{
    m_labelOutcome->setText(ChineseToUTF8_TSG("比赛结果：未分胜负"));
    m_restart->setEnabled(false);
}
