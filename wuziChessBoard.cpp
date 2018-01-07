#include "wuziChessBoard.h"


WuZiChessBoard::WuZiChessBoard(QWidget *parent)
{
    m_chessSideLen = 630;
    m_rows = ROWS;
    m_winner = 0;
    m_bFinished = false;
    m_stopRandom = true;
    m_pRandomThread = NULL;
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

    // setStyleSheet("background: rgb(200, 150, 100)");
    // setStyleSheet("background: yellow");
    

    
}

WuZiChessBoard::~WuZiChessBoard()
{
}

void WuZiChessBoard::initChessBoard()
{
    m_bFinished = false;
    m_stopRandom = true;
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

void WuZiChessBoard::drawChessPiece(int x, int y)
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

void WuZiChessBoard::eraseChessPiece(int x, int y)
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

void WuZiChessBoard::mousePressEvent(QMouseEvent *event)
{    
    if(m_bFinished)
    {
        return;
    }

    int X = (event->x()-m_delta+0.5*m_rowWidth)/m_rowWidth;
    int Y = (event->y()-m_delta+0.5*m_rowWidth)/m_rowWidth;

    if(X<=m_rows && Y<=m_rows)
    {
        m_mutex.lock();

        if(m_chessPieces[X][Y]!=0)
        {
            m_mutex.unlock();
            return;
        }
        Step step(X, Y);
        m_vecSteps.push_back(step);
        m_chessPieces[X][Y] = m_turn;
        m_turn *= -1;        
        checkFinished();
        drawChessPiece(X, Y);
        update();

        m_mutex.unlock();
    }


}

void WuZiChessBoard::randomize()
{
    if(m_bFinished)
    {
        return;
    }

    m_stopRandom = false;

    if(m_pRandomThread==NULL)
    {
        m_pRandomThread = new RandomizeThread(this);
        connect(m_pRandomThread, SIGNAL(updateBoard()), this, SLOT(update()));
    }

    if(!m_pRandomThread->isRunning())
        m_pRandomThread->start();
}


void WuZiChessBoard::clearAll()
{
    m_mutex.lock();

    initChessBoard();
    update();

    m_mutex.unlock();

    emit chessUnfinished();
}

void WuZiChessBoard::cancelLast()
{
    m_mutex.lock();

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

    m_mutex.unlock();
}

bool WuZiChessBoard::isFinished()
{
    return m_bFinished;
}

bool WuZiChessBoard::checkFinished()
{
    if(m_vecSteps.size()==0)
    {
        m_bFinished = false;
        return false;
    }

    if(m_bFinished==true)
    {
        return true;
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

void WuZiChessBoard::stopRandomize()
{
    m_mutex.lock();
    m_stopRandom = true;
    m_mutex.unlock();
}


RandomizeThread::RandomizeThread(WuZiChessBoard* board)
{
    m_pBoard = board;
}

void RandomizeThread::run()
{
    Random rand;

    while(true)
    {
        m_pBoard->m_mutex.lock();

        if(m_pBoard->m_stopRandom)
        {
            m_pBoard->m_mutex.unlock();
            break;
        }

        if(m_pBoard->m_vecSteps.size()>=(WuZiChessBoard::ROWS+1)*(WuZiChessBoard::ROWS+1))
        {
            m_pBoard->m_mutex.unlock();
            break;
        }

        int x = rand.getRandNum(WuZiChessBoard::ROWS+1);
        int y = rand.getRandNum(WuZiChessBoard::ROWS+1);

        if(m_pBoard->m_chessPieces[x][y]!=0)
        {
            m_pBoard->m_mutex.unlock();
            continue;
        }

        m_pBoard->m_chessPieces[x][y] = m_pBoard->m_turn;

        Step step(x, y);
        m_pBoard->m_vecSteps.push_back(step);
        m_pBoard->drawChessPiece(x, y);
        m_pBoard->m_turn *= -1;
        emit updateBoard();

        if(m_pBoard->checkFinished())
        {
            m_pBoard->m_mutex.unlock();
            break;
        }

        m_pBoard->m_mutex.unlock();

        SleepForMilliSeconds(300);
    }
}
