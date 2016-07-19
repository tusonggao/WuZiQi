#ifndef COMMONWIDGETHEADER_H
#define COMMONWIDGETHEADER_H

#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QSlider>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QRegion>
#include <QGroupBox>
#include <QComboBox>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QAction>
#include <QSettings>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QClipboard>
#include <QToolButton>
#include <QToolBox>
#include <QStylePainter>
#include <QTextCodec>
#include <QRect>
#include <QDesktopWidget>
#include <QByteArray>
#include <QColor>
#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>
#include <QTimer>
#include <QtCore>
#include <QDebug>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QSplitter>
#include <QTextEdit>
#include <QDockWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QInputDialog>
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QItemSelectionModel>
#include <QDirModel>
#include <QTreeView>
#include <QBitmap>
#include <QListView>
#include <QTableView>
#include <QFileSystemModel>
#include <QScrollArea>
#include <QHeaderView>
#include <QItemDelegate>
#include <QtGlobal>




inline QString ChineseToUTF8_TSG(const QByteArray &str)
{
    return QTextCodec::codecForName("GB18030")->toUnicode(str);
}

#endif // COMMONWIDGETHEADER_H
