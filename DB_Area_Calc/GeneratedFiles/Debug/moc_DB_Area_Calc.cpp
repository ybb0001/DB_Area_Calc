/****************************************************************************
** Meta object code from reading C++ file 'DB_Area_Calc.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DB_Area_Calc.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DB_Area_Calc.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DB_Area_Calc_t {
    QByteArrayData data[20];
    char stringdata0[326];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DB_Area_Calc_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DB_Area_Calc_t qt_meta_stringdata_DB_Area_Calc = {
    {
QT_MOC_LITERAL(0, 0, 12), // "DB_Area_Calc"
QT_MOC_LITERAL(1, 13, 30), // "on_pushButton_open_BMP_clicked"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 31), // "on_pushButton_ClearBack_clicked"
QT_MOC_LITERAL(4, 77, 27), // "on_pushButton_clear_clicked"
QT_MOC_LITERAL(5, 105, 16), // "load_Panel_Value"
QT_MOC_LITERAL(6, 122, 21), // "get_Binaryztion_Level"
QT_MOC_LITERAL(7, 144, 20), // "get_backGround_Level"
QT_MOC_LITERAL(8, 165, 19), // "get_Image_Threshold"
QT_MOC_LITERAL(9, 185, 23), // "get_Image_Threshold_low"
QT_MOC_LITERAL(10, 209, 21), // "get_Image_100_min_max"
QT_MOC_LITERAL(11, 231, 16), // "sensor_area_calc"
QT_MOC_LITERAL(12, 248, 15), // "prism_area_calc"
QT_MOC_LITERAL(13, 264, 12), // "Rect_Correct"
QT_MOC_LITERAL(14, 277, 11), // "remove_Edge"
QT_MOC_LITERAL(15, 289, 12), // "non_Bond_Fix"
QT_MOC_LITERAL(16, 302, 8), // "edge_Fix"
QT_MOC_LITERAL(17, 311, 5), // "getTH"
QT_MOC_LITERAL(18, 317, 1), // "k"
QT_MOC_LITERAL(19, 319, 6) // "offset"

    },
    "DB_Area_Calc\0on_pushButton_open_BMP_clicked\0"
    "\0on_pushButton_ClearBack_clicked\0"
    "on_pushButton_clear_clicked\0"
    "load_Panel_Value\0get_Binaryztion_Level\0"
    "get_backGround_Level\0get_Image_Threshold\0"
    "get_Image_Threshold_low\0get_Image_100_min_max\0"
    "sensor_area_calc\0prism_area_calc\0"
    "Rect_Correct\0remove_Edge\0non_Bond_Fix\0"
    "edge_Fix\0getTH\0k\0offset"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DB_Area_Calc[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x08 /* Private */,
       3,    0,   95,    2, 0x08 /* Private */,
       4,    0,   96,    2, 0x08 /* Private */,
       5,    0,   97,    2, 0x08 /* Private */,
       6,    0,   98,    2, 0x08 /* Private */,
       7,    0,   99,    2, 0x08 /* Private */,
       8,    0,  100,    2, 0x08 /* Private */,
       9,    0,  101,    2, 0x08 /* Private */,
      10,    0,  102,    2, 0x08 /* Private */,
      11,    0,  103,    2, 0x08 /* Private */,
      12,    0,  104,    2, 0x08 /* Private */,
      13,    0,  105,    2, 0x08 /* Private */,
      14,    0,  106,    2, 0x08 /* Private */,
      15,    0,  107,    2, 0x08 /* Private */,
      16,    0,  108,    2, 0x08 /* Private */,
      17,    2,  109,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::UInt,
    QMetaType::UInt,
    QMetaType::UInt,
    QMetaType::UInt,
    QMetaType::UInt,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int, QMetaType::Int, QMetaType::Int,   18,   19,

       0        // eod
};

void DB_Area_Calc::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DB_Area_Calc *_t = static_cast<DB_Area_Calc *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_pushButton_open_BMP_clicked(); break;
        case 1: _t->on_pushButton_ClearBack_clicked(); break;
        case 2: _t->on_pushButton_clear_clicked(); break;
        case 3: _t->load_Panel_Value(); break;
        case 4: { uint _r = _t->get_Binaryztion_Level();
            if (_a[0]) *reinterpret_cast< uint*>(_a[0]) = _r; }  break;
        case 5: { uint _r = _t->get_backGround_Level();
            if (_a[0]) *reinterpret_cast< uint*>(_a[0]) = _r; }  break;
        case 6: { uint _r = _t->get_Image_Threshold();
            if (_a[0]) *reinterpret_cast< uint*>(_a[0]) = _r; }  break;
        case 7: { uint _r = _t->get_Image_Threshold_low();
            if (_a[0]) *reinterpret_cast< uint*>(_a[0]) = _r; }  break;
        case 8: { uint _r = _t->get_Image_100_min_max();
            if (_a[0]) *reinterpret_cast< uint*>(_a[0]) = _r; }  break;
        case 9: _t->sensor_area_calc(); break;
        case 10: _t->prism_area_calc(); break;
        case 11: { int _r = _t->Rect_Correct();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 12: { int _r = _t->remove_Edge();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 13: { int _r = _t->non_Bond_Fix();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 14: { int _r = _t->edge_Fix();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 15: { int _r = _t->getTH((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject DB_Area_Calc::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DB_Area_Calc.data,
      qt_meta_data_DB_Area_Calc,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DB_Area_Calc::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DB_Area_Calc::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DB_Area_Calc.stringdata0))
        return static_cast<void*>(const_cast< DB_Area_Calc*>(this));
    return QWidget::qt_metacast(_clname);
}

int DB_Area_Calc::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
