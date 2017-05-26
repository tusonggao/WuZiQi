#include "WuZiChessBoardClient.h"

///////////////////////////////////////////////////////////

WuZiChessBoardClient::WuZiChessBoardClient(QWidget *parent)
{
    m_tcpSocket = NULL;
    m_bServerConnected = false;
    m_bOpponentOnline = false;

    m_userName = "AAA";
    m_chessSideLen = 630;
    m_rows = ROWS;
    m_winner = 0;
    m_bFinished = false;
    m_turn = -1;   //ºÚ×ÓÏÈ×ß

    m_penWidth = m_chessSideLen/(20*m_rows);
    m_rowWidth = m_chessSideLen/(m_rows+1);
    m_delta = 0.5*(m_chessSideLen-m_rowWidth*m_rows);
    m_radius = m_chessSideLen/(1.5*m_rows);

    for(int i=0; i<(ROWS+1); i++)
        for(int j=0; j<(ROWS+1); j++)
        {
            m_chessPieces[i][j] = 0;
        }

    m_label = new QLabel;
    m_pixmap = QPixmap(m_chessSideLen, m_chessSideLen);
    m_label->setPixmap(m_pixmap);

    initChessBoard();

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(m_label);
    setLayout(hLayout);

    setFixedSize(m_chessSideLen*1.03, m_chessSideLen*1.03);

    m_networkStatusCheckTimer = new QTimer;
    m_networkStatusCheckTimer->setInterval(1500);
    connect(m_networkStatusCheckTimer, SIGNAL(timeout()), this, SLOT(checkNetworkStatus()));
    m_networkStatusCheckTimer->start();

    // setStyleSheet("background: rgb(200, 150, 100)");
    // setStyleSheet("background: yellow");
    // setStyleSheet("background: beige");

    // setWindowFlags(Qt::FramelessWindowHint);
}

WuZiChessBoardClient::~WuZiChessBoardClient()
{
}

void WuZiChessBoardClient::initChessBoard()
{
    m_bFinished = false;
    m_turn = -1;

    for(int i=0; i<(ROWS+1); i++)
        for(int j=0; j<(ROWS+1); j++)
        {
            m_chessPieces[i][j] = 0;
        }
    m_vecSteps.clear();

    m_pixmap.fill(qRgb(200, 150, 100));

    QPainter painter(&m_pixmap);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, m_penWidth, Qt::SolidLine, Qt::RoundCap));

    for(int i=0; i<=m_rows; i++)
    {
        painter.drawLine(i*m_rowWidth+m_delta, m_delta,
                         i*m_rowWidth+m_delta, m_pixmap.width()-m_delta-1);
        painter.drawLine(m_delta, i*m_rowWidth+m_delta,
                         m_pixmap.height()-m_delta-1, i*m_rowWidth+m_delta);
    }

    m_label->setPixmap(m_pixmap);

    repaint();
}

void WuZiChessBoardClient::drawChessPiece(int x, int y)
{
    QPainter painter(&m_pixmap);

    painter.setRenderHint(QPainter::Antialiasing, true);

    if(m_chessPieces[x][y]==-1)   //ºÚ×Ó
    {
        painter.setPen(QPen(Qt::black, 0.2, Qt::DashDotLine, Qt::RoundCap));
        painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
        painter.drawEllipse(x*m_rowWidth+m_delta-0.5*m_radius,
                            y*m_rowWidth+m_delta-0.5*m_radius, m_radius, m_radius);
    }
    else if(m_chessPieces[x][y]==1)   //°××Ó
    {
        painter.setPen(QPen(Qt::white, 0.2, Qt::DashDotLine, Qt::RoundCap));
        painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
        painter.drawEllipse(x*m_rowWidth+m_delta-0.5*m_radius,
                            y*m_rowWidth+m_delta-0.5*m_radius, m_radius, m_radius);
    }

    m_label->setPixmap(m_pixmap);
}

void WuZiChessBoardClient::eraseChessPiece(int x, int y)
{
    QPainter painter(&m_pixmap);

    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setBrush(QBrush(qRgb(200, 150, 100), Qt::SolidPattern));
    painter.fillRect(x*m_rowWidth+m_delta-0.5*m_radius, y*m_rowWidth+m_delta-0.5*m_radius,
                     m_radius+0.1, m_radius+0.1, QBrush(qRgb(200, 150, 100)));

    painter.setPen(QPen(Qt::black, m_penWidth, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(x*m_rowWidth+m_delta,
                     qBound((double)m_delta, (double)(y*m_rowWidth+m_delta-0.5*m_radius), (double)(m_pixmap.height()-m_delta-1)),
                     x*m_rowWidth+m_delta,
                     qBound((double)m_delta, (double)(y*m_rowWidth+m_delta+0.5*m_radius), (double)(m_pixmap.height()-m_delta-1)));
    painter.drawLine(qBound((double)m_delta, (double)(x*m_rowWidth+m_delta-0.5*m_radius), (double)(m_pixmap.width()-m_delta-1)),
                     y*m_rowWidth+m_delta,
                     qBound((double)m_delta, (double)(x*m_rowWidth+m_delta+0.5*m_radius), (double)(m_pixmap.width()-m_delta-1)),
                     y*m_rowWidth+m_delta);

    m_label->setPixmap(m_pixmap);
}

void WuZiChessBoardClient::mousePressEvent(QMouseEvent *event)
{
    // m_mutex.lock();
    bool bServerConnected = m_bServerConnected;
    bool bOpponentOnline = m_bOpponentOnline;
    // m_mutex.unlock();

    if(!bServerConnected)
    {
        QMessageBox::information(this, "Information",tr("Disconnected to server"));
        return;
    }
    if(!bOpponentOnline)
    {
        QMessageBox::information(this, "Information",tr("Your opponent is not online"));
        return;
    }
    if(m_bFinished)
    {
        QMessageBox::information(this, "Information",tr("Game is over"));
        return;
    }

    if((m_bColorIsBlack && (m_turn==1)) || (!m_bColorIsBlack && (m_turn==-1)))
    {
        QMessageBox::information(this, "Information",tr("Sorry, it's not your turn."));
        return;
    }

    int X = (event->x()-m_delta+0.5*m_rowWidth)/m_rowWidth;
    int Y = (event->y()-m_delta+0.5*m_rowWidth)/m_rowWidth;

    if(X<=m_rows && Y<=m_rows)
    {
        if(m_chessPieces[X][Y]!=0)
        {
            return;
        }
        Step step(X, Y);
        m_vecSteps.push_back(step);
        m_chessPieces[X][Y] = m_turn;
        m_turn *= -1;        
        checkFinished();
        drawChessPiece(X, Y);
        update();
        sendStepToServer(X, Y);
    }
}

void WuZiChessBoardClient::restart()
{
    clearAll();
    sendRestartSignalToServer();
}

void WuZiChessBoardClient::clearAll()
{
    initChessBoard();
    update();

    emit chessUnfinished();
}

void WuZiChessBoardClient::cancelLast()
{
    if(m_vecSteps.size()!=0)
    {
        Step lastStep = m_vecSteps.back();
        m_chessPieces[lastStep.x][lastStep.y] = 0;
        eraseChessPiece(lastStep.x, lastStep.y);
        m_vecSteps.pop_back();
        m_turn *= -1;
        m_bFinished = false;
        checkFinished();
        update();
    }
}

bool WuZiChessBoardClient::isFinished()
{
    return m_bFinished;
}

bool WuZiChessBoardClient::checkFinished()
{
    if(m_vecSteps.size()==0)
    {
        m_bFinished = false;
        return false;
    }

    Step lastStep = m_vecSteps.back();
    int lastX = lastStep.x;
    int lastY = lastStep.y;

    for(int i=0; i<5; i++)  // check row
    {
        int sum = 0;
        for(int j=0; j<5; j++)
        {
            if( ((lastX-i+j)<0) || ((lastX-i+j)>ROWS) )
                continue;
            sum += m_chessPieces[lastX-i+j][lastY];
        }

        if(sum!=5 && sum!=-5)
        {
            continue;
        }

        m_winner = sum/5;
        m_bFinished = true;
        emit chessFinished(m_winner);
        return true;
    }

    for(int i=0; i<5; i++)  // check column
    {
        int sum = 0;
        for(int j=0; j<5; j++)
        {
            if( ((lastY+i-j)<0) || ((lastY+i-j)>ROWS) )
                continue;
            sum += m_chessPieces[lastX][lastY+i-j];
        }
        if(sum!=5 && sum!=-5)
        {
            continue;
        }

        m_winner = sum/5;
        m_bFinished = true;
        emit chessFinished(m_winner);
        return true;
    }

    for(int i=0; i<5; i++)  // check south-east and north-west direction
    {
        int sum = 0;
        for(int j=0; j<5; j++)
        {
            if( ((lastX+i-j)<0) || ((lastX+i-j)>ROWS) || ((lastY+i-j)<0) || ((lastY+i-j)>ROWS))
                continue;
            sum += m_chessPieces[lastX+i-j][lastY+i-j];
        }
        if(sum!=5 && sum!=-5)
        {
            continue;
        }

        m_winner = sum/5;
        m_bFinished = true;
        emit chessFinished(m_winner);
        return true;
    }

    for(int i=0; i<5; i++)  // check north-east and south-west direction
    {
        int sum = 0;
        for(int j=0; j<5; j++)
        {
            if( ((lastX-i+j)<0) || ((lastX-i+j)>ROWS) || ((lastY+i-j)<0) || ((lastY+i-j)>ROWS))
                continue;
            sum += m_chessPieces[lastX-i+j][lastY+i-j];
        }
        if(sum!=5 && sum!=-5)
        {
            continue;
        }

        m_winner = sum/5;
        m_bFinished = true;
        emit chessFinished(m_winner);
        return true;
    }

    m_bFinished = false;
    emit chessUnfinished();
    return false;
}

void WuZiChessBoardClient::setUserName(QString str)
{
    m_userName = str;
}

void WuZiChessBoardClient::connectServer(QString userName, QString opponentName,
                                         int port, bool colorIsBlack)
{
    qDebug() << "into WuZiChessBoardClient::connectServer()" ;
    m_serverIP = new QHostAddress();
    m_serverIP->setAddress("127.0.0.1");
    m_port = port;
    m_userName = userName;
    m_opponentName = opponentName;
    m_bColorIsBlack = colorIsBlack;
    m_bServerConnected = true;

    m_tcpSocket = new QTcpSocket(this);

    connect(m_tcpSocket, SIGNAL(connected()),this, SLOT(slotConnected()));
    connect(m_tcpSocket, SIGNAL(disconnected()),this, SLOT(slotDisconnected()));
    connect(m_tcpSocket, SIGNAL(readyRead()),this, SLOT(dataReceived()));

    m_tcpSocket->connectToHost(*m_serverIP, m_port);
}


void WuZiChessBoardClient::slotConnected()
{
    qDebug() << "Get into WuZiChessBoardClient::slotConnected()";
    QString msg = QString("%1:entered game room opponent:%2::").arg(m_userName).arg(m_opponentName);
    QByteArray bytes = msg.toLatin1();
    m_tcpSocket->write(bytes, bytes.length());
    emit connectServerSuccess(m_userName, m_opponentName, m_bColorIsBlack);
}


void WuZiChessBoardClient::sendRestartSignalToServer()
{
    qDebug() << "Get into WuZiChessBoardClient::sendStepToServer()";

    QString msg = QString("%1::%2!!restart!").arg(m_userName).arg(m_opponentName);

    QByteArray bytes = msg.toLatin1();
    if(m_tcpSocket!=NULL)
    {
        m_tcpSocket->write(bytes, bytes.length());
    }
}

void WuZiChessBoardClient::sendStepToServer(int x, int y)
{
    qDebug() << "Get into WuZiChessBoardClient::sendStepToServer()";

    QString msg = QString("%1:step(%2,%3)").arg(m_userName).arg(x).arg(y);

    QByteArray bytes = msg.toLatin1();
    if(m_tcpSocket!=NULL)
    {
        m_tcpSocket->write(bytes, bytes.length());
    }
}

void WuZiChessBoardClient::slotDisconnected()
{
    emit disconnectFromServer();
}

void WuZiChessBoardClient::dataReceived()
{
    QByteArray datagram;
    datagram.resize(m_tcpSocket->bytesAvailable());

    m_tcpSocket->read(datagram.data(),datagram.size());

    QString msg=datagram.data();
    if(isValidStepInfo(msg))
    {
        Step step = translateStepInfo(msg);
        m_chessPieces[step.x][step.y] = m_turn;
        drawChessPiece(step.x, step.y);
        m_vecSteps.push_back(step);
        checkFinished();
        m_turn *= -1;
    }
    else if(msg.contains("restart"))
    {
        clearAll();
    }
    else if(msg.contains("AreYouOnline"))
    {
        QString msg = QString("%1::IAmOnline").arg(m_userName);
        QByteArray bytes = msg.toLatin1();
        if(m_tcpSocket!=NULL)
        {
            m_tcpSocket->write(bytes, bytes.length());
        }
    }
    else if(msg.contains("OpponentIsOnline"))
    {
        m_bOpponentOnlineLatest = true;
        m_bServerConnectedLatest = true;
    }
    else if(msg.contains("ServerIsOnline"))
    {
        m_bServerConnectedLatest = true;
    }
}

bool WuZiChessBoardClient::isValidStepInfo(QString &msg) const
{
    QString playerName = msg.left(msg.indexOf(":"));
    return m_opponentName==playerName && msg.contains("step");
}

Step WuZiChessBoardClient::translateStepInfo(QString &msg) const
{
    int x, y;
    int indexStartX = msg.indexOf("step(") + QString("step(").length();
    int widthX = msg.indexOf(",") - indexStartX;
    QString xStr = msg.mid(indexStartX, widthX);
    x= xStr.toInt();

    int indexStartY = msg.indexOf(",") + QString(",").length();
    int widthY = msg.indexOf(")") - indexStartY;
    QString yStr = msg.mid(indexStartY, widthY);
    y= yStr.toInt();

    return Step(x, y);
}


void WuZiChessBoardClient::checkNetworkStatus()
{
    // m_mutex.lock();
    m_bServerConnected = m_bServerConnectedLatest;
    m_bOpponentOnline = m_bOpponentOnlineLatest;
    // m_mutex.unlock();

    m_bServerConnectedLatest = false;
    m_bOpponentOnlineLatest = false;

    QString msg = QString("%1::CheckMyOpponentOnline").arg(m_opponentName);
    QByteArray bytes = msg.toLatin1();
    if(m_tcpSocket!=NULL)
    {
        m_tcpSocket->write(bytes, bytes.length());
    }
}
