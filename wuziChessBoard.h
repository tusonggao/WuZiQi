#ifndef WUZICHESS_H
#define WUZICHESS_H

#include "./commonHeaderFiles/commonwidgetheader.h"
#include "./commonHeaderFiles/commonFunc.h"

class Step
{
public:
    Step(int index_X, int index_Y)
    {
        x = index_X;
        y = index_Y;
    }

public:
    int x;
    int y;
};


class WuZiChessBoard : public QWidget
{
    Q_OBJECT

public:
    explicit WuZiChessBoard(QWidget *parent = 0);
    ~WuZiChessBoard();

    bool checkFinished();
    bool isFinished();
    void initChessBoard();
    void drawChessPiece(int x, int y);
    void eraseChessPiece(int x, int y);
    void randomize();
    void stopRandomize();
    void clearAll();
    void cancelLast();

    friend class RandomizeThread;

    enum
    {
        ROWS = 15
    };

protected:
//    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
//    void keyPressEvent(QKeyEvent *);

signals:
    void chessFinished(int winner);
    void chessUnfinished();

private:
    QPixmap m_pixmap;
    QLabel *m_label;
    RandomizeThread *m_pRandomThread;
    QMutex m_mutex;
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
    bool m_stopRandom;
    int m_turn; // -1 for black; 1 for white;    
};


class RandomizeThread: public QThread
{
    Q_OBJECT

public:
    RandomizeThread(WuZiChessBoard* board);
    void run();

signals:
    void updateBoard();

private:
    WuZiChessBoard *m_pBoard;
};

#endif // WUZICHESS_H
