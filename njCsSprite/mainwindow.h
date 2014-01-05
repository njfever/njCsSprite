#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dataDefine.h"

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
    QGraphicsView* m_pgv;
    QMap<ActionKey::enumActionKey, QAction*> m_actions;
    QMenu* m_menuFile;
    QMenu* m_menuHelp;
    QToolBar* m_toolbarFile;
    QToolBar* m_toolbarHelp;

    void createActions();
    void createMenus();
    void createToolbarButtons();

private slots:
    // menu 'File'
    void quitProgram();
    void newProject();
    void openProject();
    void saveProject();
    void saveProjectAs();

    // menu 'Edit'
    void option();
    void imageManager();
    void addImage();
    void removeImage();
    void generateDemo();

    //menu 'Range'
    void rangeRandom();
    void rangeHorizontal();
    void rangeVertical();
    void rangeMinArea();

    //menu 'Tool'
    void analyseImage();
    void manualOperate();
    void autoSplit();
};

#endif // MAINWINDOW_H
