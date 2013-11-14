#ifndef DATASTRUCT_H
#define DATASTRUCT_H
//
// include head files
//
#include <QGraphicsView>
#include <QMap>
#include <QAction>
#include <QMessageBox>

class ActionKey
{
public:
    enum enumActionKey
    {
        NewFile,
        OpenFile,
        SaveFile,
        SaveAs,
        CloseFile,
        Quit
    };
};


#endif // DATASTRUCT_H
