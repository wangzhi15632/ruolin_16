#ifndef SEARCHFILE_H
#define SEARCHFILE_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMenuBar>
#include <QStatusBar>
#include <QApplication>
#include <QSqlQueryModel>
#include "csqlquerymodel.h"
#include "ctableview.h"
#include <QSqlQuery>

#include <QMessageBox>
#include <QSqlError>
#include <QPoint>

const QString strSelectSQL = "SELECT name, path, type FROM everything";
const QString strWhereGlobWildcard = " WHERE name GLOB '*%1*'"; /* match case */
const QString strWhereLikeWildcard = " WHERE name LIKE '%%1%'";
const QString strWhereGlob = " WHERE name GLOB '%1'"; /* match case */
const QString strWhereLike = " WHERE name LIKE '%1'";
const QString strOrderByNm = " ORDER BY name";
const QString strCountSelectSQL = "SELECT COUNT(*) FROM everything";
const QString strDbName = "/var/lib/everything/everything.db";
//const QString strDbName = "everything.db";

class SearchFile : public QMainWindow
{
    Q_OBJECT

public:
    explicit SearchFile(QWidget *parent = 0);
    ~SearchFile();

signals:

public slots:
    void on_keywordEdit_textChanged();
    void reloadModel();
    void on_tableView_doubleClicked(const QModelIndex & index);
    void showContextMenu(const QPoint &pos);
    void openFile();
    void openFilePath();
    void copyFullPath();

private:
    void setupUi();
    void retranslateUi();
    bool initTable();
    bool connectDb();
    bool reConnectDb();
    bool loadSettings(bool loadDefault = false);
    void setFilter(const QString& text);
    void setTitle(const QString& text);

public:
    CSqlQueryModel *m_sourceModel;
    QSqlQuery *m_sqlQuery;
    QSqlDatabase m_db;

    /* settings */
    bool m_enableMatchCase;
    bool m_enableRegex;

    /* GUI widgets */
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *keywordEdit;
    CTableView *tableView;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

private:
    int m_showContextRow;
};

#endif // MAINWINDOW_H

