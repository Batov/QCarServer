/****************************************************************************
** Meta object code from reading C++ file 'CarCtrl.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CarCtrl.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CarCtrl.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CarCtrl[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,    9,    8,    8, 0x0a,
      32,    8,    8,    8, 0x0a,
      48,    8,    8,    8, 0x0a,
      63,    8,    8,    8, 0x0a,
      70,    8,    8,    8, 0x0a,
      87,    8,    8,    8, 0x09,
     100,    8,    8,    8, 0x09,
     115,    8,    8,    8, 0x09,
     129,    8,    8,    8, 0x09,
     141,    8,    8,    8, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_CarCtrl[] = {
    "CarCtrl\0\0a\0Responce(QByteArray)\0"
    "EmergencyStop()\0ResumeMoving()\0Stop()\0"
    "Run(QStringList)\0Connection()\0"
    "Disconnected()\0NetworkRead()\0initSides()\0"
    "initSettings()\0"
};

void CarCtrl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CarCtrl *_t = static_cast<CarCtrl *>(_o);
        switch (_id) {
        case 0: _t->Responce((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->EmergencyStop(); break;
        case 2: _t->ResumeMoving(); break;
        case 3: _t->Stop(); break;
        case 4: _t->Run((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 5: _t->Connection(); break;
        case 6: _t->Disconnected(); break;
        case 7: _t->NetworkRead(); break;
        case 8: _t->initSides(); break;
        case 9: _t->initSettings(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CarCtrl::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CarCtrl::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CarCtrl,
      qt_meta_data_CarCtrl, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CarCtrl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CarCtrl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CarCtrl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CarCtrl))
        return static_cast<void*>(const_cast< CarCtrl*>(this));
    return QWidget::qt_metacast(_clname);
}

int CarCtrl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
