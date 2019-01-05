/********************************************************************************
** Form generated from reading UI file 'sudodialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUDODIALOG_H
#define UI_SUDODIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SudoDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *incorrectLabel;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *pwdLineEdit;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelBtn;
    QPushButton *okBtn;

    void setupUi(QDialog *SudoDialog)
    {
        if (SudoDialog->objectName().isEmpty())
            SudoDialog->setObjectName(QStringLiteral("SudoDialog"));
        SudoDialog->resize(300, 210);
        SudoDialog->setStyleSheet(QStringLiteral("font: 14px"));
        verticalLayout = new QVBoxLayout(SudoDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_2 = new QLabel(SudoDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setStyleSheet(QStringLiteral("font: bold 16px;"));
        label_2->setWordWrap(true);

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(SudoDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setWordWrap(true);

        verticalLayout->addWidget(label_3);

        incorrectLabel = new QLabel(SudoDialog);
        incorrectLabel->setObjectName(QStringLiteral("incorrectLabel"));
        incorrectLabel->setStyleSheet(QStringLiteral("font: bold;"));
        incorrectLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(incorrectLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(SudoDialog);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        pwdLineEdit = new QLineEdit(SudoDialog);
        pwdLineEdit->setObjectName(QStringLiteral("pwdLineEdit"));
        pwdLineEdit->setEchoMode(QLineEdit::Password);
        pwdLineEdit->setDragEnabled(true);

        horizontalLayout->addWidget(pwdLineEdit);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        cancelBtn = new QPushButton(SudoDialog);
        cancelBtn->setObjectName(QStringLiteral("cancelBtn"));

        horizontalLayout_2->addWidget(cancelBtn);

        okBtn = new QPushButton(SudoDialog);
        okBtn->setObjectName(QStringLiteral("okBtn"));

        horizontalLayout_2->addWidget(okBtn);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(SudoDialog);

        okBtn->setDefault(true);


        QMetaObject::connectSlotsByName(SudoDialog);
    } // setupUi

    void retranslateUi(QDialog *SudoDialog)
    {
        SudoDialog->setWindowTitle(QApplication::translate("SudoDialog", "Dialog", Q_NULLPTR));
        label_2->setText(QApplication::translate("SudoDialog", "输入密码来执行管理员操作.", Q_NULLPTR));
        label_3->setText(QApplication::translate("SudoDialog", "这个程序帮你高效的搜索文件.", Q_NULLPTR));
        incorrectLabel->setText(QApplication::translate("SudoDialog", "密码错误... 请重新输入.", Q_NULLPTR));
        label->setText(QApplication::translate("SudoDialog", "密码 : ", Q_NULLPTR));
        pwdLineEdit->setText(QString());
        cancelBtn->setText(QApplication::translate("SudoDialog", "取消", Q_NULLPTR));
        okBtn->setText(QApplication::translate("SudoDialog", "确认", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SudoDialog: public Ui_SudoDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUDODIALOG_H
