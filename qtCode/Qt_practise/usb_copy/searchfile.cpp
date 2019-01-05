#include "searchfile.h"
#include "ui_searchfile.h"
#include <QDebug>
#include <QMessageBox>
#include <sudodialog.h>
#include <QClipboard>
#include <QSqlRecord>

SearchFile::SearchFile(QWidget *parent) :
    QMainWindow(parent), m_sourceModel()
{
    setupUi();
   // setWindowIcon(QIcon(QPixmap("://windowIcon.png")));
    m_sourceModel = new CSqlQueryModel;
    m_sqlQuery = new QSqlQuery;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(strDbName);
    if (connectDb())
    {
        loadSettings(1);
        initTable();
    }
}

SearchFile::~SearchFile()
{
    delete m_sqlQuery;
}

void SearchFile::setupUi()
{
    resize(698, 351);
    centralWidget = new QWidget(this);

    verticalLayout = new QVBoxLayout(centralWidget);
    verticalLayout->setSpacing(3);
    verticalLayout->setContentsMargins(1, 1, 1, 1);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(3);

    keywordEdit = new QLineEdit(centralWidget);
    keywordEdit->setObjectName(QString::fromUtf8("keywordEdit"));

    horizontalLayout->addWidget(keywordEdit);

    verticalLayout->addLayout(horizontalLayout);

    tableView = new CTableView(centralWidget);
    tableView->setObjectName(QString::fromUtf8("tableView"));

    verticalLayout->addWidget(tableView);

    setCentralWidget(centralWidget);

    menuBar = new QMenuBar(this);
    menuBar->setGeometry(QRect(0, 0, 700, 25));
    setMenuBar(menuBar);

    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    /* set title and texts */
    retranslateUi();

    /* connect slots by OBJECT name */
    QMetaObject::connectSlotsByName(this);
}

void SearchFile::retranslateUi()
{
   // setWindowTitle(QApplication::translate("MainWindow", "Everything", 0, QApplication::UnicodeUTF8));
}

bool SearchFile::reConnectDb()
{
    if (!m_db.isOpen())
    {
        m_db.close();
    }

    connectDb();

    return true;
}

bool SearchFile::connectDb()
{
    if (!m_db.open())
    {
        QMessageBox::warning(0, "Connect Database Error", m_db.lastError().text());
        return false;
    }
    return true;
}

bool SearchFile::initTable()
{
    m_sourceModel->setQuery(strSelectSQL + strOrderByNm);
    m_sourceModel->setHeaderData(0, Qt::Horizontal, "Name");
    m_sourceModel->setHeaderData(1, Qt::Horizontal, "Path");
    m_sourceModel->setHeaderData(2, Qt::Horizontal, "Size");
    m_sourceModel->setHeaderData(3, Qt::Horizontal, "Data Modified");

    tableView->setModel(m_sourceModel);
    tableView->setColumnWidth(0, 208);
    tableView->setColumnWidth(1, 270);
    tableView->setColumnWidth(2, 70);
    tableView->setColumnWidth(3, 130);
    tableView->setStyleSheet("QTableView{border-style: none}"
            "QTableView::item:selected{background: rgb(51,153,255)}");

    connect(tableView, SIGNAL(hoverRowChanged(int)), m_sourceModel, SLOT(setHoverRow(int)));

    /* create right button menu */
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    return true;
}

void SearchFile::on_tableView_doubleClicked(const QModelIndex & index)
{
    m_showContextRow = index.row();
    openFile();
}

void SearchFile::openFile()
{
    QString cmd = "xdg-open " + tableView->model()->index(m_showContextRow, 1).data().toString() + "/"
        + tableView->model()->index(m_showContextRow, 0).data().toString();
    if (system(cmd.toLocal8Bit().data()) != 0)
    {
        QMessageBox::warning(this, "Error opening", "No application is registered as handling this file");
    }
}

void SearchFile::openFilePath()
{
    QString cmd = "xdg-open " + tableView->model()->index(m_showContextRow, 1).data().toString();
    system(cmd.toLocal8Bit().data());
}

void SearchFile::copyFullPath()
{
    QClipboard *board = QApplication::clipboard();
    QString path = tableView->model()->index(m_showContextRow, 1).data().toString();
    QString name = tableView->model()->index(m_showContextRow, 0).data().toString();
    if (path == "/")
    {
        board->setText(path + name);
    }
    else
    {
        board->setText(path + "/" + name);
    }
}

void SearchFile::showContextMenu(const QPoint &pos)
{
    if (tableView->indexAt(pos).row() == -1)
    {
        return;
    }

    m_showContextRow = tableView->indexAt(pos).row();

    QMenu menu(this);
    QAction *open = menu.addAction("Open");
    QAction *openPath = menu.addAction("Open Path");
    QAction *copyFullPath = menu.addAction("Copy Full Name to Clipboard");

    connect(open, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(openPath, SIGNAL(triggered()), this, SLOT(openFilePath()));
    connect(copyFullPath, SIGNAL(triggered()), this, SLOT(copyFullPath()));
    menu.exec(QCursor::pos());
}

bool SearchFile::loadSettings(bool loadDefault)
{
    if (loadDefault)
    {
        m_enableMatchCase = false;
        m_enableRegex = false;
    }
    else
    {
        //load settings from ini file
    }
}

void SearchFile::setTitle(const QString& text)
{
    if (text.isEmpty())
    {
        setWindowTitle("Everything");
    }
    else
    {
        setWindowTitle(text + " - Everything");
    }
}

void SearchFile::setFilter(const QString& text)
{
    //    m_sourceModel->setQuery(strSelectSQL+" WHERE name LIKE '%" + text + "%' ORDER BY name");
    //    m_proxyModel->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive));
    if (text.contains('*'))
    {
        if (m_enableMatchCase)
        {
            m_sourceModel->setQuery(strSelectSQL + strWhereGlob.arg(text) + strOrderByNm);
        }
        else
        {
            QString keyword = text;
            m_sourceModel->setQuery(strSelectSQL + strWhereLike.arg(keyword.replace('*', '%')) + strOrderByNm);
        }
    }
    else
    {
        if (m_enableMatchCase)
        {
            m_sourceModel->setQuery(strSelectSQL + strWhereGlobWildcard.arg(text) + strOrderByNm);
        }
        else
        {
            m_sourceModel->setQuery(strSelectSQL + strWhereLikeWildcard.arg(text) + strOrderByNm);
        }
    }
}

void SearchFile::on_keywordEdit_textChanged()
{
    setTitle(keywordEdit->text());
    setFilter(keywordEdit->text());
}

void SearchFile::reloadModel()
{
    qDebug() << "reloadModel";
    reConnectDb();
    loadSettings(1);
    initTable();

    m_sourceModel->setQuery(strSelectSQL + strOrderByNm);
}
