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
    // menu 'File'
    // close
    QAction* act = new QAction(tr("&Quit"), this);
    act->setShortcuts(QKeySequence::Close);
    act->setStatusTip(tr("quite the program."));
    connect(act, SIGNAL(triggered()), this, SLOT(quitProgram()));
    m_actions[ActionKey::Quit] = act;

    // new project actions
    act = new QAction(tr("&New"), this);
    act->setShortcuts(QKeySequence::New);
    act->setStatusTip(tr("Create new css sprite project."));
    connect(act, SIGNAL(triggered()), this, SLOT(newProject()));
    m_actions[ActionKey::NewFile] = act;

    // open an existing project
    act = new QAction(tr("&Open"), this);
    act->setShortcuts(QKeySequence::Open);
    act->setStatusTip(tr("Open css sprite project."));
    connect(act, SIGNAL(triggered()), this, SLOT(openProject()));
    m_actions[ActionKey::OpenFile] = act;

    // save project
    act = new QAction(tr("&Save"), this);
    act->setShortcuts(QKeySequence::Save);
    act->setStatusTip(tr("Save the project."));
    connect(act, SIGNAL(triggered()), this, SLOT(saveProject()));
    m_actions[ActionKey::SaveFile] = act;

    // save as
    act = new QAction(tr("Save &as..."), this);
    act->setShortcuts(QKeySequence::SaveAs);
    act->setStatusTip(tr("Save project as another file."));
    connect(act, SIGNAL(triggered()), this, SLOT(saveProjectAs()));
    m_actions[ActionKey::SaveAs] = act;

    // menu 'Edit'
    // option
    act = new QAction(tr("Option"), this);
    act->setStatusTip(tr("Set background and size parameters."));
    connect(act, SIGNAL(triggered()), this, SLOT(option()));
    m_actions[ActionKey::Option] = act;

    // image manager
    act = new QAction(tr("ImageManager"), this);
    act->setStatusTip(tr("show interface of image-manager."));
    connect(act, SIGNAL(triggered()), this, SLOT(imageManager()));
    m_actions[ActionKey::ImageManager] = act;

    // add image
    act = new QAction(tr("AddImage"), this);
    act->setStatusTip(tr("add an image to current project."));
    connect(act, SIGNAL(triggered()), this, SLOT(addImage()));
    m_actions[ActionKey::AddImage] = act;

    // remove image
    act = new QAction(tr("RemoveImage"), this);
    act->setStatusTip(tr("remove an image from current project."));
    connect(act, SIGNAL(triggered()), this, SLOT(removeImage()));
    m_actions[ActionKey::RemoveImage] = act;

    // generate demo
    act = new QAction(tr("GenerateDemo"), this);
    act->setStatusTip(tr("output file(img, css & html)."));
    connect(act, SIGNAL(triggered()), this, SLOT(generateDemo()));
    m_actions[ActionKey::GenerateDemo] = act;

    // menu 'Range'
    // random
    act = new QAction(tr("Random"), this);
    act->setStatusTip(tr(""));
    connect(act, SIGNAL(triggered()), this, SLOT(rangeRandom()));
    m_actions[ActionKey::Ramdom] = act;

    // horizontal
    act = new QAction(tr("Horizontal"), this);
    act->setStatusTip(tr(""));
    connect(act, SIGNAL(triggered()), this, SLOT(rangeHorizontal()));
    m_actions[ActionKey::Horizontal] = act;

    // vertical
    act = new QAction(tr("Vertical"), this);
    act->setStatusTip(tr(""));
    connect(act, SIGNAL(triggered()), this, SLOT(rangeVertical()));
    m_actions[ActionKey::Vertical] = act;

    // MinArea
    act = new QAction(tr("MinArea"), this);
    act->setStatusTip(tr("apply algorithm to range rectangles to make them close together."));
    connect(act, SIGNAL(triggered()), this, SLOT(rangeMinArea()));
    m_actions[ActionKey::MinArea] = act;

    // menu 'Tool'
    // analyse image
    act = new QAction(tr("AnalyseImage"), this);
    act->setStatusTip(tr("Analyse an image."));
    connect(act, SIGNAL(triggered()), this, SLOT(analyseImage()));
    m_actions[ActionKey::AnalyseImage] = act;

    // manual operate
    act = new QAction(tr("ManualOperate"), this);
    act->setStatusTip(tr(""));
    connect(act, SIGNAL(triggered()), this, SLOT(manualOperate()));
    m_actions[ActionKey::ManualOperate] = act;

    // auto split
    act = new QAction(tr("AutoSplit"), this);
    act->setStatusTip(tr(""));
    connect(act, SIGNAL(triggered()), this, SLOT(autoSplit()));
    m_actions[ActionKey::AutoSplit] = act;

}

void MainWindow::createMenus()
{
    // menu 'File'
    m_menuFile = menuBar()->addMenu(tr("&File"));
    m_menuFile->addAction(m_actions[ActionKey::NewFile]);
    m_menuFile->addAction(m_actions[ActionKey::OpenFile]);
    m_menuFile->addAction(m_actions[ActionKey::SaveFile]);
    m_menuFile->addAction(m_actions[ActionKey::SaveAs]);
    m_menuFile->addAction(m_actions[ActionKey::Quit]);

    // menu 'Edit'
    m_menuFile = menuBar()->addMenu(tr("&Edit"));
    m_menuFile->addAction(m_actions[ActionKey::Option]);
    m_menuFile->addAction(m_actions[ActionKey::ImageManager]);
    m_menuFile->addAction(m_actions[ActionKey::AddImage]);
    m_menuFile->addAction(m_actions[ActionKey::RemoveImage]);
    m_menuFile->addAction(m_actions[ActionKey::GenerateDemo]);

    // menu 'Range'
    m_menuFile = menuBar()->addMenu(tr("&Range"));
    m_menuFile->addAction(m_actions[ActionKey::Ramdom]);
    m_menuFile->addAction(m_actions[ActionKey::Horizontal]);
    m_menuFile->addAction(m_actions[ActionKey::Vertical]);
    m_menuFile->addAction(m_actions[ActionKey::MinArea]);

    // menu 'Tool'
    m_menuFile = menuBar()->addMenu(tr("&Tool"));
    m_menuFile->addAction(m_actions[ActionKey::AnalyseImage]);
    m_menuFile->addAction(m_actions[ActionKey::ManualOperate]);
    m_menuFile->addAction(m_actions[ActionKey::AutoSplit]);

}

void MainWindow::createToolbarButtons()
{
//    m_toolbarFile = ui->mainToolBar;
//    m_toolbarFile->addAction(m_actions[ActionKey::NewFile]);
//    m_toolbarFile->addAction(m_actions[ActionKey::OpenFile]);
//    m_toolbarFile->addAction(m_actions[ActionKey::SaveFile]);
}

void MainWindow::quitProgram()
{
    exit(0);
    // to do
}

void MainWindow::newProject()
{
    QMessageBox::information(this, tr("test"), tr("you clicked New menu or toolbar button"));
}

void MainWindow::openProject()
{
    // to do
}

void MainWindow::saveProject()
{
    // to do
}

void MainWindow::saveProjectAs()
{
    // to do
}

void MainWindow::option()
{
}

void MainWindow::imageManager()
{
}

void MainWindow::addImage()
{
}

void MainWindow::removeImage()
{
}

void MainWindow::generateDemo()
{
}

//menu 'Range'
void MainWindow::rangeRandom()
{
}

void MainWindow::rangeHorizontal()
{
}

void MainWindow::rangeVertical()
{
}

void MainWindow::rangeMinArea()
{
}

//menu 'Tool'
void MainWindow::analyseImage()
{
}

void MainWindow::manualOperate()
{
}

void MainWindow::autoSplit()
{
}
