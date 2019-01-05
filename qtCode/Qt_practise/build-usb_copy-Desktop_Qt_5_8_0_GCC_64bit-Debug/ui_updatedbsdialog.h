/********************************************************************************
** Form generated from reading UI file 'updatedbsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UPDATEDBSDIALOG_H
#define UI_UPDATEDBSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_UpdateDbsDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *updateStat;
    QLabel *locateDbRes;
    QLabel *searchDbRes;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelBtn;
    QPushButton *unlockBtn;

    void setupUi(QDialog *UpdateDbsDialog)
    {
        if (UpdateDbsDialog->objectName().isEmpty())
            UpdateDbsDialog->setObjectName(QStringLiteral("UpdateDbsDialog"));
        UpdateDbsDialog->resize(480, 201);
        UpdateDbsDialog->setStyleSheet(QStringLiteral("font:   14px"));
        verticalLayout_3 = new QVBoxLayout(UpdateDbsDialog);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(UpdateDbsDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(0, 30));
        label->setStyleSheet(QStringLiteral("font: normal bold 16px;"));

        verticalLayout->addWidget(label);

        label_2 = new QLabel(UpdateDbsDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(0, 30));
        label_2->setWordWrap(true);

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(UpdateDbsDialog);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout->addWidget(label_3);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 1);

        verticalLayout_3->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        updateStat = new QLabel(UpdateDbsDialog);
        updateStat->setObjectName(QStringLiteral("updateStat"));

        verticalLayout_2->addWidget(updateStat);

        locateDbRes = new QLabel(UpdateDbsDialog);
        locateDbRes->setObjectName(QStringLiteral("locateDbRes"));

        verticalLayout_2->addWidget(locateDbRes);

        searchDbRes = new QLabel(UpdateDbsDialog);
        searchDbRes->setObjectName(QStringLiteral("searchDbRes"));

        verticalLayout_2->addWidget(searchDbRes);


        verticalLayout_3->addLayout(verticalLayout_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        cancelBtn = new QPushButton(UpdateDbsDialog);
        cancelBtn->setObjectName(QStringLiteral("cancelBtn"));

        horizontalLayout_2->addWidget(cancelBtn);

        unlockBtn = new QPushButton(UpdateDbsDialog);
        unlockBtn->setObjectName(QStringLiteral("unlockBtn"));

        horizontalLayout_2->addWidget(unlockBtn);


        verticalLayout_3->addLayout(horizontalLayout_2);

        verticalLayout_3->setStretch(1, 1);

        retranslateUi(UpdateDbsDialog);

        QMetaObject::connectSlotsByName(UpdateDbsDialog);
    } // setupUi

    void retranslateUi(QDialog *UpdateDbsDialog)
    {
        UpdateDbsDialog->setWindowTitle(QApplication::translate("UpdateDbsDialog", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("UpdateDbsDialog", "更新本地数据和搜索数据", Q_NULLPTR));
        label_2->setText(QApplication::translate("UpdateDbsDialog", "为了提供更精确的搜索，请进行本地数据和搜索数据更新.", Q_NULLPTR));
        label_3->setText(QApplication::translate("UpdateDbsDialog", "这需要管理员(admin) 权限.", Q_NULLPTR));
        updateStat->setText(QApplication::translate("UpdateDbsDialog", "状态", Q_NULLPTR));
        locateDbRes->setText(QApplication::translate("UpdateDbsDialog", "本地更新", Q_NULLPTR));
        searchDbRes->setText(QApplication::translate("UpdateDbsDialog", "搜索更新", Q_NULLPTR));
        cancelBtn->setText(QApplication::translate("UpdateDbsDialog", "取消", Q_NULLPTR));
        unlockBtn->setText(QApplication::translate("UpdateDbsDialog", "更新", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class UpdateDbsDialog: public Ui_UpdateDbsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPDATEDBSDIALOG_H
