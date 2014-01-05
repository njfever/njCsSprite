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
        //Menu 'File'
        NewFile,        // create a new css sprite project
        OpenFile,       // open an existing project
        SaveFile,       // save modified project
        SaveAs,         // save as alias
        CloseFile,      // close current project
        Quit,           // quit the program
        //Menu 'Edit'
        Option,         // set background and size parameters
        ImageManager,      // show interface of image-manager
        AddImage,       // add an image to current project
        RemoveImage,    // remove as image from current project
        GenerateDemo,   // output test file(img, css & html)
        //Menu 'Range'
        Ramdom,
        Horizontal,
        Vertical,
        MinArea,        // apply algorithm to range rectangles to make them close together
        //Menu 'Tool'
        AnalyseImage,
        ManualOperate,
        AutoSplit
    };
};


#endif // DATASTRUCT_H
