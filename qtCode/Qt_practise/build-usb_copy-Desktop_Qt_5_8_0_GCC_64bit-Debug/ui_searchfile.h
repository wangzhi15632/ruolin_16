/********************************************************************************
** Form generated from reading UI file 'searchfile.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCHFILE_H
#define UI_SEARCHFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SearchFile
{
public:
    QMenuBar *menubar;
    QWidget *centralwidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *SearchFile)
    {
        if (SearchFile->objectName().isEmpty())
            SearchFile->setObjectName(QStringLiteral("SearchFile"));
        SearchFile->resize(800, 600);
        menubar = new QMenuBar(SearchFile);
        menubar->setObjectName(QStringLiteral("menubar"));
        SearchFile->setMenuBar(menubar);
        centralwidget = new QWidget(SearchFile);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        SearchFile->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(SearchFile);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        SearchFile->setStatusBar(statusbar);

        retranslateUi(SearchFile);

        QMetaObject::connectSlotsByName(SearchFile);
    } // setupUi

    void retranslateUi(QMainWindow *SearchFile)
    {
        SearchFile->setWindowTitle(QApplication::translate("SearchFile", "MainWindow", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SearchFile: public Ui_SearchFile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHFILE_H
