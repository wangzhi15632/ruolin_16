/****************************************************************************
** Meta object code from reading C++ file 'updatedbsdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../usb_copy/updatedbsdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'updatedbsdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UpdateDbsDialog_t {
    QByteArrayData data[12];
    char stringdata0[157];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UpdateDbsDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UpdateDbsDialog_t qt_meta_stringdata_UpdateDbsDialog = {
    {
QT_MOC_LITERAL(0, 0, 15), // "UpdateDbsDialog"
QT_MOC_LITERAL(1, 16, 15), // "databaseUpdated"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 20), // "on_cancelBtn_clicked"
QT_MOC_LITERAL(4, 54, 20), // "on_unlockBtn_clicked"
QT_MOC_LITERAL(5, 75, 14), // "onProcFinished"
QT_MOC_LITERAL(6, 90, 8), // "exitCode"
QT_MOC_LITERAL(7, 99, 20), // "QProcess::ExitStatus"
QT_MOC_LITERAL(8, 120, 10), // "exitStatus"
QT_MOC_LITERAL(9, 131, 10), // "taskFinish"
QT_MOC_LITERAL(10, 142, 6), // "taskid"
QT_MOC_LITERAL(11, 149, 7) // "success"

    },
    "UpdateDbsDialog\0databaseUpdated\0\0"
    "on_cancelBtn_clicked\0on_unlockBtn_clicked\0"
    "onProcFinished\0exitCode\0QProcess::ExitStatus\0"
    "exitStatus\0taskFinish\0taskid\0success"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UpdateDbsDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   40,    2, 0x08 /* Private */,
       4,    0,   41,    2, 0x08 /* Private */,
       5,    2,   42,    2, 0x08 /* Private */,
       9,    2,   47,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 7,    6,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,   10,   11,

       0        // eod
};

void UpdateDbsDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UpdateDbsDialog *_t = static_cast<UpdateDbsDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->databaseUpdated(); break;
        case 1: _t->on_cancelBtn_clicked(); break;
        case 2: _t->on_unlockBtn_clicked(); break;
        case 3: _t->onProcFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 4: _t->taskFinish((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (UpdateDbsDialog::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UpdateDbsDialog::databaseUpdated)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject UpdateDbsDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_UpdateDbsDialog.data,
      qt_meta_data_UpdateDbsDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UpdateDbsDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UpdateDbsDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UpdateDbsDialog.stringdata0))
        return static_cast<void*>(const_cast< UpdateDbsDialog*>(this));
    if (!strcmp(_clname, "Ui_UpdateDbsDialog"))
        return static_cast< Ui_UpdateDbsDialog*>(const_cast< UpdateDbsDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int UpdateDbsDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void UpdateDbsDialog::databaseUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
