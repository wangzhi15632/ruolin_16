/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../usb_copy/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[21];
    char stringdata0[249];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 19), // "starFtpTransmission"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 8), // "slotShow"
QT_MOC_LITERAL(4, 41, 19), // "slotShowTranscoding"
QT_MOC_LITERAL(5, 61, 12), // "slotProgress"
QT_MOC_LITERAL(6, 74, 5), // "sum_t"
QT_MOC_LITERAL(7, 80, 8), // "copied_t"
QT_MOC_LITERAL(8, 89, 6), // "time_t"
QT_MOC_LITERAL(9, 96, 12), // "slotCloseDev"
QT_MOC_LITERAL(10, 109, 3), // "num"
QT_MOC_LITERAL(11, 113, 11), // "slotFindDev"
QT_MOC_LITERAL(12, 125, 5), // "char*"
QT_MOC_LITERAL(13, 131, 10), // "mountPoint"
QT_MOC_LITERAL(14, 142, 16), // "showLocalStorage"
QT_MOC_LITERAL(15, 159, 20), // "emitToFtpTranslation"
QT_MOC_LITERAL(16, 180, 11), // "starFtpTime"
QT_MOC_LITERAL(17, 192, 17), // "updateFtpProgress"
QT_MOC_LITERAL(18, 210, 16), // "ftpCfgBtnClicked"
QT_MOC_LITERAL(19, 227, 16), // "usbFmtActClicked"
QT_MOC_LITERAL(20, 244, 4) // "test"

    },
    "MainWindow\0starFtpTransmission\0\0"
    "slotShow\0slotShowTranscoding\0slotProgress\0"
    "sum_t\0copied_t\0time_t\0slotCloseDev\0"
    "num\0slotFindDev\0char*\0mountPoint\0"
    "showLocalStorage\0emitToFtpTranslation\0"
    "starFtpTime\0updateFtpProgress\0"
    "ftpCfgBtnClicked\0usbFmtActClicked\0"
    "test"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    4,   80,    2, 0x08 /* Private */,
       4,    2,   89,    2, 0x08 /* Private */,
       5,    5,   94,    2, 0x08 /* Private */,
       9,    1,  105,    2, 0x08 /* Private */,
      11,    1,  108,    2, 0x08 /* Private */,
      14,    0,  111,    2, 0x08 /* Private */,
      15,    0,  112,    2, 0x08 /* Private */,
      16,    0,  113,    2, 0x08 /* Private */,
      17,    4,  114,    2, 0x08 /* Private */,
      18,    0,  123,    2, 0x08 /* Private */,
      19,    0,  124,    2, 0x08 /* Private */,
      20,    0,  125,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::ULong, QMetaType::ULong, QMetaType::ULong,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::ULongLong,    2,    2,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 6, 0x80000000 | 7, 0x80000000 | 8, QMetaType::Bool,    2,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 6, 0x80000000 | 7, 0x80000000 | 8,    2,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->starFtpTransmission(); break;
        case 1: _t->slotShow((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< ulong(*)>(_a[2])),(*reinterpret_cast< ulong(*)>(_a[3])),(*reinterpret_cast< ulong(*)>(_a[4]))); break;
        case 2: _t->slotShowTranscoding((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< unsigned long long(*)>(_a[2]))); break;
        case 3: _t->slotProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< sum_t(*)>(_a[2])),(*reinterpret_cast< copied_t(*)>(_a[3])),(*reinterpret_cast< time_t(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        case 4: _t->slotCloseDev((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->slotFindDev((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 6: _t->showLocalStorage(); break;
        case 7: _t->emitToFtpTranslation(); break;
        case 8: _t->starFtpTime(); break;
        case 9: _t->updateFtpProgress((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< sum_t(*)>(_a[2])),(*reinterpret_cast< copied_t(*)>(_a[3])),(*reinterpret_cast< time_t(*)>(_a[4]))); break;
        case 10: _t->ftpCfgBtnClicked(); break;
        case 11: _t->usbFmtActClicked(); break;
        case 12: _t->test(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::starFtpTransmission)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::starFtpTransmission()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
