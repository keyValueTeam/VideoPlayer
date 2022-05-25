/********************************************************************************
** Form generated from reading UI file 'playTest.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYTEST_H
#define UI_PLAYTEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_playTestClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *playTestClass)
    {
        if (playTestClass->objectName().isEmpty())
            playTestClass->setObjectName(QString::fromUtf8("playTestClass"));
        playTestClass->resize(600, 400);
        menuBar = new QMenuBar(playTestClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        playTestClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(playTestClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        playTestClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(playTestClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        playTestClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(playTestClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        playTestClass->setStatusBar(statusBar);

        retranslateUi(playTestClass);

        QMetaObject::connectSlotsByName(playTestClass);
    } // setupUi

    void retranslateUi(QMainWindow *playTestClass)
    {
        playTestClass->setWindowTitle(QCoreApplication::translate("playTestClass", "playTest", nullptr));
    } // retranslateUi

};

namespace Ui {
    class playTestClass: public Ui_playTestClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYTEST_H
