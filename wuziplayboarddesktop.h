#ifndef WUZIPLAYBOARDDESKTOP_H
#define WUZIPLAYBOARDDESKTOP_H

#include "../commonHeaderFiles/commonwidgetheader.h"
#include "wuzichessboarddesktop.h"

class WuziPlayBoardDesktop : public QDialog
{
    Q_OBJECT

public:
    explicit WuziPlayBoardDesktop(QWidget *parent = 0);
    ~WuziPlayBoardDesktop();

private slots:
    void randomize();
    void clearAll();
    void cancelLast();
    void chessFinished(int winner);
    void chessUnfinished();

protected:


private:
    WuZiChessBoardDeskTop *m_chessBoard;

    QPushButton *m_randomize;
    QPushButton *m_cancelLast;
    QPushButton *m_clearAll;
    QPushButton *m_restart;
    QLabel *m_labelOutcome;
};


#endif // WUZIPLAYBOARDDESKTOP_H
