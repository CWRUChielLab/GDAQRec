/********************************************************************************
** Form generated from reading ui file 'DAQSettingsDialog.ui'
**
** Created: Sat Jan 26 18:10:07 2008
**      by: Qt User Interface Compiler version 4.3.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DAQSETTINGSDIALOG_H
#define UI_DAQSETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

class Ui_DAQSettingsDialog
{
public:
    QVBoxLayout *vboxLayout;
    QGridLayout *gridLayout;
    QLabel *label_1;
    QSpinBox *numChannels;
    QLabel *label_10;
    QLineEdit *samplingRate;
    QLabel *label_5;
    QLineEdit *maxV1;
    QLabel *label_2;
    QLineEdit *maxV2;
    QLabel *label_3;
    QLineEdit *maxV3;
    QLabel *label_4;
    QLineEdit *maxV4;
    QLabel *label_6;
    QLineEdit *maxV5;
    QLabel *label_7;
    QLineEdit *maxV6;
    QLabel *label_8;
    QLineEdit *maxV7;
    QLabel *label_9;
    QLineEdit *maxV8;
    QPushButton *fgColor;
    QPushButton *bgColor;
    QPushButton *color1;
    QPushButton *color2;
    QPushButton *color3;
    QPushButton *color4;
    QPushButton *color5;
    QPushButton *color6;
    QPushButton *color7;
    QPushButton *color8;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QSpacerItem *spacerItem1;

    void setupUi(QDialog *DAQSettingsDialog)
    {
    if (DAQSettingsDialog->objectName().isEmpty())
        DAQSettingsDialog->setObjectName(QString::fromUtf8("DAQSettingsDialog"));
    DAQSettingsDialog->resize(359, 387);
    vboxLayout = new QVBoxLayout(DAQSettingsDialog);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    gridLayout = new QGridLayout();
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label_1 = new QLabel(DAQSettingsDialog);
    label_1->setObjectName(QString::fromUtf8("label_1"));

    gridLayout->addWidget(label_1, 0, 0, 1, 1);

    numChannels = new QSpinBox(DAQSettingsDialog);
    numChannels->setObjectName(QString::fromUtf8("numChannels"));
    numChannels->setMinimum(1);
    numChannels->setMaximum(8);
    numChannels->setValue(2);

    gridLayout->addWidget(numChannels, 0, 1, 1, 1);

    label_10 = new QLabel(DAQSettingsDialog);
    label_10->setObjectName(QString::fromUtf8("label_10"));

    gridLayout->addWidget(label_10, 1, 0, 1, 1);

    samplingRate = new QLineEdit(DAQSettingsDialog);
    samplingRate->setObjectName(QString::fromUtf8("samplingRate"));

    gridLayout->addWidget(samplingRate, 1, 1, 1, 1);

    label_5 = new QLabel(DAQSettingsDialog);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    gridLayout->addWidget(label_5, 2, 0, 1, 1);

    maxV1 = new QLineEdit(DAQSettingsDialog);
    maxV1->setObjectName(QString::fromUtf8("maxV1"));

    gridLayout->addWidget(maxV1, 2, 1, 1, 1);

    label_2 = new QLabel(DAQSettingsDialog);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    gridLayout->addWidget(label_2, 3, 0, 1, 1);

    maxV2 = new QLineEdit(DAQSettingsDialog);
    maxV2->setObjectName(QString::fromUtf8("maxV2"));

    gridLayout->addWidget(maxV2, 3, 1, 1, 1);

    label_3 = new QLabel(DAQSettingsDialog);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    gridLayout->addWidget(label_3, 4, 0, 1, 1);

    maxV3 = new QLineEdit(DAQSettingsDialog);
    maxV3->setObjectName(QString::fromUtf8("maxV3"));

    gridLayout->addWidget(maxV3, 4, 1, 1, 1);

    label_4 = new QLabel(DAQSettingsDialog);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    gridLayout->addWidget(label_4, 5, 0, 1, 1);

    maxV4 = new QLineEdit(DAQSettingsDialog);
    maxV4->setObjectName(QString::fromUtf8("maxV4"));

    gridLayout->addWidget(maxV4, 5, 1, 1, 1);

    label_6 = new QLabel(DAQSettingsDialog);
    label_6->setObjectName(QString::fromUtf8("label_6"));

    gridLayout->addWidget(label_6, 6, 0, 1, 1);

    maxV5 = new QLineEdit(DAQSettingsDialog);
    maxV5->setObjectName(QString::fromUtf8("maxV5"));

    gridLayout->addWidget(maxV5, 6, 1, 1, 1);

    label_7 = new QLabel(DAQSettingsDialog);
    label_7->setObjectName(QString::fromUtf8("label_7"));

    gridLayout->addWidget(label_7, 7, 0, 2, 1);

    maxV6 = new QLineEdit(DAQSettingsDialog);
    maxV6->setObjectName(QString::fromUtf8("maxV6"));

    gridLayout->addWidget(maxV6, 7, 1, 2, 1);

    label_8 = new QLabel(DAQSettingsDialog);
    label_8->setObjectName(QString::fromUtf8("label_8"));

    gridLayout->addWidget(label_8, 9, 0, 1, 1);

    maxV7 = new QLineEdit(DAQSettingsDialog);
    maxV7->setObjectName(QString::fromUtf8("maxV7"));

    gridLayout->addWidget(maxV7, 9, 1, 1, 1);

    label_9 = new QLabel(DAQSettingsDialog);
    label_9->setObjectName(QString::fromUtf8("label_9"));

    gridLayout->addWidget(label_9, 10, 0, 1, 1);

    maxV8 = new QLineEdit(DAQSettingsDialog);
    maxV8->setObjectName(QString::fromUtf8("maxV8"));

    gridLayout->addWidget(maxV8, 10, 1, 1, 1);

    fgColor = new QPushButton(DAQSettingsDialog);
    fgColor->setObjectName(QString::fromUtf8("fgColor"));

    gridLayout->addWidget(fgColor, 0, 2, 1, 1);

    bgColor = new QPushButton(DAQSettingsDialog);
    bgColor->setObjectName(QString::fromUtf8("bgColor"));

    gridLayout->addWidget(bgColor, 1, 2, 1, 1);

    color1 = new QPushButton(DAQSettingsDialog);
    color1->setObjectName(QString::fromUtf8("color1"));

    gridLayout->addWidget(color1, 2, 2, 1, 1);

    color2 = new QPushButton(DAQSettingsDialog);
    color2->setObjectName(QString::fromUtf8("color2"));

    gridLayout->addWidget(color2, 3, 2, 1, 1);

    color3 = new QPushButton(DAQSettingsDialog);
    color3->setObjectName(QString::fromUtf8("color3"));

    gridLayout->addWidget(color3, 4, 2, 1, 1);

    color4 = new QPushButton(DAQSettingsDialog);
    color4->setObjectName(QString::fromUtf8("color4"));

    gridLayout->addWidget(color4, 5, 2, 1, 1);

    color5 = new QPushButton(DAQSettingsDialog);
    color5->setObjectName(QString::fromUtf8("color5"));

    gridLayout->addWidget(color5, 6, 2, 2, 1);

    color6 = new QPushButton(DAQSettingsDialog);
    color6->setObjectName(QString::fromUtf8("color6"));

    gridLayout->addWidget(color6, 8, 2, 1, 1);

    color7 = new QPushButton(DAQSettingsDialog);
    color7->setObjectName(QString::fromUtf8("color7"));

    gridLayout->addWidget(color7, 9, 2, 1, 1);

    color8 = new QPushButton(DAQSettingsDialog);
    color8->setObjectName(QString::fromUtf8("color8"));

    gridLayout->addWidget(color8, 10, 2, 1, 1);


    vboxLayout->addLayout(gridLayout);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    okButton = new QPushButton(DAQSettingsDialog);
    okButton->setObjectName(QString::fromUtf8("okButton"));
    okButton->setDefault(true);

    hboxLayout->addWidget(okButton);

    cancelButton = new QPushButton(DAQSettingsDialog);
    cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

    hboxLayout->addWidget(cancelButton);


    vboxLayout->addLayout(hboxLayout);

    spacerItem1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout->addItem(spacerItem1);

    label_1->setBuddy(numChannels);
    label_10->setBuddy(samplingRate);
    label_5->setBuddy(maxV1);
    label_2->setBuddy(maxV2);
    label_3->setBuddy(maxV3);
    label_4->setBuddy(maxV4);
    label_6->setBuddy(maxV5);
    label_7->setBuddy(maxV6);
    label_8->setBuddy(maxV7);
    label_9->setBuddy(maxV8);
    QWidget::setTabOrder(numChannels, samplingRate);
    QWidget::setTabOrder(samplingRate, maxV1);
    QWidget::setTabOrder(maxV1, maxV2);
    QWidget::setTabOrder(maxV2, maxV3);
    QWidget::setTabOrder(maxV3, maxV4);
    QWidget::setTabOrder(maxV4, maxV5);
    QWidget::setTabOrder(maxV5, maxV6);
    QWidget::setTabOrder(maxV6, maxV7);
    QWidget::setTabOrder(maxV7, maxV8);
    QWidget::setTabOrder(maxV8, okButton);
    QWidget::setTabOrder(okButton, cancelButton);

    retranslateUi(DAQSettingsDialog);

    QMetaObject::connectSlotsByName(DAQSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *DAQSettingsDialog)
    {
    DAQSettingsDialog->setWindowTitle(QApplication::translate("DAQSettingsDialog", "DAQ Settings", 0, QApplication::UnicodeUTF8));
    label_1->setText(QApplication::translate("DAQSettingsDialog", "Number of &Channels:", 0, QApplication::UnicodeUTF8));
    label_10->setText(QApplication::translate("DAQSettingsDialog", "&Sampling Rate", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("DAQSettingsDialog", "Channel &1 Max Voltage", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("DAQSettingsDialog", "Channel &2 Max Voltage", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("DAQSettingsDialog", "Channel &3 Max Voltage", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("DAQSettingsDialog", "Channel &4 Max Voltage", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("DAQSettingsDialog", "Channel &5 Max Voltage", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("DAQSettingsDialog", "Channel &6 Max Voltage", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("DAQSettingsDialog", "Channel &7 Max Voltage", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("DAQSettingsDialog", "Channel &8 Max Voltage", 0, QApplication::UnicodeUTF8));
    fgColor->setText(QApplication::translate("DAQSettingsDialog", "foreground", 0, QApplication::UnicodeUTF8));
    bgColor->setText(QApplication::translate("DAQSettingsDialog", "background", 0, QApplication::UnicodeUTF8));
    color1->setText(QApplication::translate("DAQSettingsDialog", "line 1", 0, QApplication::UnicodeUTF8));
    color2->setText(QApplication::translate("DAQSettingsDialog", "line 2", 0, QApplication::UnicodeUTF8));
    color3->setText(QApplication::translate("DAQSettingsDialog", "line 3", 0, QApplication::UnicodeUTF8));
    color4->setText(QApplication::translate("DAQSettingsDialog", "line 4", 0, QApplication::UnicodeUTF8));
    color5->setText(QApplication::translate("DAQSettingsDialog", "line 5", 0, QApplication::UnicodeUTF8));
    color6->setText(QApplication::translate("DAQSettingsDialog", "line 6", 0, QApplication::UnicodeUTF8));
    color7->setText(QApplication::translate("DAQSettingsDialog", "line 7", 0, QApplication::UnicodeUTF8));
    color8->setText(QApplication::translate("DAQSettingsDialog", "line 8", 0, QApplication::UnicodeUTF8));
    okButton->setText(QApplication::translate("DAQSettingsDialog", "OK", 0, QApplication::UnicodeUTF8));
    cancelButton->setText(QApplication::translate("DAQSettingsDialog", "Cancel", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(DAQSettingsDialog);
    } // retranslateUi

};

namespace Ui {
    class DAQSettingsDialog: public Ui_DAQSettingsDialog {};
} // namespace Ui

#endif // UI_DAQSETTINGSDIALOG_H
