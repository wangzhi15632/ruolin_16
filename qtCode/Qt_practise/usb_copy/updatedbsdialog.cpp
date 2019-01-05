#include "updatedbsdialog.h"
#include "ui_updatedbsdialog.h"
#include "sudodialog.h"
#include <QDebug>
#include <QProcess>
#include <QThread>

UpdateDbsDialog::UpdateDbsDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint), m_passwardValid(false), m_pTaskMgr(NULL)
{
    setupUi(this);
    updateStat->hide();
    locateDbRes->hide();
    searchDbRes->hide();
}

UpdateDbsDialog::~UpdateDbsDialog()
{
    qDebug() << "delete updatedb";
    if (m_pTaskMgr != NULL)
    {
        qDebug() << "delete mgr" << m_pTaskMgr;
        delete m_pTaskMgr;
    }
}

void UpdateDbsDialog::on_cancelBtn_clicked()
{
    close();
}


void UpdateDbsDialog::on_unlockBtn_clicked()
{
    const QString password = SudoDialog::getValidPassword(this);
    if (password.isEmpty())
    {
        updateStat->setText(tr("认证失败."));
        updateStat->show();
        return;
    }

    updateStat->setText(tr("更新文件..."));
    updateStat->show();
    cancelBtn->setDisabled(true);
    unlockBtn->setDisabled(true);

    m_pTaskMgr = TaskManager::getTaskManager(this);
    QObject::connect(m_pTaskMgr, SIGNAL(taskFinished(int,bool)), this, SLOT(taskFinish(int,bool)));
    m_pTaskMgr->setPassward(password);
    m_pTaskMgr->start();
}

void UpdateDbsDialog::onProcFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "on finish: " << exitCode << exitStatus;
}

void UpdateDbsDialog::taskFinish(int taskid, bool success)
{
    switch(taskid)
    {
    case TASKID_UPDATE_LOCATEDB:
        if (success)
        {
            locateDbRes->setText(tr("本地数据更新成功."));
        }
        else
        {
            locateDbRes->setText(tr("本地数据更新失败."));
        }
        locateDbRes->show();
        break;

    case TASKID_UPDATE_SEARCHDB:
        if (success)
        {
            searchDbRes->setText(tr("搜索数据更新成功."));
        }
        else
        {
            searchDbRes->setText(tr("搜索数据更新失败."));
        }
        searchDbRes->show();
        unlockBtn->hide();
        cancelBtn->setText(tr("关闭"));
        cancelBtn->setEnabled(true);
        updateStat->setDisabled(true);
        emit databaseUpdated();
        break;

    default:
        break;
    }
}
