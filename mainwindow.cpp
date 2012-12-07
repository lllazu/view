#include "mainwindow.h"

MainWindow::MainWindow()
{
    setupEditor();
    setupTreeView();
    setupMenuBar();
    setupStatusBar();

    title = tr("view is editor.. wow");
    setWindowTitle(title);
}

void MainWindow::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(11);

    editor = new QTextEdit;
    editor->setFont(font);
    editor->setTabStopWidth(20);

    highlighter = new Highlighter(editor->document());
}

void MainWindow::setupTreeView()
{
    fileSystemModel = new QFileSystemModel;
    fileSystemModel->setRootPath(QDir::rootPath());

    tree = new QTreeView;
    tree->setModel(fileSystemModel);

    tree->setAnimated(false);
    tree->setIndentation(20);
    tree->setSortingEnabled(true);

    tree->setHeaderHidden(true);
    tree->setColumnHidden(1, true);
    tree->setColumnHidden(2, true);
    tree->setColumnHidden(3, true);

    connect(tree, SIGNAL(clicked(const QModelIndex &)), this, SLOT(fileClicked(const QModelIndex &)));
}

void MainWindow::setupMenuBar()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(tree);
    mainLayout->addWidget(editor);
    mainLayout->setStretch(0, 3);
    mainLayout->setStretch(1, 5);

    QWidget *window = new QWidget();
    window->setLayout(mainLayout);

    setCentralWidget(window);

    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("New document"));

    connect(newAct, SIGNAL(triggered()), this, SLOT(saveNew()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));

    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));

    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
}

void MainWindow::setupStatusBar()
{
    statusBar();
}


void MainWindow::fileClicked(const QModelIndex & index)
{
    statusBar()->clearMessage();

    QFileInfo info = fileSystemModel->fileInfo(index);
    if (info.isFile()) {
        QString fileName = info.absoluteFilePath();
        if (currentFileName != fileName) {
            openFile(fileName);
        }
    }
}


bool MainWindow::save()
{
    if (currentFileName.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(currentFileName, false);
    }
}

bool MainWindow::saveAs()
{
    return saveAsAndOpen(false);
}

bool MainWindow::saveNew()
{
    return saveAsAndOpen(true);
}

bool MainWindow::saveAsAndOpen(bool empty)
{
    bool saved = saveAs(empty);
    if (saved) {
        openFile(currentFileName);
    }
    return saved;
}

bool MainWindow::saveAs(bool empty)
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName, empty);
}

bool MainWindow::saveFile(const QString &fileName, bool empty)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        statusBar()->showMessage(file.errorString(), 3000);
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    if (!empty) {
        out << editor->toPlainText();
    }
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);

    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::openFile(const QString &fileName)
{
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            editor->setPlainText(file.readAll());
            setCurrentFile(fileName);
        } else {
            statusBar()->showMessage(file.errorString(), 3000);
        }
    }
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    currentFileName = fileName;
    setWindowTitle(fileName);
}
