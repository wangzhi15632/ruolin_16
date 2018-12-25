/****************************************************************************
** Meta object code from reading C++ file 'ftpconfig.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../usb_copy/ftpconfig.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ftpconfig.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FtpConfig_t {
    QByteArrayData data[13];
    char stringdata0[171];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FtpConfig_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FtpConfig_t qt_meta_stringdata_FtpConfig = {
    {
QT_MOC_LITERAL(0, 0, 9), // "FtpConfig"
QT_MOC_LITERAL(1, 10, 19), // "connectOrDisconnect"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 10), // "uploadFile"
QT_MOC_LITERAL(4, 42, 12), // "cancelUpload"
QT_MOC_LITERAL(5, 55, 12), // "connectToFtp"
QT_MOC_LITERAL(6, 68, 18), // "ftpCommandFinished"
QT_MOC_LITERAL(7, 87, 9), // "commandId"
QT_MOC_LITERAL(8, 97, 5), // "error"
QT_MOC_LITERAL(9, 103, 26), // "updateDataTransferProgress"
QT_MOC_LITERAL(10, 130, 9), // "readBytes"
QT_MOC_LITERAL(11, 140, 10), // "totalBytes"
QT_MOC_LITERAL(12, 151, 19) // "enableConnectButton"

    },
    "FtpConfig\0connectOrDisconnect\0\0"
    "uploadFile\0cancelUpload\0connectToFtp\0"
    "ftpCommandFinished\0commandId\0error\0"
    "updateDataTransferProgress\0readBytes\0"
    "totalBytes\0enableConnectButton"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FtpConfig[] = {

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
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    0,   51,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    2,   53,    2, 0x08 /* Private */,
       9,    2,   58,    2, 0x08 /* Private */,
      12,    0,   63,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    7,    8,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,   10,   11,
    QMetaType::Void,

       0        // eod
};

void FtpConfig::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FtpConfig *_t = static_cast<FtpConfig *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connectOrDisconnect(); break;
        case 1: _t->uploadFile(); break;
        case 2: _t->cancelUpload(); break;
        case 3: _t->connectToFtp(); break;
        case 4: _t->ftpCommandFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->updateDataTransferProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 6: _t->enableConnectButton(); break;
        default: ;
        }
    }
}

const QMetaObject FtpConfig::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FtpConfig.data,
      qt_meta_data_FtpConfig,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FtpConfig::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FtpConfig::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FtpConfig.stringdata0))
        return static_cast<void*>(const_cast< FtpConfig*>(this));
    return QDialog::qt_metacast(_clname);
}

int FtpConfig::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
