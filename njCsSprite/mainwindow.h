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
    void newFile();
};

#endif // MAINWINDOW_H
