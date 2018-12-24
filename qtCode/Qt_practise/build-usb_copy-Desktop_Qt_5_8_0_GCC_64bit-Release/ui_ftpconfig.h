/********************************************************************************
** Form generated from reading UI file 'ftpconfig.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FTPCONFIG_H
#define UI_FTPCONFIG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FtpConfig
{
public:
    QVBoxLayout *verticalLayout_3;

    void setupUi(QDialog *FtpConfig)
    {
        if (FtpConfig->objectName().isEmpty())
            FtpConfig->setObjectName(QStringLiteral("FtpConfig"));
        FtpConfig->resize(517, 494);
        verticalLayout_3 = new QVBoxLayout(FtpConfig);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));

        retranslateUi(FtpConfig);

        QMetaObject::connectSlotsByName(FtpConfig);
    } // setupUi

    void retranslateUi(QDialog *FtpConfig)
    {
        FtpConfig->setWindowTitle(QApplication::translate("FtpConfig", "Dialog", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FtpConfig: public Ui_FtpConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FTPCONFIG_H
