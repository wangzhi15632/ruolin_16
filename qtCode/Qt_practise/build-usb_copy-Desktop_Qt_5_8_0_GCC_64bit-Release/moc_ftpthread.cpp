/****************************************************************************
** Meta object code from reading C++ file 'ftpthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../usb_copy/ftpthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ftpthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FtpManager_t {
    QByteArrayData data[14];
    char stringdata0[145];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FtpManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FtpManager_t qt_meta_stringdata_FtpManager = {
    {
QT_MOC_LITERAL(0, 0, 10), // "FtpManager"
QT_MOC_LITERAL(1, 11, 5), // "error"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 27), // "QNetworkReply::NetworkError"
QT_MOC_LITERAL(4, 46, 11), // "sendFtpInfo"
QT_MOC_LITERAL(5, 58, 5), // "sum_t"
QT_MOC_LITERAL(6, 64, 8), // "copied_t"
QT_MOC_LITERAL(7, 73, 6), // "time_t"
QT_MOC_LITERAL(8, 80, 13), // "replyFinished"
QT_MOC_LITERAL(9, 94, 14), // "QNetworkReply*"
QT_MOC_LITERAL(10, 109, 9), // "loadError"
QT_MOC_LITERAL(11, 119, 3), // "put"
QT_MOC_LITERAL(12, 123, 5), // "char*"
QT_MOC_LITERAL(13, 129, 15) // "ftpErrorTimeout"

    },
    "FtpManager\0error\0\0QNetworkReply::NetworkError\0"
    "sendFtpInfo\0sum_t\0copied_t\0time_t\0"
    "replyFinished\0QNetworkReply*\0loadError\0"
    "put\0char*\0ftpErrorTimeout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FtpManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    4,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   56,    2, 0x08 /* Private */,
      10,    1,   59,    2, 0x08 /* Private */,
      11,    3,   62,    2, 0x08 /* Private */,
      13,    0,   69,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 5, 0x80000000 | 6, 0x80000000 | 7,    2,    2,    2,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 9,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 12, QMetaType::QString, QMetaType::LongLong,    2,    2,    2,
    QMetaType::Void,

       0        // eod
};

void FtpManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FtpManager *_t = static_cast<FtpManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->error((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        case 1: _t->sendFtpInfo((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< sum_t(*)>(_a[2])),(*reinterpret_cast< copied_t(*)>(_a[3])),(*reinterpret_cast< time_t(*)>(_a[4]))); break;
        case 2: _t->replyFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 3: _t->loadError((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        case 4: _t->put((*reinterpret_cast< char*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< long long(*)>(_a[3]))); break;
        case 5: _t->ftpErrorTimeout(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply::NetworkError >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply::NetworkError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FtpManager::*_t)(QNetworkReply::NetworkError );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FtpManager::error)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (FtpManager::*_t)(QString , sum_t , copied_t , time_t );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FtpManager::sendFtpInfo)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject FtpManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FtpManager.data,
      qt_meta_data_FtpManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FtpManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FtpManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FtpManager.stringdata0))
        return static_cast<void*>(const_cast< FtpManager*>(this));
    return QObject::qt_metacast(_clname);
}

int FtpManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void FtpManager::error(QNetworkReply::NetworkError _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FtpManager::sendFtpInfo(QString _t1, sum_t _t2, copied_t _t3, time_t _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
