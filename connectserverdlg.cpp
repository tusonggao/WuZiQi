#include "connectserverdlg.h"

///////////////////////////////////////////////////////


ConnectServerDlg::ConnectServerDlg()
{
    m_userName = "user1";
    m_opponentName = "user2";
    m_portNUm = 7999;
    m_colorIsBlack = true; //执黑子 先手

    m_userNamelabel = new QLabel(ChineseToUTF8_TSG("用户名："));
    m_userNameEdit = new QLineEdit();
    m_opponentNamelabel = new QLabel(ChineseToUTF8_TSG("对手方用户名："));
    m_opponentNameEdit = new QLineEdit();
    m_portNumlabel = new QLabel(ChineseToUTF8_TSG("服务端口号："));
    m_portNumEdit = new QLineEdit();
    m_colorlabel = new QLabel(ChineseToUTF8_TSG("棋子颜色："));
    m_colorCombo = new QComboBox();
    m_colorCombo->addItem(ChineseToUTF8_TSG("黑子（先手）"));
    m_colorCombo->addItem(ChineseToUTF8_TSG("白子（后手）"));

    m_ok = new QPushButton(ChineseToUTF8_TSG("连接服务器"));
    m_cancel = new QPushButton(ChineseToUTF8_TSG("取消"));

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(m_userNamelabel, 0, 0, 1, 1);
    mainLayout->addWidget(m_userNameEdit, 0, 1, 1, 2);
    mainLayout->addWidget(m_opponentNamelabel, 1, 0, 1, 1);
    mainLayout->addWidget(m_opponentNameEdit, 1, 1, 1, 2);
    mainLayout->addWidget(m_portNumlabel, 2, 0, 1, 1);
    mainLayout->addWidget(m_portNumEdit, 2, 1, 1, 2);
    mainLayout->addWidget(m_colorlabel, 3, 0, 1, 1);
    mainLayout->addWidget(m_colorCombo, 3, 1, 1, 2);
    mainLayout->addWidget(m_ok, 5, 0, 1, 1);
    mainLayout->addWidget(m_cancel, 5, 2, 1, 1);

    setFixedSize(sizeHint()*1.2);

    connect(m_ok, SIGNAL(clicked(bool)), this, SLOT(okClicked()));
    connect(m_cancel, SIGNAL(clicked(bool)), this, SLOT(close()));
}

void ConnectServerDlg::okClicked()
{
    if(m_userNameEdit->text().isEmpty() || m_opponentNameEdit->text().isEmpty() ||
      m_portNumEdit->text().isEmpty())
    {
        return;
    }
    else
    {
        m_userName = m_userNameEdit->text();
        m_opponentName = m_opponentNameEdit->text();
        m_portNUm = m_portNumEdit->text().toInt();
        if(m_colorCombo->currentIndex()==0)
        {
            m_colorIsBlack = true;
        }
        else
        {
            m_colorIsBlack = false;
        }

        emit connectServerSignal(m_userName, m_opponentName, m_portNUm, m_colorIsBlack);
        close();
    }
}

void ConnectServerDlg::showEvent(QShowEvent *event)
{
    m_userNameEdit->setText(m_userName);
    m_opponentNameEdit->setText(m_opponentName);
    m_portNumEdit->setText(QString("%1").arg(m_portNUm));
    m_colorCombo->setCurrentIndex(m_colorIsBlack?0:1);
}
