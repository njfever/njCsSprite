/* 
  Copyright (C) 2013-2014 njfever@163.com
  version 0.1, March 30th, 2014

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef DATASTRUCT_H
#define DATASTRUCT_H
//
// include files
//
#include <QGraphicsView>
#include <QMap>
#include <QAction>
#include <QMessageBox>
#include <QString>

const QString STR_MY_TITLE = "njCsSprite";
const QString STR_MY_PROJ_XML_FILE_NAME = "njCsSprite.xml";
const QString STR_MY_LANGUAGE = "language";

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
        ProjectSetup,
        Quit,           // quit the program
        //Menu 'Sprite'
        SpriteNoneFixed,
        SpriteAllFixed,
        SpriteSetFixed,
        SpriteLock,         // set background and size parameters
        SpriteManager,      // show interface of image-manager
        ArrangeOption,
        AddImage,       // add an image to current project
        RemoveImage,    // remove as image from current project
        GenerateDemo,   // output test file(img, css & html)
        //Menu 'Arrange'
        ArrangeHorizontal,
        ArrangeVertical,
        ArrangeAuto,        // apply algorithm to Arrange rectangles to make them close together
        // Menu 'Help'
        HelpLang,      // select lanuage
        HelpAboutApp,
        HelpAboutQt,
        //Menu 'Tool'
        AnalyseImage,
        ManualOperate,
        AutoSplit
    };
};

class ArrangerStatusCode
{
public:
    enum enumArrangerStatusCode
    {
        INITED, // initialized
        BEGIN,  // begin arrange
        ARRANGING,  // doing arrange
        END,    // end of arrange
        ERR,  // occasioned error
        INFO    // output some information
    };
};


Q_DECLARE_METATYPE(ArrangerStatusCode::enumArrangerStatusCode)

typedef struct tagCanvasConfig
{
    QString strDocName;
    long nCanvasWidth;
    long nCanvasHeight;

    long nBlockWidth;  // in pixel
    long nBlockHeight; // in pixel
    long nBlockSpanHor;// in how much blocks
    long nBlockSpanVer;// in how much blocks
    long nBlockMarginLeft; // in pixel
    long nBlockMarginTop;  // in pixel

    tagCanvasConfig()
    {
        strDocName = QString("unnamed");
        nCanvasWidth = 800;
        nCanvasHeight = 1000;

        nBlockWidth = 10;  // in pixel
        nBlockHeight = 10; // in pixel
        nBlockSpanHor = 0;// in how much blocks
        nBlockSpanVer = 0;// in how much blocks
        nBlockMarginTop = 2;  // in pixel
        nBlockMarginLeft = 2; // in pixel
    }
}ProjectConfig, *PCanvasConfig;

typedef struct tagMyRect
{
    long nTop;
    long nLeft;
    long nBottom;
    long nRight;

    tagMyRect()
    {
        nTop = nLeft = nBottom = nRight = 0;
    }

    tagMyRect(long x, long y, long width, long height)
    {
        nTop = y;
        nLeft = x;
        nBottom = y + height;
        nRight = x + width;
    }
}MyRect,*PMyRect;

#endif // DATASTRUCT_H
