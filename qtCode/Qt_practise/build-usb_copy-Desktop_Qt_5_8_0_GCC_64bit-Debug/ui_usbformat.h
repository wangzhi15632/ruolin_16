/********************************************************************************
** Form generated from reading UI file 'usbformat.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USBFORMAT_H
#define UI_USBFORMAT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_usbFormat
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QCheckBox *checkBox_yes;
    QCheckBox *checkBox_no;
    QDialogButtonBox *buttonBox;
    QButtonGroup *buttonGroup;

    void setupUi(QDialog *usbFormat)
    {
        if (usbFormat->objectName().isEmpty())
            usbFormat->setObjectName(QStringLiteral("usbFormat"));
        usbFormat->resize(342, 114);
        verticalLayout = new QVBoxLayout(usbFormat);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(usbFormat);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        checkBox_yes = new QCheckBox(usbFormat);
        buttonGroup = new QButtonGroup(usbFormat);
        buttonGroup->setObjectName(QStringLiteral("buttonGroup"));
        buttonGroup->addButton(checkBox_yes);
        checkBox_yes->setObjectName(QStringLiteral("checkBox_yes"));
        checkBox_yes->setChecked(true);

        horizontalLayout->addWidget(checkBox_yes);

        checkBox_no = new QCheckBox(usbFormat);
        buttonGroup->addButton(checkBox_no);
        checkBox_no->setObjectName(QStringLiteral("checkBox_no"));

        horizontalLayout->addWidget(checkBox_no);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(usbFormat);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(usbFormat);
        QObject::connect(buttonBox, SIGNAL(accepted()), usbFormat, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), usbFormat, SLOT(reject()));

        QMetaObject::connectSlotsByName(usbFormat);
    } // setupUi

    void retranslateUi(QDialog *usbFormat)
    {
        usbFormat->setWindowTitle(QApplication::translate("usbFormat", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("usbFormat", "\345\244\215\345\210\266\345\256\214\346\210\220\345\220\216\346\230\257\345\220\246\346\240\274\345\274\217\345\214\226SD\345\215\241\357\274\232", Q_NULLPTR));
        checkBox_yes->setText(QApplication::translate("usbFormat", "\346\230\257", Q_NULLPTR));
        checkBox_no->setText(QApplication::translate("usbFormat", "\345\220\246", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class usbFormat: public Ui_usbFormat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USBFORMAT_H
