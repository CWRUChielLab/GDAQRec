/****************************************************************************
** Meta object code from reading C++ file 'DAQSettingsDialog.h'
**
** Created: Sat Jan 26 19:10:10 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "DAQSettingsDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DAQSettingsDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_DAQSettingsDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x08,
      36,   18,   18,   18, 0x08,
      53,   18,   18,   18, 0x08,
      69,   18,   18,   18, 0x08,
      85,   18,   18,   18, 0x08,
     101,   18,   18,   18, 0x08,
     117,   18,   18,   18, 0x08,
     133,   18,   18,   18, 0x08,
     149,   18,   18,   18, 0x08,
     165,   18,   18,   18, 0x08,
     181,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DAQSettingsDialog[] = {
    "DAQSettingsDialog\0\0bgColorClicked()\0"
    "fgColorClicked()\0color1Clicked()\0"
    "color2Clicked()\0color3Clicked()\0"
    "color4Clicked()\0color5Clicked()\0"
    "color6Clicked()\0color7Clicked()\0"
    "color8Clicked()\0textChanged()\0"
};

const QMetaObject DAQSettingsDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DAQSettingsDialog,
      qt_meta_data_DAQSettingsDialog, 0 }
};

const QMetaObject *DAQSettingsDialog::metaObject() const
{
    return &staticMetaObject;
}

void *DAQSettingsDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DAQSettingsDialog))
	return static_cast<void*>(const_cast< DAQSettingsDialog*>(this));
    if (!strcmp(_clname, "Ui::DAQSettingsDialog"))
	return static_cast< Ui::DAQSettingsDialog*>(const_cast< DAQSettingsDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int DAQSettingsDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: bgColorClicked(); break;
        case 1: fgColorClicked(); break;
        case 2: color1Clicked(); break;
        case 3: color2Clicked(); break;
        case 4: color3Clicked(); break;
        case 5: color4Clicked(); break;
        case 6: color5Clicked(); break;
        case 7: color6Clicked(); break;
        case 8: color7Clicked(); break;
        case 9: color8Clicked(); break;
        case 10: textChanged(); break;
        }
        _id -= 11;
    }
    return _id;
}
