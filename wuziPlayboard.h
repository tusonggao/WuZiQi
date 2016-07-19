#ifndef PLAYBOARD_H
#define PLAYBOARD_H

#include "./commonHeaderFiles/commonwidgetheader.h"
#include "wuziChessBoard.h"

class WuziPlayBoard : public QDialog
{
    Q_OBJECT

public:
    explicit WuziPlayBoard(QWidget *parent = 0);
    ~WuziPlayBoard();

private slots:
    void randomize();
    void clearAll();
    void cancelLast();
    void chessFinished(int winner);
    void chessUnfinished();

protected:
//    void paintEvent(QPaintEvent *);
//    void mousePressEvent(QMouseEvent *);
//    void keyPressEvent(QKeyEvent *);

private:
    WuZiChessBoard *m_chessBoard;
    QPushButton *m_randomize;
    QPushButton *m_cancelLast;
    QPushButton *m_clearAll;
    QPushButton *m_restart;

    QLabel *m_labelOutcome;
};


#endif // PLAYBOARD_H
