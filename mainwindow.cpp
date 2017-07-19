#include "mainwindow.h"




MainWindow::MainWindow(QWidget *parent)
{
    m_playBoardClient = NULL;
    m_playBoardDesktop = NULL;

    m_state = non_selected;

    createActions();
    createMenus();

    m_playBoardDesktop = new WuziPlayBoardDesktop;
    setCentralWidget(m_playBoardDesktop);
    setFixedSize(sizeHint());

    QLabel *label = new QLabel;
    setCentralWidget(label);

    setWindowTitle(ChineseToUTF8_TSG("五子棋游戏"));
}

MainWindow::~MainWindow()
{
}


void MainWindow::createMenus()
{
    selectMenu = menuBar()->addMenu(ChineseToUTF8_TSG("选择"));
    selectMenu->addAction(desktopLoadAction);
    selectMenu->addAction(networkLoadAction);

    helpMenu = menuBar()->addMenu(ChineseToUTF8_TSG("帮助"));
    helpMenu->addAction(aboutAction);
}

void MainWindow::createActions()
{
    desktopLoadAction = new QAction(ChineseToUTF8_TSG("单机版"), this);
    connect(desktopLoadAction, SIGNAL(triggered()), this, SLOT(desktopLoad()));

    networkLoadAction = new QAction(ChineseToUTF8_TSG("网络版"), this);
    connect(networkLoadAction, SIGNAL(triggered()), this, SLOT(networkLoad()));

    aboutAction = new QAction(ChineseToUTF8_TSG("关于"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::about()
{
    QString qstring1, qstring2;

    qstring1 = ChineseToUTF8_TSG("关于五子棋游戏");
    qstring2 = ChineseToUTF8_TSG(
               "<h2>五子棋小游戏 1.1</h2>"
               "<p>Copyright &copy; 2016 Software Inc."
               "<p>五子棋小游戏 是一个基于QT的小型益智游戏"
               "<p>作者：涂松高"
               "<p>日期：2016-06-23");

    QMessageBox::about(this, qstring1, qstring2);
}

void MainWindow::desktopLoad()
{
    if(m_state != desktop)
    {
        m_playBoardDesktop = new WuziPlayBoardDesktop;
        setCentralWidget(m_playBoardDesktop);
        setWindowTitle(ChineseToUTF8_TSG("五子棋游戏 单机版"));
        m_state = desktop;
    }
}

void MainWindow::networkLoad()
{
    if(m_state != network)
    {
        m_playBoardClient = new WuziPlayBoardClient;
        setCentralWidget(m_playBoardClient);
        setWindowTitle(ChineseToUTF8_TSG("五子棋游戏 网络版"));
        m_state = network;

        connect(m_playBoardClient, SIGNAL(connectServerSuccess(QString &)),
                this, SLOT(connectServerSuccess(QString &)));
    }
}

void MainWindow::connectServerSuccess(QString &title)
{
    setWindowTitle(title);
}
