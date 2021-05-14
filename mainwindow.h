#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QFileSystemModel>
#include <QMainWindow>
#include <QTreeView>

#include "highlighter.h"

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void fileClicked(const QModelIndex & index);
    bool save();
    bool saveAs();
    bool saveNew();

private:
    void setupEditor();
    void setupTreeView();
    void setupMenuBar();
    void setupStatusBar();
    bool saveAsAndOpen(bool empty);
    bool saveAs(bool empty);
    bool saveFile(const QString &fileName, bool empty);
    void openFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);

    QString title;

    QFileSystemModel *fileSystemModel;
    QTreeView *tree;

    Highlighter *highlighter;
    QTextEdit *editor;

    QString currentFileName;

    QMenuBar *bar;
    QMenu *fileMenu;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *newAct;
    QAction *findAct;
};

#endif
