#ifndef CONNECTSERVERDLG_H
#define CONNECTSERVERDLG_H

#include "../commonHeaderFiles/commonwidgetheader.h"

// I am doing this to remind myself to program everyday.
// I am doing this to remind myself to program everyday.

// I am doing this to remind myself to program everyday.

class ConnectServerDlg: public QDialog
{
    Q_OBJECT

public:
    ConnectServerDlg();
    QString getUserName();

signals:
    void connectServerSignal(QString, QString, int, bool colorIsBlack);

private slots:
    void okClicked();
    void showEvent(QShowEvent *event);

private:
    QLabel *m_userNamelabel;
    QLineEdit *m_userNameEdit;
    QLabel *m_opponentNamelabel;
    QLineEdit *m_opponentNameEdit;
    QLabel *m_portNumlabel;
    QLineEdit *m_portNumEdit;
    QLabel *m_colorlabel;
    QComboBox *m_colorCombo;

    QPushButton *m_ok;
    QPushButton *m_cancel;

    QString m_userName;
    QString m_opponentName;
    bool m_colorIsBlack;
    int m_portNUm;
};


#endif // CONNECTSERVERDLG_H
