/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QGroupBox *groupBox_2;
    QGroupBox *groupBox_3;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox_5;
    QGroupBox *groupBox_6;
    QGroupBox *groupBox_7;
    QGroupBox *groupBox_8;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBox_9;
    QGroupBox *groupBox_10;
    QGroupBox *groupBox_11;
    QGroupBox *groupBox_12;
    QHBoxLayout *horizontalLayout_4;
    QGroupBox *groupBox_13;
    QGroupBox *groupBox_14;
    QGroupBox *groupBox_15;
    QGroupBox *groupBox_16;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1139, 698);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_5 = new QHBoxLayout(centralWidget);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));

        horizontalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));

        horizontalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));

        horizontalLayout->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));

        horizontalLayout->addWidget(groupBox_4);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        groupBox_5 = new QGroupBox(centralWidget);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));

        horizontalLayout_2->addWidget(groupBox_5);

        groupBox_6 = new QGroupBox(centralWidget);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));

        horizontalLayout_2->addWidget(groupBox_6);

        groupBox_7 = new QGroupBox(centralWidget);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));

        horizontalLayout_2->addWidget(groupBox_7);

        groupBox_8 = new QGroupBox(centralWidget);
        groupBox_8->setObjectName(QStringLiteral("groupBox_8"));

        horizontalLayout_2->addWidget(groupBox_8);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        groupBox_9 = new QGroupBox(centralWidget);
        groupBox_9->setObjectName(QStringLiteral("groupBox_9"));

        horizontalLayout_3->addWidget(groupBox_9);

        groupBox_10 = new QGroupBox(centralWidget);
        groupBox_10->setObjectName(QStringLiteral("groupBox_10"));

        horizontalLayout_3->addWidget(groupBox_10);

        groupBox_11 = new QGroupBox(centralWidget);
        groupBox_11->setObjectName(QStringLiteral("groupBox_11"));

        horizontalLayout_3->addWidget(groupBox_11);

        groupBox_12 = new QGroupBox(centralWidget);
        groupBox_12->setObjectName(QStringLiteral("groupBox_12"));

        horizontalLayout_3->addWidget(groupBox_12);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        groupBox_13 = new QGroupBox(centralWidget);
        groupBox_13->setObjectName(QStringLiteral("groupBox_13"));

        horizontalLayout_4->addWidget(groupBox_13);

        groupBox_14 = new QGroupBox(centralWidget);
        groupBox_14->setObjectName(QStringLiteral("groupBox_14"));

        horizontalLayout_4->addWidget(groupBox_14);

        groupBox_15 = new QGroupBox(centralWidget);
        groupBox_15->setObjectName(QStringLiteral("groupBox_15"));

        horizontalLayout_4->addWidget(groupBox_15);

        groupBox_16 = new QGroupBox(centralWidget);
        groupBox_16->setObjectName(QStringLiteral("groupBox_16"));

        horizontalLayout_4->addWidget(groupBox_16);


        verticalLayout->addLayout(horizontalLayout_4);


        horizontalLayout_5->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1139, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "USB-1", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "USB-2", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "USB-3", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "USB-4", Q_NULLPTR));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "USB-5", Q_NULLPTR));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "USB-6", Q_NULLPTR));
        groupBox_7->setTitle(QApplication::translate("MainWindow", "USB-7", Q_NULLPTR));
        groupBox_8->setTitle(QApplication::translate("MainWindow", "USB-8", Q_NULLPTR));
        groupBox_9->setTitle(QApplication::translate("MainWindow", "USB-9", Q_NULLPTR));
        groupBox_10->setTitle(QApplication::translate("MainWindow", "USB-10", Q_NULLPTR));
        groupBox_11->setTitle(QApplication::translate("MainWindow", "USB-11", Q_NULLPTR));
        groupBox_12->setTitle(QApplication::translate("MainWindow", "USB-12", Q_NULLPTR));
        groupBox_13->setTitle(QApplication::translate("MainWindow", "USB-13", Q_NULLPTR));
        groupBox_14->setTitle(QApplication::translate("MainWindow", "USB-14", Q_NULLPTR));
        groupBox_15->setTitle(QApplication::translate("MainWindow", "USB-15", Q_NULLPTR));
        groupBox_16->setTitle(QApplication::translate("MainWindow", "USB-16", Q_NULLPTR));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
