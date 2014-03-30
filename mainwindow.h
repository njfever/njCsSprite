/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QFocusEvent>
#include <QtWidgets>
#include <QActionGroup>
#include <QSettings>

#include "dataDefine.h"
#include "cspritemgr.h"
#include "csetupprojectdlg.h"
#include "cmytableview.h"
#include "cmygraphicsview.h"
#include "carrangeoptiondlg.h"
#include "carrangerctrldlg.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    Ui::MainWindow *ui;
    QString m_strLanguage;
    CMyGraphicsView* m_pgv;
    CMyTableView* m_tableSprite;
    QTextEdit* m_txtInfo;

    QMap<ActionKey::enumActionKey, QAction*> m_actions;
    QActionGroup* m_actGrpLang;
    QSettings* m_settings;
    QMenu* m_menuFile;
    QToolBar* m_toolbarFile;
    QToolBar* m_toolbarHelp;
    QString m_strWorkDirectory;
    ProjectConfig m_projConfig;
    CSpriteMgr* m_spriteMgr;
    QSplitter* m_h1Splitter;
    QSplitter* m_vSplitter;

    bool m_bProjectIsNull;
    bool m_bIsEditedWithoutSave;
    bool m_bIsSceneMouseEvent;

    void initLanguage();
    void createActions();
    void createMenus();
    void createToolbarButtons();
    void createViews();
    void setDefaultSizeOfSplitter();
    void updateSpriteTableBySpriteMgr();

    void updateWindowTitleAndMenuStatus();
    void updateSpriteMgrAndView();
    void appendInfo(QString strInfo);
    void appendInfoByAddSprite(const QString& strFile, int nErr);
    void appendInfoBySelectSprite();
    void setInfo(QString strInfo);

    bool isEditedWithoutSave();
    void setEditedWithoutSave();

    void getSpriteCssStyle(const CSprite* sp, const QRect &rectBG, QRect &rectStyle) const;

    void updateSpriteFixedStatus();

    int writeDemoHtml(const QString& strFile, const QString& strBgImage);
    int createCssSpriteImage(const QString& strFile);

    int loadConfigFromXml(const QString& strXmlFilePath);
    int saveConfigToXml(const QString& strXmlFilePath, bool bCopyImg = true);

    //bool checkChoseDirectory(QString& strDir, bool bToCreate = true);
    void closeProject();
private slots:
    // menu 'File'
    void quitProgram();
    void newProject();
    void openProject();
    void saveProject();
    void saveProjectAs();
    void setupProject();

    void switchAllFixed();
    void switchNoneFixed();
    void switchFixed();
    void rowSelectionChanged();
    void spriteSelectionChanged();

    // menu 'Sprite'
    void lockSprites();
    void generateDemo();
    void addImage();
    void removeImage();
    void arrangeOption();

    //menu 'Arrange'
    void arrangeHorizontal();
    void arrangeVertical();
    void arrangeAuto();

    // menu 'Help'
    void aboutProgram();
    void aboutQt();
    void switchLanguage(QAction* act);
    //menu 'Tool'
    void analyseImage();
    void manualOperate();
    void autoSplit();

signals:
    void clearSpriteTableSelection();
};

#endif // MAINWINDOW_H
