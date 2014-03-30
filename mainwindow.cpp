/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#include <QDir>
#include <QXmlStreamReader>
#include <QFileDialog>
#include <QImage>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_strLanguage = tr("English");

    m_spriteMgr = NULL;
    m_pgv = NULL;

    m_settings = new QSettings(QString("njfever"), STR_MY_TITLE);

    initLanguage();

    createViews();

    m_bProjectIsNull = true;
    m_bIsEditedWithoutSave = false;
    m_bIsSceneMouseEvent = false;

    m_strWorkDirectory = "";

    createActions();

    createMenus();

    createToolbarButtons();

    newProject();

    m_actions[ActionKey::SpriteLock]->setChecked(true);
    lockSprites();
}


MainWindow::~MainWindow()
{
    delete ui;

    if(NULL != m_pgv)
    {
        delete m_pgv;
    }

    if(NULL != m_spriteMgr)
    {
        delete m_spriteMgr;
    }

}

void MainWindow::initLanguage()
{
    QDir dir(":/res/lang");
    QString strLang = m_settings->value(STR_MY_LANGUAGE).toString().toLower();

    QStringList fileNames = dir.entryList(QStringList("*.qm"), QDir::Files,
                                          QDir::Name);  // njcs_chn.qm ; njcs_en.qm
    QString strLangPath;
    QAction* act = NULL;
    QTranslator translator;

    m_actGrpLang = new QActionGroup(this);

    foreach(QString fileName, fileNames)
    {
        translator.load(dir.filePath(fileName));

        if(fileName.length() > 8)
        {
            act = m_actGrpLang->addAction(translator.translate("MainWindow", "English"));
            act->setData(fileName.mid(5, fileName.length() - 8));
            act->setCheckable(true);
            if(strLang == act->data())
            {
                act->setChecked(true);
                strLangPath = dir.filePath(fileName);
            }
        }
    }

    connect(m_actGrpLang, SIGNAL(triggered(QAction*)), this, SLOT(switchLanguage(QAction*)));
}

void MainWindow::createActions()
{
    // menu 'File'
    // close
    QAction* act = new QAction(QIcon(":/res/img/close.png"), tr("&Quit"), this);
    act->setShortcuts(QKeySequence::Close);
    act->setStatusTip(tr("Quit the program."));
    connect(act, SIGNAL(triggered()), this, SLOT(quitProgram()));
    m_actions[ActionKey::Quit] = act;

    // new project actions
    act = new QAction(QIcon(":/res/img/new.png"), tr("&New..."), this);
    act->setShortcuts(QKeySequence::New);
    act->setStatusTip(tr("Create new css sprite project."));
    connect(act, SIGNAL(triggered()), this, SLOT(newProject()));
    m_actions[ActionKey::NewFile] = act;

    // open an existing project
    act = new QAction(QIcon(":/res/img/open.png"), tr("&Open..."), this);
    act->setShortcuts(QKeySequence::Open);
    act->setStatusTip(tr("Open an existing css sprite project."));
    connect(act, SIGNAL(triggered()), this, SLOT(openProject()));
    m_actions[ActionKey::OpenFile] = act;

    // save project
    act = new QAction(QIcon(":/res/img/save.png"), tr("&Save"), this);
    act->setShortcuts(QKeySequence::Save);
    act->setStatusTip(tr("Save the project."));
    connect(act, SIGNAL(triggered()), this, SLOT(saveProject()));
    m_actions[ActionKey::SaveFile] = act;

    // save as
    act = new QAction(tr("Save &as..."), this);
    act->setShortcuts(QKeySequence::SaveAs);
    act->setStatusTip(tr(""));
    connect(act, SIGNAL(triggered()), this, SLOT(saveProjectAs()));
    m_actions[ActionKey::SaveAs] = act;

    // pproject setup
    act = new QAction(QIcon(":/res/img/option.png"), tr("S&etup..."), this);
    act->setStatusTip(tr("Setup the project."));
    connect(act, SIGNAL(triggered()), this, SLOT(setupProject()));
    m_actions[ActionKey::ProjectSetup] = act;

    // menu 'Sprite'
    // add image
    act = new QAction(QIcon(":/res/img/addImage.png"), tr("Add Image"), this);
    act->setStatusTip(tr("Add image(s) to current project."));
    connect(act, SIGNAL(triggered()), this, SLOT(addImage()));
    m_actions[ActionKey::AddImage] = act;

    // remove image
    act = new QAction(QIcon(":/res/img/removeImage.png"), tr("Remove Image"), this);
    act->setStatusTip(tr("Remove selected image(s) from current project."));
    connect(act, SIGNAL(triggered()), this, SLOT(removeImage()));
    m_actions[ActionKey::RemoveImage] = act;

    // SpriteNoneFixed,
    act = new QAction(QIcon(":/res/img/deselectAll.png"), tr("&Fixed None"), this);
    act->setStatusTip(tr("Set all sprites not fixed."));
    connect(act, SIGNAL(triggered()), this, SLOT(switchNoneFixed()));
    m_actions[ActionKey::SpriteNoneFixed] = act;

    // SpriteAllFixed,
    act = new QAction(QIcon(":/res/img/selectAll.png"), tr("&Fixed All"), this);
    act->setStatusTip(tr("Set all sprites fixed."));
    connect(act, SIGNAL(triggered()), this, SLOT(switchAllFixed()));
    m_actions[ActionKey::SpriteAllFixed] = act;

    //SpeiteSetFixed,
    act = new QAction(QIcon(":/res/img/select.png"), tr("Fixed Swicth"), this);
    act->setStatusTip(tr("Switch selected sprite(s) fixed or not."));
    connect(act, SIGNAL(triggered()), this, SLOT(switchFixed()));
    m_actions[ActionKey::SpriteSetFixed] = act;

    // Generate Demo
    act = new QAction(QIcon(":/res/img/genDemo.png"), tr("&Generate Demo..."), this);
    act->setStatusTip(tr("Generate css and html file."));
    connect(act, SIGNAL(triggered()), this, SLOT(generateDemo()));
    m_actions[ActionKey::GenerateDemo] = act;


    // Sprite Lock
    act = new QAction(QIcon(":/res/img/fixed.png"), tr("&Lock Postion"), this);
    act->setCheckable(true);

    act->setChecked(false);
    act->setStatusTip(tr("Lock postion of sprites."));
    connect(act, SIGNAL(triggered()), this, SLOT(lockSprites()));
    m_actions[ActionKey::SpriteLock] = act;

    // horizontal
    act = new QAction(QIcon(":/res/img/horLine.png"), tr("&Horizontal"), this);
    act->setStatusTip(tr(""));
    connect(act, SIGNAL(triggered()), this, SLOT(arrangeHorizontal()));
    m_actions[ActionKey::ArrangeHorizontal] = act;

    // vertical
    act = new QAction(QIcon(":/res/img/verLine.png"), tr("&Vertical"), this);
    act->setStatusTip(tr(""));
    connect(act, SIGNAL(triggered()), this, SLOT(arrangeVertical()));
    m_actions[ActionKey::ArrangeVertical] = act;

    // Auto
    act = new QAction(QIcon(":/res/img/autoLine.png"), tr("&Auto"), this);
    act->setStatusTip(tr("Make items close together by algorithm."));
    connect(act, SIGNAL(triggered()), this, SLOT(arrangeAuto()));
    m_actions[ActionKey::ArrangeAuto] = act;

    // Option
    act = new QAction(QIcon(":/res/img/option.png"), tr("&Option..."), this);
    act->setStatusTip(tr("Setup arrange parampeter."));
    connect(act, SIGNAL(triggered()), this, SLOT(arrangeOption()));
    m_actions[ActionKey::ArrangeOption] = act;

    // help

    // about program
    act = new QAction(tr("&About..."), this);
    act->setStatusTip(tr("About this program."));
    connect(act, SIGNAL(triggered()), this, SLOT(aboutProgram()));
    m_actions[ActionKey::HelpAboutApp] = act;

    // about qt
    act = new QAction(tr("About Qt..."), this);
    connect(act, SIGNAL(triggered()), this, SLOT(aboutQt()));
    m_actions[ActionKey::HelpAboutQt] = act;


    // menu 'Tool'
    // analyse image
//    act = new QAction(tr("AnalyseImage"), this);
//    act->setStatusTip(tr("Analyse an image."));
//    connect(act, SIGNAL(triggered()), this, SLOT(analyseImage()));
//    m_actions[ActionKey::AnalyseImage] = act;

//    // manual operate
//    act = new QAction(tr("ManualOperate"), this);
//    act->setStatusTip(tr(""));
//    connect(act, SIGNAL(triggered()), this, SLOT(manualOperate()));
//    m_actions[ActionKey::ManualOperate] = act;

//    // auto split
//    act = new QAction(tr("AutoSplit"), this);
//    act->setStatusTip(tr(""));
//    connect(act, SIGNAL(triggered()), this, SLOT(autoSplit()));
//    m_actions[ActionKey::AutoSplit] = act;

}

void MainWindow::createMenus()
{
    // menu 'File'
    m_menuFile = menuBar()->addMenu(tr("&File"));
    m_menuFile->addAction(m_actions[ActionKey::NewFile]);
    m_menuFile->addAction(m_actions[ActionKey::OpenFile]);
    m_menuFile->addAction(m_actions[ActionKey::SaveFile]);
    m_menuFile->addAction(m_actions[ActionKey::SaveAs]);
    m_menuFile->addSeparator();
    m_menuFile->addAction(m_actions[ActionKey::ProjectSetup]);
    m_menuFile->addSeparator();
    m_menuFile->addAction(m_actions[ActionKey::Quit]);

    // menu 'Sprite'
    m_menuFile = menuBar()->addMenu(tr("&Sprite"));
    m_menuFile->addAction(m_actions[ActionKey::AddImage]);
    m_menuFile->addAction(m_actions[ActionKey::RemoveImage]);
    m_menuFile->addSeparator();
    m_menuFile->addAction(m_actions[ActionKey::SpriteAllFixed]);
    m_menuFile->addAction(m_actions[ActionKey::SpriteNoneFixed]);
    m_menuFile->addAction(m_actions[ActionKey::SpriteSetFixed]);

    m_menuFile->addSeparator();
    m_menuFile->addAction(m_actions[ActionKey::GenerateDemo]);
    m_menuFile->addSeparator();
    m_menuFile->addAction(m_actions[ActionKey::SpriteLock]);

    // menu 'Arrange'
    m_menuFile = menuBar()->addMenu(tr("&Arrange"));
    m_menuFile->addAction(m_actions[ActionKey::ArrangeHorizontal]);
    m_menuFile->addAction(m_actions[ActionKey::ArrangeVertical]);
    m_menuFile->addAction(m_actions[ActionKey::ArrangeAuto]);
    m_menuFile->addSeparator();
    m_menuFile->addAction(m_actions[ActionKey::ArrangeOption]);

    // menu 'help'
    m_menuFile = menuBar()->addMenu(tr("&Help"));

    m_menuFile->addMenu(tr("&Language"))->addActions(m_actGrpLang->actions());

    m_menuFile->addAction(m_actions[ActionKey::HelpAboutApp]);
    m_menuFile->addAction(m_actions[ActionKey::HelpAboutQt]);

    // menu 'Tool'
    //    m_menuFile = menuBar()->addMenu(tr("&Tool"));
    //    m_menuFile->addAction(m_actions[ActionKey::AnalyseImage]);
    //    m_menuFile->addAction(m_actions[ActionKey::ManualOperate]);
    //    m_menuFile->addAction(m_actions[ActionKey::AutoSplit]);

}

void MainWindow::createToolbarButtons()
{
    m_toolbarFile = ui->mainToolBar;
    m_toolbarFile->setWindowTitle(tr("File"));
    m_toolbarFile->addAction(m_actions[ActionKey::NewFile]);
    m_toolbarFile->addAction(m_actions[ActionKey::OpenFile]);
    m_toolbarFile->addAction(m_actions[ActionKey::SaveFile]);

    QToolBar* toolBar = addToolBar(tr("Sprite"));
    toolBar->addAction(m_actions[ActionKey::AddImage]);
    toolBar->addAction(m_actions[ActionKey::RemoveImage]);
    toolBar->addSeparator();
    toolBar->addAction(m_actions[ActionKey::SpriteAllFixed]);
    toolBar->addAction(m_actions[ActionKey::SpriteNoneFixed]);
    toolBar->addAction(m_actions[ActionKey::SpriteSetFixed]);
    toolBar->addSeparator();
    toolBar->addAction(m_actions[ActionKey::GenerateDemo]);
    toolBar->addSeparator();
    toolBar->addAction(m_actions[ActionKey::SpriteLock]);

    toolBar = addToolBar(tr("Arranger"));
    toolBar->addAction(m_actions[ActionKey::ArrangeHorizontal]);
    toolBar->addAction(m_actions[ActionKey::ArrangeVertical]);
    toolBar->addAction(m_actions[ActionKey::ArrangeAuto]);
    toolBar->addSeparator();
    toolBar->addAction(m_actions[ActionKey::ArrangeOption]);

}

void MainWindow::createViews()
{
    m_h1Splitter = new QSplitter;
    QSplitter* h2Splitter = new QSplitter;

    m_vSplitter = new QSplitter;

    m_vSplitter->setOrientation(Qt::Vertical);
    m_vSplitter->addWidget(m_h1Splitter);
    m_vSplitter->addWidget(h2Splitter);

    // sprites
    QGroupBox* pgb = new QGroupBox;
    pgb->setTitle(tr("Sprites"));
    m_h1Splitter->addWidget(pgb);

    QBoxLayout* pbl = new QBoxLayout(QBoxLayout::LeftToRight);
    pgb->setLayout(pbl);

    m_tableSprite = new CMyTableView;
    connect(m_tableSprite, SIGNAL(rowSelectionChanged()), this, SLOT(rowSelectionChanged()));
    connect(this, SIGNAL(clearSpriteTableSelection()), m_tableSprite, SLOT(clearSelection()));

    m_tableSprite->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableSprite->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableSprite->setStyle(QStyleFactory::create("Windows"));

    QStandardItemModel* itemModel = new QStandardItemModel;
    // set table header
    QStringList sl;
    sl<<tr("arrange")<<tr("crc32")<<tr("file");
    itemModel = new QStandardItemModel;
    itemModel->setHorizontalHeaderLabels(sl);

    m_tableSprite->setModel(itemModel);
    m_tableSprite->setHorizontalScrollMode(QTableView::ScrollPerPixel);
    m_tableSprite->setVerticalScrollMode(QTableView::ScrollPerPixel);

    pbl->addWidget(m_tableSprite);

    // preview
    pgb = new QGroupBox;
    pgb->setTitle(tr("Preview"));
    m_h1Splitter->addWidget(pgb);

    pbl = new QBoxLayout(QBoxLayout::LeftToRight);
    pgb->setLayout(pbl);

    m_pgv = new CMyGraphicsView(this);

    updateSpriteMgrAndView();

    pbl->addWidget(m_pgv);

    // info
    pgb = new QGroupBox;
    pgb->setTitle(tr("Info"));
    h2Splitter->addWidget(pgb);

    pbl = new QBoxLayout(QBoxLayout::LeftToRight);
    pgb->setLayout(pbl);

    m_txtInfo = new QTextEdit;
    pbl->addWidget(m_txtInfo);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(m_vSplitter);
    ui->centralWidget->setLayout(layout);

    setDefaultSizeOfSplitter();

}

void MainWindow::setDefaultSizeOfSplitter()
{
    // set default size
    QList<int> sizes;
    sizes<<100<<200;
    m_h1Splitter->setSizes(sizes);
    sizes.clear();
    sizes<<200<<100;
    m_vSplitter->setSizes(sizes);
}

void MainWindow::updateSpriteTableBySpriteMgr()
{
    QStandardItemModel* model = (QStandardItemModel*)m_tableSprite->model();

    while(model->rowCount() > 0) model->removeRow(0);

    CSprite* sp = NULL;
    for(int i = 0; i < m_spriteMgr->items().count(); i++)
    {
        sp = (CSprite*)m_spriteMgr->items()[i];
        model->setItem(i, 0, new QStandardItem(tr("Fixed")));
        model->item(i, 0)->setCheckable(true);
        model->item(i, 0)->setCheckState(sp->isFixed()?Qt::Checked:Qt::Unchecked);

        model->setItem(i, 1, new QStandardItem(QString("%1").arg(sp->getCRC32(), 0, 16)));
        model->setItem(i, 2, new QStandardItem(sp->getFilePath()));

        if(sp->isSelected()) m_tableSprite->selectRow(i);

    }
}

void MainWindow::updateWindowTitleAndMenuStatus()
{
    if(m_bProjectIsNull)
    {
        this->setWindowTitle(STR_MY_TITLE);
        m_actions[ActionKey::ProjectSetup]->setEnabled(false);
        m_actions[ActionKey::SaveFile]->setEnabled(false);
        m_actions[ActionKey::SaveAs]->setEnabled(false);

        m_actions[ActionKey::AddImage]->setEnabled(false);
        m_actions[ActionKey::RemoveImage]->setEnabled(false);
        m_actions[ActionKey::SpriteAllFixed]->setEnabled(false);
        m_actions[ActionKey::SpriteNoneFixed]->setEnabled(false);
        m_actions[ActionKey::SpriteSetFixed]->setEnabled(false);
        m_actions[ActionKey::SpriteLock]->setEnabled(false);
        m_actions[ActionKey::GenerateDemo]->setEnabled(false);

        m_actions[ActionKey::ArrangeHorizontal]->setEnabled(false);
        m_actions[ActionKey::ArrangeVertical]->setEnabled(false);
        m_actions[ActionKey::ArrangeAuto]->setEnabled(false);


    }
    else
    {
        QString t = STR_MY_TITLE + QString(" - ") + m_projConfig.strDocName;
        this->setWindowTitle(t);
        m_actions[ActionKey::ProjectSetup]->setEnabled(true);
        m_actions[ActionKey::SaveFile]->setEnabled(true);
        m_actions[ActionKey::SaveAs]->setEnabled(true);

        m_actions[ActionKey::AddImage]->setEnabled(true);
        m_actions[ActionKey::RemoveImage]->setEnabled(true);
        m_actions[ActionKey::SpriteAllFixed]->setEnabled(true);
        m_actions[ActionKey::SpriteNoneFixed]->setEnabled(true);
        m_actions[ActionKey::SpriteSetFixed]->setEnabled(true);
        m_actions[ActionKey::SpriteLock]->setEnabled(true);
        m_actions[ActionKey::GenerateDemo]->setEnabled(true);

        m_actions[ActionKey::ArrangeHorizontal]->setEnabled(true);
        m_actions[ActionKey::ArrangeVertical]->setEnabled(true);
        m_actions[ActionKey::ArrangeAuto]->setEnabled(true);
    }
}

void MainWindow::updateSpriteMgrAndView()
{
    if(m_spriteMgr != NULL)
    {
        delete m_spriteMgr;
        m_spriteMgr = NULL;
    }

    m_spriteMgr = new CSpriteMgr();
    m_spriteMgr->setArrangerParam(m_projConfig);

    updateSpriteTableBySpriteMgr();

    m_pgv->setCacheMode(QGraphicsView::CacheBackground);

    m_pgv->setDragMode(QGraphicsView::RubberBandDrag);
    m_pgv->setInteractive(true);

    m_pgv->setScene((QGraphicsScene*) m_spriteMgr);

    connect(m_spriteMgr, SIGNAL(mouseReleased()), this, SLOT(spriteSelectionChanged()));

}

void MainWindow::appendInfo(QString strInfo)
{
    m_txtInfo->append(strInfo);
}

void MainWindow::appendInfoByAddSprite(const QString &strFile, int nErr)
{
    if(nErr > 0)
    {
       appendInfo(tr("Image <b>%1</b> has been loaded...skip").arg(strFile));
    }
    else if(nErr < 0)
    {
        appendInfo(tr("Image <b>%1</b> loaded failed").arg(strFile));
    }
    else
    {
        appendInfo(tr("Image <b>%1</b> loaded.").arg(strFile));
    }

}

void MainWindow::appendInfoBySelectSprite()
{
    int nCount = m_spriteMgr->selectedItems().count();
    if(nCount > 1)
    {
        appendInfo(tr("Selected %1 item(s).").arg(nCount));
    }
    else if(nCount == 1)
    {
        QRect rectBG, rectStyle;
        CSprite* sp = (CSprite*)m_spriteMgr->selectedItems()[0];

        m_spriteMgr->getMinRect(rectBG);

        getSpriteCssStyle(sp, rectBG, rectStyle);
        appendInfo(QString(".id%1{background-position: %2px %3px; width: %4px; height: %5px;}")
                             .arg(sp->getID()).arg(rectStyle.x()).arg(rectStyle.y()).arg(rectStyle.width()).arg(rectStyle.height()));
    }
}

void MainWindow::setInfo(QString strInfo)
{
    m_txtInfo->setText(strInfo);
}

void MainWindow::quitProgram()
{
    this->close();
}

bool MainWindow::isEditedWithoutSave()
{
    if(m_bProjectIsNull)
    {
        return false;
    }
    else if(!m_bIsEditedWithoutSave)
    {
        return false;
    }

    return true;
}

void MainWindow::setEditedWithoutSave()
{
    if(m_bProjectIsNull)
    {
        return;
    }

    m_bIsEditedWithoutSave = true;
}

void MainWindow::getSpriteCssStyle(const CSprite *sp, const QRect& rectBG, QRect& rectStyle) const
{
    rectStyle = QRect(0 - (int)(sp->x() - rectBG.left()), 0 - (int)(sp->y() - rectBG.top()), sp->getWidth(), sp->getHeight());
}

void MainWindow::updateSpriteFixedStatus()
{
    bool bFixed;
    QStandardItemModel* model = (QStandardItemModel*)m_tableSprite->model();

    CSprite* sp = NULL;
    for(int i = 0; i < m_spriteMgr->items().count(); i++)
    {
        sp = (CSprite*)m_spriteMgr->items()[i];
        bFixed = false;
        QString strCRC32 = QString("%1").arg(sp->getCRC32(), 0, 16);
        QString strCRC32Row = "";
        for(int j = 0; j < model->rowCount(); j++)
        {

            strCRC32Row = model->item(j, 1)->text();

            if( strCRC32Row == strCRC32)
            {
                if(model->item(j , 0)->checkState() == Qt::Checked)
                {
                    bFixed = true;
                }
                break;
            }

        }

        sp->setFixed(bFixed);
    }
}

int MainWindow::writeDemoHtml(const QString &strFile, const QString& strBgImage)
{
    QList<QString> listStyles;
    QRect rectBG, rectStyle;
    m_spriteMgr->getMinRect(rectBG);

    for(int i = 0; i < m_spriteMgr->items().count(); i++)
    {
        getSpriteCssStyle((CSprite*)m_spriteMgr->items()[i], rectBG, rectStyle);
        listStyles.push_back(QString(".s%1{background-position: %2px %3px; width: %4px; height: %5px;}")
                             .arg(i).arg(rectStyle.x()).arg(rectStyle.y()).arg(rectStyle.width()).arg(rectStyle.height()));
    }

    QString strHtml = QString(
            "<html>\n"
            "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n"
            "<head>\n"
            "<title>%1 - %2</title>\n"
            "<style>\n"
            "body{background:url(transparentBG.gif);}\n"
            ".text{background:#eee; font-size:12px;}\n"
            ".bg{background:url(%3); margin: 0 auto;}\n"
            ".wrap{margin-bottom: 10px; text-align: center;}\n")
            .arg(STR_MY_TITLE)
            .arg(m_projConfig.strDocName)
            .arg(strBgImage);

    for(int i = 0; i < listStyles.count(); i++)
    {
        strHtml += listStyles[i] + "\n";
    }

    strHtml += QString(
                "</style>\n"
                "</head>\n"
                "<body>\n");

    for(int i = 0; i < listStyles.count(); i++)
    {
        strHtml += QString(
                    "<div class=\"wrap\">\n"
                    "<div class=\"bg s%1\"></div>\n"
                    "<div class=\"text\">%2</div>\n"
                    "</div>\n").arg(i).arg(listStyles[i]);
    }

    strHtml += QString(
                "</body>\n"
                "</html>");

    QFile f(strFile);
    if(f.open(QFile::WriteOnly))
    {
        f.write(strHtml.toUtf8());
        return 0;
    }

    return -1;
}

int MainWindow::createCssSpriteImage(const QString &strFile)
{
    QRect rect;
    QImage img;

    m_spriteMgr->getMinRect(rect);

    if(strFile.endsWith(QString("png")))
    {
        img = QImage(rect.width(), rect.height(), QImage::Format_ARGB32);
        img.fill(QColor(0, 0, 0, 0));
    }
    else
    {
        img = QImage(rect.width(), rect.height(), QImage::Format_RGB32);
        img.fill(QColor(255, 255, 255));
    }

    CSprite* sp = NULL;

    QPainter painter;

    painter.begin(&img);

    for(int i = m_spriteMgr->items().count() - 1; i >= 0; i--)
    {
        sp = (CSprite*)m_spriteMgr->items()[i];
        painter.drawPixmap(sp->x() - rect.left(),
                           sp->y() - rect.top(),
                           sp->getWidth(),
                           sp->getHeight(),
                           *sp->getPixmap());
    }

    painter.end();

    if(!img.save(strFile))
    {
        QMessageBox::warning(this, tr("Save file failed"), tr("Save image:<b>%1</b> failed.").arg(strFile));
        return -1;
    }

    return 0;
}

void MainWindow::newProject()
{
    if(isEditedWithoutSave())
    {
        if(QMessageBox::No == QMessageBox::question(this, tr("Haven't Save"), tr("The project haven't been save, would you wanna create a new project?"),
                                                    QMessageBox::Yes | QMessageBox::No,
                                                    QMessageBox::No))
        {
            return;
        }
    }

    closeProject();

    ProjectConfig cc;
    m_projConfig = cc;
    m_bProjectIsNull = false;
    updateWindowTitleAndMenuStatus();
}

void MainWindow::openProject()
{
    if(isEditedWithoutSave())
    {
        if(QMessageBox::No == QMessageBox::question(this, tr("Haven't Save"), tr("The project haven't been save, would you wanna create a new project?"),
                                                    QMessageBox::Yes | QMessageBox::No,
                                                    QMessageBox::No))
        {
            return;
        }
    }

    closeProject();

    QString strPath = QFileDialog::getExistingDirectory(this, tr("Choose Project's Directory"), m_strWorkDirectory);

    if(strPath.length() < 1)
    {
        return;
    }


    m_strWorkDirectory = strPath + (strPath.endsWith(QString("/"))?"":"/");
    QString xmlFile = m_strWorkDirectory + STR_MY_PROJ_XML_FILE_NAME;
    if(0 == loadConfigFromXml(xmlFile))
    {
        m_bProjectIsNull = false;
    }

    updateWindowTitleAndMenuStatus();
    updateSpriteTableBySpriteMgr();
}

void MainWindow::saveProject()
{
    if(m_strWorkDirectory.length() < 1)
    {
        saveProjectAs();
    }
    else
    {
        QString strXmlFile = m_strWorkDirectory + STR_MY_PROJ_XML_FILE_NAME;
        if(QFile(strXmlFile).exists()
                && QMessageBox::No == QMessageBox::question(this,
                                                           tr("Overwrite Existing File?"),
                                                           tr("File <b>%1</b> will be overwrited, continue?").arg(strXmlFile),
                                                           QMessageBox::Yes | QMessageBox::No,
                                                           QMessageBox::No))
        {
            return;
        }

        bool bCopyImg = true;
        if(m_spriteMgr->items().count() > 0 && QMessageBox::No == QMessageBox::question(this,
                                                   tr("Copy Image File To Specified Directory?"),
                                                   tr("Copy all image files to your specified directory is very usefull for packing, eg:rar or zip archive."),
                                                   QMessageBox::Yes | QMessageBox::No,
                                                   QMessageBox::Yes))
        {
            bCopyImg = false;
        }

        saveConfigToXml(strXmlFile, bCopyImg);
    }

}

void MainWindow::saveProjectAs()
{
    QString strPath = QFileDialog::getExistingDirectory(this, tr("Choose Directory To Save"), m_strWorkDirectory);

    if(strPath.length() < 1)
    {
        return;
    }

    m_strWorkDirectory = strPath + (strPath.endsWith(QString("/"))?"":"/");
    saveProject();
}

void MainWindow::setupProject()
{
    CSetupProjectDlg setupDlg;
    setupDlg.setDocName(m_projConfig.strDocName);
    setupDlg.setCanvasSize(m_spriteMgr->width(), m_spriteMgr->height());
    if(QDialog::Accepted == setupDlg.exec())
    {
        m_projConfig.strDocName = setupDlg.getDocName();
        updateWindowTitleAndMenuStatus();
    }
}

void MainWindow::switchAllFixed()
{
    m_spriteMgr->switchFixed(1);
}

void MainWindow::switchNoneFixed()
{
    m_spriteMgr->switchFixed(2);
}

void MainWindow::switchFixed()
{
    m_spriteMgr->switchFixed(0);
}

void MainWindow::rowSelectionChanged()
{
    m_spriteMgr->update();
    bool bSelected = false;

    if(!m_bIsSceneMouseEvent)
    {
        CSprite* sp = NULL;
        for(int i = 0; i < m_spriteMgr->items().count(); i++)
        {
            sp = (CSprite*)m_spriteMgr->items()[i];
            bSelected = false;
            QString strCRC32 = QString("%1").arg(sp->getCRC32(), 0, 16);
            QString strCRC32Row = "";
            QModelIndexList pmil = m_tableSprite->selectionModel()->selectedRows();
            for(int j = 0; j < pmil.size(); j++)
            {

                strCRC32Row = ((QStandardItemModel*)pmil[j].model())->item(pmil[j].row(), 1)->text();

                if( strCRC32Row == strCRC32)
                {
                    bSelected = true;
                    break;
                }

            }

            m_spriteMgr->items()[i]->setSelected(bSelected);
        }
    }
}

void MainWindow::spriteSelectionChanged()
{
    CSprite* sp = NULL;

    m_bIsSceneMouseEvent = true;
    emit clearSpriteTableSelection();

    m_tableSprite->setSelectionMode(QAbstractItemView::MultiSelection);

    QStandardItemModel* model = (QStandardItemModel*)m_tableSprite->model();
    QString strCRC32 = "";

    for(int i = 0; i < m_spriteMgr->items().count(); i++)
    {
        sp = (CSprite*)m_spriteMgr->items()[i];

        for(int j = 0; j < model->rowCount(); j++)
        {
            strCRC32 = QString("%1").arg(sp->getCRC32(), 0, 16);

            if(model->item(j, 1)->text() == strCRC32)
            {
                model->item(j, 0)->setCheckState(sp->isFixed()?Qt::Checked:Qt::Unchecked);
                if(sp->isSelected())
                {
                    m_tableSprite->selectRow(j);
                    break;
                }
            }
        }

    }

    m_tableSprite->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_bIsSceneMouseEvent = false;

    appendInfoBySelectSprite();
}


void MainWindow::lockSprites()
{
    CSprite* sp = NULL;
    QAction* act = m_actions[ActionKey::SpriteLock];

    if(m_spriteMgr != NULL && m_spriteMgr->items().count() > 0)
    {
        if(act->isChecked())
        {
            for(int i = 0; i < m_spriteMgr->items().count(); i++)
            {
                sp = (CSprite*)m_spriteMgr->items()[i];
                sp->setFlags(QGraphicsItem::ItemIsSelectable);
            }
        }
        else
        {
            for(int i = 0; i < m_spriteMgr->items().count(); i++)
            {
                sp = (CSprite*)m_spriteMgr->items()[i];
                sp->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
            }
        }
    }
}

void MainWindow::arrangeOption()
{
    CArrangeOptionDlg dlg;

    dlg.setOptionValue(m_projConfig);
    if(QDialog::Accepted == dlg.exec())
    {
         ProjectConfig pc;
         dlg.getOptionValue(pc);

         if(m_spriteMgr->setArrangerParam(pc) != 0)
         {
             QMessageBox::warning(this, tr("Applay arrange parameter failed!")
                                  ,tr("Applay arrange parameter failed!\n Now will be restore parameter."));
             m_spriteMgr->setArrangerParam(m_projConfig);
         }
         else
         {
            m_projConfig = pc;
         }
    }
}

void MainWindow::addImage()
{
    int err = 0;
    QFileDialog fd;
    fd.setWindowTitle(tr("Select file to add"));
    fd.setParent(this);
    QStringList sl;
    sl<<tr("Image files (*.bmp *.jpg *.png *.gif)");
    sl<<tr("All files (*.*)");
    fd.setNameFilters(sl);
    fd.setFileMode(QFileDialog::ExistingFiles);

    int nCount = 0;

    if(QFileDialog::Accepted == fd.exec())
    {
        sl = fd.selectedFiles();

        appendInfo(tr("Begin load image(s)(will load %1 item(s))").arg(sl.count()));

        foreach (QString file, sl)
        {
            err = m_spriteMgr->addSprite(file, QPoint(0,0), false);
            appendInfoByAddSprite(file, err);

            if(err < 0)
            {
                if(QMessageBox::No == QMessageBox::warning(this,
                                                           tr("Add image file failed"),
                                                           tr("Can not add file:<b>%1</b>.\n Would you go on load remain files?")
                                                           .arg(file),
                                                           QMessageBox::Yes | QMessageBox::No,
                                                           QMessageBox::Yes))
                {
                    break;
                }
            }
            else if(err == 0)
            {
                nCount++;
            }
        }

        appendInfo(tr("End load image(s)(%1 items loaded.)").arg(nCount));

        lockSprites();

        updateSpriteTableBySpriteMgr();

        QRect rect;
        m_spriteMgr->getMinRect(rect);
        m_pgv->ensureVisible(rect);
    }
}

void MainWindow::removeImage()
{
    CSprite* sp = NULL;
    QList<long> ids;
    for(int i = 0; i < m_spriteMgr->items().count(); i++)
    {
        sp = (CSprite*)m_spriteMgr->items()[i];
        if(sp->isSelected())
        {
            ids<<sp->getID();
        }
    }

    foreach(long id, ids)
    {
        m_spriteMgr->removeSpriteByID(id);
    }

    m_spriteMgr->update();
    updateSpriteTableBySpriteMgr();
}

void MainWindow::generateDemo()
{
    QFileDialog fd;
    fd.setWindowTitle(tr("Select file to create"));
    fd.setParent(this);
    QStringList sl;
    sl<<tr("Html file (*.htm)");
    fd.setNameFilters(sl);

    if(QFileDialog::Accepted == fd.exec())
    {
        sl = fd.selectedFiles();
        QString strFile = sl[0];

        if(!strFile.endsWith(".htm"))
        {
            strFile += ".htm";
        }

        int n1 = strFile.lastIndexOf("\\");
        int n2 = strFile.lastIndexOf("/");
        int n = n1 > n2?n1:n2;

        QString strFolder = strFile.mid(0, n + 1);
        QString strFileName = strFile.mid(n + 1);

        QString strFileNameImage = strFileName.mid(0, strFileName.lastIndexOf(".")) + ".png";

        // write background image file
        const unsigned char srcData[52] = { 0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x5, 0x0, 0x5, 0x0, 0x91, 0x0, 0x0, 0x0, 0x0, 0x0,
             0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0, 0x0, 0x0, 0x21, 0xF9, 0x4, 0x1, 0x0, 0x0, 0x2,
             0x0, 0x2C, 0x0, 0x0, 0x0, 0x0, 0x5, 0x0, 0x5, 0x0, 0x0, 0x2, 0x5, 0x94, 0x8F, 0x6,
             0xC9, 0x52, 0x0, 0x3B};
        QFile file(QString("%1%2").arg(strFolder).arg("transparentBG.gif"));
        file.open(QFile::WriteOnly);
        file.write((const char *)srcData, sizeof(srcData));
        file.close();
        //
        do
        {
            if(QFile(strFolder + strFileNameImage).exists())
            {
                if(QMessageBox::No == QMessageBox::warning(this, tr("Image is existing"), tr("Image <b>%1</b> is existing, overwrite it?").arg(strFolder + strFileNameImage),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
                {
                    break;
                }
            }

            if(0 != createCssSpriteImage(strFolder + strFileNameImage))
            {
                QMessageBox::warning(this, tr("Write image file failed!"), tr("Write image file <b>%1</b> failed!").arg(strFolder + strFileNameImage));
            }
        }
        while(false);

        do
        {
            if(QFile(strFile).exists())
            {
                if(QMessageBox::No == QMessageBox::warning(this, tr("Html file is existing"), tr("Html file <b>%1</b> is existing, overwrite it?").arg(strFile),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
                {
                    break;
                }
            }

            if(0 != writeDemoHtml(strFile, strFileNameImage))
            {
                QMessageBox::warning(this, tr("Write html file failed!"), tr("Write html file <b>%1</b> failed!").arg(strFile));
            }
        }
        while(false);

    }
}

void MainWindow::arrangeHorizontal()
{
    updateSpriteFixedStatus();
    CArrangerCtrlDlg* dlg = new CArrangerCtrlDlg();
    dlg->setArrangeType(m_spriteMgr, 1);

    dlg->show();
}

void MainWindow::arrangeVertical()
{
    updateSpriteFixedStatus();
    CArrangerCtrlDlg* dlg = new CArrangerCtrlDlg();
    dlg->setArrangeType(m_spriteMgr, 2);

    dlg->show();
}

void MainWindow::arrangeAuto()
{
    updateSpriteFixedStatus();
    CArrangerCtrlDlg* dlg = new CArrangerCtrlDlg();
    dlg->setArrangeType(m_spriteMgr, 0);

    dlg->exec();

}

void MainWindow::aboutProgram()
{
    QFile fileLicense;
    fileLicense.setFileName(":/LICENSE");
    fileLicense.open(QFile::ReadOnly);

    QTextStream ts(&fileLicense);

    QMessageBox::information(this, STR_MY_TITLE, ts.readAll());

    fileLicense.close();
}

void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this, STR_MY_TITLE);
}

void MainWindow::switchLanguage(QAction *act)
{
    act->setChecked(true);
    m_settings->setValue(STR_MY_LANGUAGE, act->data());

    QString strTips = tr("LanguageChose");

    QTranslator translator;
    translator.load(QString(":/res/lang/njcs_%1.qm").arg(act->data().toString().toLower()));
    strTips = translator.translate("MainWindow", "LanguageChose");
    if(strTips.length() == 0)
    {
        strTips = "Your chose has been saved, please <b>Restart Program</b> to effect.";
    }

    QMessageBox::information(this, STR_MY_TITLE, strTips);
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

void MainWindow::closeProject()
{
    m_bProjectIsNull = true;
    m_bIsEditedWithoutSave = false;

    updateSpriteMgrAndView();

    updateWindowTitleAndMenuStatus();
}

int MainWindow::loadConfigFromXml(const QString& strXmlFilePath)
{
    int err = -1;
    QFile doc;

    doc.setFileName(strXmlFilePath);

    if(!doc.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, tr("Open config file failed"), tr("Open file:<b>%1</b> failed.").arg(strXmlFilePath));

        return err;
    }

    QXmlStreamReader sr(&doc);
    ProjectConfig cc;
    QString tagName;

    int nCount = 0;
    while(!sr.atEnd() && sr.error() == QXmlStreamReader::NoError )
    {
        sr.readNext();
        if(sr.isStartElement())
        {
            tagName = sr.name().toString().toLower();
            if(tagName == QString("docName").toLower())
            {
                cc.strDocName = sr.readElementText();
            }
            else if(tagName == QString("canvasWidth").toLower())
            {
                cc.nCanvasWidth = sr.readElementText().toInt();
            }
            else if(tagName == QString("canvasHeight").toLower())
            {
                cc.nCanvasHeight = sr.readElementText().toInt();
            }
            else if(tagName == QString("blockWidth").toLower())
            {
                cc.nBlockWidth = sr.readElementText().toInt();
            }
            else if(tagName == QString("blockHeight").toLower())
            {
                cc.nBlockHeight = sr.readElementText().toInt();
            }
            else if(tagName == QString("blockSpanHor").toLower())
            {
                cc.nBlockSpanHor = sr.readElementText().toInt();
            }
            else if(tagName == QString("blockSpanVer").toLower())
            {
                cc.nBlockSpanVer = sr.readElementText().toInt();
            }
            else if(tagName == QString("blockMarginLeft").toLower())
            {
                cc.nBlockMarginLeft = sr.readElementText().toInt();
            }
            else if(tagName == QString("blockMarginTop").toLower())
            {
                cc.nBlockMarginTop = sr.readElementText().toInt();
            }
            else if(tagName == QString("image").toLower())
            {
                QPoint point;
                point.setX(sr.attributes().value("left").toInt());
                point.setY(sr.attributes().value("top").toInt());

                QString strFile = sr.attributes().value("file").toString();
                if(QDir::isRelativePath(strFile))
                {
                    strFile = m_strWorkDirectory + strFile;
                }

                int err = m_spriteMgr->addSprite(strFile, point, true);
                appendInfoByAddSprite(strFile, err);

                if( err < 0)
                {
                    appendInfo(tr("Load image <b>%1</b> failed.").arg(strFile));
                    if(QMessageBox::Yes == QMessageBox::warning(this, tr("Add image failed"), tr("Add image:<b>%1</b> failed, do you want to terminate load?")
                                                                .arg(sr.attributes().value("file").toString()),
                                                                QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
                    {
                        break;
                    }
                }
                else if(err == 0)
                {
                    nCount++;
                }
            }
        }
    }

    appendInfo(tr("Loaded %1 items from config.").arg(nCount));

    lockSprites();
    m_projConfig = cc;
    m_bProjectIsNull = true;
    m_bIsEditedWithoutSave = false;
    updateWindowTitleAndMenuStatus();
    return 0;
}

int MainWindow::saveConfigToXml(const QString& strXmlFilePath, bool bCopyImg)
{
    int err = -1;

    QFile doc;

    doc.setFileName(strXmlFilePath);

    if(!doc.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, tr("Create file failed"), tr("Create file:<b>%1</b> failed.")
                                                                    .arg(strXmlFilePath));
        return err;
    }

    QXmlStreamWriter sw(&doc);
    sw.setAutoFormatting(true);
    sw.writeStartDocument();
    sw.writeStartElement("njCsSprite");
    sw.writeTextElement("docName", m_projConfig.strDocName);

    sw.writeStartElement("background");
    sw.writeTextElement("canvasWidth", QString("%1").arg(m_projConfig.nCanvasWidth));
    sw.writeTextElement("canvasHeight", QString("%1").arg(m_projConfig.nCanvasHeight));
    sw.writeTextElement("blockWidth", QString("%1").arg(m_projConfig.nBlockWidth));
    sw.writeTextElement("blockHeight", QString("%1").arg(m_projConfig.nBlockHeight));
    sw.writeTextElement("blockSpanHor", QString("%1").arg(m_projConfig.nBlockSpanHor));
    sw.writeTextElement("blockSpanVer", QString("%1").arg(m_projConfig.nBlockSpanVer));
    sw.writeTextElement("blockMarginLeft", QString("%1").arg(m_projConfig.nBlockMarginLeft));
    sw.writeTextElement("blockMarginTop", QString("%1").arg(m_projConfig.nBlockMarginTop));
    sw.writeEndElement();

    sw.writeStartElement("images");
    CSprite* s = NULL;
    QString strNew = "";
    for(int i = 0; i < m_spriteMgr->items().count(); i++)
    {
        s = (CSprite*)m_spriteMgr->items()[i];
        strNew = s->getFilePath();

        if(bCopyImg)
        {
            strNew = QString("%1.%2")
                    .arg(s->getCRC32(), 0, 16)
                    .arg(s->getFileExt());
            if(s->getFilePath() != m_strWorkDirectory + strNew &&
                    !QFile::copy(s->getFilePath(), m_strWorkDirectory + strNew))
            {
                if(QMessageBox::Yes == QMessageBox::warning(this, tr("Copy file failed"), tr("Copy image:<b>%1</b> failed, do you want to terminate save?")
                                                            .arg(s->getFilePath()),
                                                            QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
                {
                    break;
                }
            }
        }

        sw.writeStartElement("image");
        sw.writeAttribute("top", QString("%1").arg(s->y()));
        sw.writeAttribute("left", QString("%1").arg(s->x()));
        sw.writeAttribute("file", strNew);
        if(bCopyImg)
        {
            sw.writeAttribute("origin", s->getFilePath());
        }
        sw.writeEndElement();
    }

    sw.writeEndElement();
    sw.writeEndDocument();

    m_bIsEditedWithoutSave = false;

    err = 0;
    return err;
}

