/****************************************************************************
** Meta object code from reading C++ file 'ftp_traversing.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../usb_copy/ftp_traversing.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ftp_traversing.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FtpTraversing_t {
    QByteArrayData data[5];
    char stringdata0[50];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FtpTraversing_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FtpTraversing_t qt_meta_stringdata_FtpTraversing = {
    {
QT_MOC_LITERAL(0, 0, 13), // "FtpTraversing"
QT_MOC_LITERAL(1, 14, 10), // "starFtpPut"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 5), // "char*"
QT_MOC_LITERAL(4, 32, 17) // "transmission_task"

    },
    "FtpTraversing\0starFtpPut\0\0char*\0"
    "transmission_task"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FtpTraversing[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   31,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString, QMetaType::LongLong,    2,    2,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void FtpTraversing::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FtpTraversing *_t = static_cast<FtpTraversing *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->starFtpPut((*reinterpret_cast< char*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< long long(*)>(_a[3]))); break;
        case 1: _t->transmission_task(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FtpTraversing::*_t)(char * , const QString , long long );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FtpTraversing::starFtpPut)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject FtpTraversing::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FtpTraversing.data,
      qt_meta_data_FtpTraversing,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FtpTraversing::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FtpTraversing::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FtpTraversing.stringdata0))
        return static_cast<void*>(const_cast< FtpTraversing*>(this));
    return QObject::qt_metacast(_clname);
}

int FtpTraversing::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void FtpTraversing::starFtpPut(char * _t1, const QString _t2, long long _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
