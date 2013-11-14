#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("njCsSprite");

    m_pgv = new QGraphicsView(this);

    this->setCentralWidget(m_pgv);

    createActions();
    createMenus();
    createToolbarButtons();
}


MainWindow::~MainWindow()
{
    delete ui;

    if(NULL != m_pgv)
    {
        delete m_pgv;
    }


}

void MainWindow::createActions()
{
    QAction* act = new QAction(tr("&New"), this);
    act->setShortcuts(QKeySequence::New);
    act->setStatusTip(tr("Create new css sprite project."));
    connect(act, SIGNAL(triggered()), this, SLOT(newFile()));
    m_actions[ActionKey::NewFile] = act;


}

void MainWindow::createMenus()
{
    m_menuFile = menuBar()->addMenu(tr("&File"));
    m_menuFile->addAction(m_actions[ActionKey::NewFile]);


}

void MainWindow::createToolbarButtons()
{
    m_toolbarFile = ui->mainToolBar;
    m_toolbarFile->addAction(m_actions[ActionKey::NewFile]);


}


void MainWindow::newFile()
{
    QMessageBox::information(this, tr("test"), tr("you clicked New menu or toolbar button"));
}
