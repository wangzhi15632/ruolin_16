/****************************************************************************
** Meta object code from reading C++ file 'searchfile.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../usb_copy/searchfile.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'searchfile.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SearchFile_t {
    QByteArrayData data[11];
    char stringdata0[139];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SearchFile_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SearchFile_t qt_meta_stringdata_SearchFile = {
    {
QT_MOC_LITERAL(0, 0, 10), // "SearchFile"
QT_MOC_LITERAL(1, 11, 26), // "on_keywordEdit_textChanged"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 11), // "reloadModel"
QT_MOC_LITERAL(4, 51, 26), // "on_tableView_doubleClicked"
QT_MOC_LITERAL(5, 78, 5), // "index"
QT_MOC_LITERAL(6, 84, 15), // "showContextMenu"
QT_MOC_LITERAL(7, 100, 3), // "pos"
QT_MOC_LITERAL(8, 104, 8), // "openFile"
QT_MOC_LITERAL(9, 113, 12), // "openFilePath"
QT_MOC_LITERAL(10, 126, 12) // "copyFullPath"

    },
    "SearchFile\0on_keywordEdit_textChanged\0"
    "\0reloadModel\0on_tableView_doubleClicked\0"
    "index\0showContextMenu\0pos\0openFile\0"
    "openFilePath\0copyFullPath"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SearchFile[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a /* Public */,
       3,    0,   50,    2, 0x0a /* Public */,
       4,    1,   51,    2, 0x0a /* Public */,
       6,    1,   54,    2, 0x0a /* Public */,
       8,    0,   57,    2, 0x0a /* Public */,
       9,    0,   58,    2, 0x0a /* Public */,
      10,    0,   59,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,    5,
    QMetaType::Void, QMetaType::QPoint,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SearchFile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SearchFile *_t = static_cast<SearchFile *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_keywordEdit_textChanged(); break;
        case 1: _t->reloadModel(); break;
        case 2: _t->on_tableView_doubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 3: _t->showContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 4: _t->openFile(); break;
        case 5: _t->openFilePath(); break;
        case 6: _t->copyFullPath(); break;
        default: ;
        }
    }
}

const QMetaObject SearchFile::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_SearchFile.data,
      qt_meta_data_SearchFile,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SearchFile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SearchFile::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SearchFile.stringdata0))
        return static_cast<void*>(const_cast< SearchFile*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int SearchFile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
