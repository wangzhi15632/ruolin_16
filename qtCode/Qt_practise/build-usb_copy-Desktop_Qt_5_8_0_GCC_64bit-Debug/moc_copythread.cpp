/****************************************************************************
** Meta object code from reading C++ file 'copythread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../usb_copy/copythread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'copythread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CopyThread_t {
    QByteArrayData data[10];
    char stringdata0[81];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CopyThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CopyThread_t qt_meta_stringdata_CopyThread = {
    {
QT_MOC_LITERAL(0, 0, 10), // "CopyThread"
QT_MOC_LITERAL(1, 11, 8), // "sendToUI"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 5), // "sum_t"
QT_MOC_LITERAL(4, 27, 8), // "copied_t"
QT_MOC_LITERAL(5, 36, 6), // "time_t"
QT_MOC_LITERAL(6, 43, 12), // "sendUDevInfo"
QT_MOC_LITERAL(7, 56, 6), // "rcvPar"
QT_MOC_LITERAL(8, 63, 5), // "char*"
QT_MOC_LITERAL(9, 69, 11) // "timeOutEmit"

    },
    "CopyThread\0sendToUI\0\0sum_t\0copied_t\0"
    "time_t\0sendUDevInfo\0rcvPar\0char*\0"
    "timeOutEmit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CopyThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    5,   34,    2, 0x06 /* Public */,
       6,    4,   45,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   54,    2, 0x08 /* Private */,
       9,    0,   57,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 3, 0x80000000 | 4, 0x80000000 | 5, QMetaType::Bool,    2,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::ULong, QMetaType::ULong, QMetaType::ULong,    2,    2,    2,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void,

       0        // eod
};

void CopyThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CopyThread *_t = static_cast<CopyThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendToUI((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< sum_t(*)>(_a[2])),(*reinterpret_cast< copied_t(*)>(_a[3])),(*reinterpret_cast< time_t(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        case 1: _t->sendUDevInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< ulong(*)>(_a[2])),(*reinterpret_cast< ulong(*)>(_a[3])),(*reinterpret_cast< ulong(*)>(_a[4]))); break;
        case 2: _t->rcvPar((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 3: _t->timeOutEmit(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CopyThread::*_t)(int , sum_t , copied_t , time_t , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CopyThread::sendToUI)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CopyThread::*_t)(int , unsigned long , unsigned long , unsigned long );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CopyThread::sendUDevInfo)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject CopyThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CopyThread.data,
      qt_meta_data_CopyThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CopyThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CopyThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CopyThread.stringdata0))
        return static_cast<void*>(const_cast< CopyThread*>(this));
    return QThread::qt_metacast(_clname);
}

int CopyThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void CopyThread::sendToUI(int _t1, sum_t _t2, copied_t _t3, time_t _t4, bool _t5)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CopyThread::sendUDevInfo(int _t1, unsigned long _t2, unsigned long _t3, unsigned long _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
