/********************************************************************************
** Form generated from reading UI file 'QtWidgetsApplication7.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTWIDGETSAPPLICATION7_H
#define UI_QTWIDGETSAPPLICATION7_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "MySlider.h"
#include "VideoWidget.h"

QT_BEGIN_NAMESPACE

class Ui_FFVideoPlayerClass
{
public:
    QAction *action_OpenLocalFiles;
    QAction *action_OpenNetStream;
    QAction *action_OpenCamera;
    QAction *action_About;
    QAction *action_GetSourceCode;
    QWidget *centralWidget;
    VideoWidget *openGLWidget;
    QLabel *label_Playtime;
    MySlider *timerSlider;
    QSlider *sliderVolume;
    QPushButton *btnPlayVideo;
    QPushButton *btnCutImage;
    QPushButton *btnSetVolume;
    QPushButton *btnFullScreen;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menushezhi;
    QMenu *menu_Help;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FFVideoPlayerClass)
    {
        if (FFVideoPlayerClass->objectName().isEmpty())
            FFVideoPlayerClass->setObjectName(QString::fromUtf8("FFVideoPlayerClass"));
        FFVideoPlayerClass->resize(720, 652);
        action_OpenLocalFiles = new QAction(FFVideoPlayerClass);
        action_OpenLocalFiles->setObjectName(QString::fromUtf8("action_OpenLocalFiles"));
        action_OpenNetStream = new QAction(FFVideoPlayerClass);
        action_OpenNetStream->setObjectName(QString::fromUtf8("action_OpenNetStream"));
        action_OpenCamera = new QAction(FFVideoPlayerClass);
        action_OpenCamera->setObjectName(QString::fromUtf8("action_OpenCamera"));
        action_About = new QAction(FFVideoPlayerClass);
        action_About->setObjectName(QString::fromUtf8("action_About"));
        action_GetSourceCode = new QAction(FFVideoPlayerClass);
        action_GetSourceCode->setObjectName(QString::fromUtf8("action_GetSourceCode"));
        centralWidget = new QWidget(FFVideoPlayerClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        openGLWidget = new VideoWidget(centralWidget);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
        openGLWidget->setGeometry(QRect(40, 30, 611, 351));
        openGLWidget->setStyleSheet(QString::fromUtf8(""));
        label_Playtime = new QLabel(centralWidget);
        label_Playtime->setObjectName(QString::fromUtf8("label_Playtime"));
        label_Playtime->setGeometry(QRect(40, 530, 331, 16));
        label_Playtime->setStyleSheet(QString::fromUtf8("QLabel\n"
"{\n"
"	font-family: \"Microsoft YaHei\";\n"
"    font-size: 18px;\n"
"    color: white;\n"
"	background-color: rgb(53,53,53);\n"
"}"));
        timerSlider = new MySlider(centralWidget);
        timerSlider->setObjectName(QString::fromUtf8("timerSlider"));
        timerSlider->setGeometry(QRect(40, 510, 641, 16));
        timerSlider->setOrientation(Qt::Horizontal);
        sliderVolume = new QSlider(centralWidget);
        sliderVolume->setObjectName(QString::fromUtf8("sliderVolume"));
        sliderVolume->setGeometry(QRect(540, 530, 81, 16));
        sliderVolume->setOrientation(Qt::Horizontal);
        btnPlayVideo = new QPushButton(centralWidget);
        btnPlayVideo->setObjectName(QString::fromUtf8("btnPlayVideo"));
        btnPlayVideo->setGeometry(QRect(380, 520, 41, 31));
        btnCutImage = new QPushButton(centralWidget);
        btnCutImage->setObjectName(QString::fromUtf8("btnCutImage"));
        btnCutImage->setGeometry(QRect(430, 520, 30, 30));
        btnSetVolume = new QPushButton(centralWidget);
        btnSetVolume->setObjectName(QString::fromUtf8("btnSetVolume"));
        btnSetVolume->setGeometry(QRect(510, 520, 30, 30));
        btnFullScreen = new QPushButton(centralWidget);
        btnFullScreen->setObjectName(QString::fromUtf8("btnFullScreen"));
        btnFullScreen->setGeometry(QRect(640, 520, 30, 30));
        FFVideoPlayerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(FFVideoPlayerClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 720, 37));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menushezhi = new QMenu(menuBar);
        menushezhi->setObjectName(QString::fromUtf8("menushezhi"));
        menu_Help = new QMenu(menuBar);
        menu_Help->setObjectName(QString::fromUtf8("menu_Help"));
        FFVideoPlayerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FFVideoPlayerClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        FFVideoPlayerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(FFVideoPlayerClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        FFVideoPlayerClass->setStatusBar(statusBar);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menushezhi->menuAction());
        menuBar->addAction(menu_Help->menuAction());
        menu_File->addAction(action_OpenLocalFiles);
        menu_File->addAction(action_OpenNetStream);
        menu_File->addAction(action_OpenCamera);
        menu_Help->addAction(action_About);
        menu_Help->addAction(action_GetSourceCode);

        retranslateUi(FFVideoPlayerClass);

        QMetaObject::connectSlotsByName(FFVideoPlayerClass);
    } // setupUi

    void retranslateUi(QMainWindow *FFVideoPlayerClass)
    {
        FFVideoPlayerClass->setWindowTitle(QApplication::translate("FFVideoPlayerClass", "FFVideoPlayer", nullptr));
        action_OpenLocalFiles->setText(QApplication::translate("FFVideoPlayerClass", "\346\211\223\345\274\200\346\234\254\345\234\260\350\247\206\351\242\221", nullptr));
        action_OpenNetStream->setText(QApplication::translate("FFVideoPlayerClass", "\346\211\223\345\274\200\347\275\221\347\273\234\346\265\201", nullptr));
        action_OpenCamera->setText(QApplication::translate("FFVideoPlayerClass", "\346\211\223\345\274\200\346\221\204\345\203\217\345\244\264", nullptr));
        action_About->setText(QApplication::translate("FFVideoPlayerClass", "\345\205\263\344\272\216\346\234\254\350\275\257\344\273\266", nullptr));
        action_GetSourceCode->setText(QApplication::translate("FFVideoPlayerClass", "\350\216\267\345\217\226\346\272\220\344\273\243\347\240\201", nullptr));
        label_Playtime->setText(QString());
        btnPlayVideo->setText(QString());
        btnCutImage->setText(QString());
        btnSetVolume->setText(QString());
        btnFullScreen->setText(QString());
        menu_File->setTitle(QApplication::translate("FFVideoPlayerClass", "\346\226\207\344\273\266", nullptr));
        menushezhi->setTitle(QApplication::translate("FFVideoPlayerClass", "\350\256\276\347\275\256", nullptr));
        menu_Help->setTitle(QApplication::translate("FFVideoPlayerClass", "\345\270\256\345\212\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FFVideoPlayerClass: public Ui_FFVideoPlayerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTWIDGETSAPPLICATION7_H
