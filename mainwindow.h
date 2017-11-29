#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "wuziPlayboardclient.h"
#include "wuziplayboarddesktop.h"


//I am doing this to remind myself to code EVERYDAY! Today, I have written code on my local computer.
//I am doing this to remind myself to code EVERYDAY! Today, I have written code on my local computer.




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void createActions();
    void createMenus();

private slots:
    void desktopLoad();
    void networkLoad();
    void about();
    void connectServerSuccess(QString &title);
    

private:
    enum State{
        non_selected = 0,
        network = 1,
        desktop = 2
    };

    WuziPlayBoardClient *m_playBoardClient;
    WuziPlayBoardDesktop *m_playBoardDesktop;

    QMenu *selectMenu;
    QMenu *helpMenu;

    QAction *desktopLoadAction;
    QAction *networkLoadAction;
    QAction *aboutAction;

    State m_state;
};


#endif // MAINWINDOW_H
