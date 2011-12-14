#include <QtGui>
#include "DAQSettingsDialog.h"

DAQSettings::DAQSettings()
{
}


void DAQSettings::save() 
{
   QSettings settings("Chiel Lab", "GDAQrec");

   settings.setValue("numChannels", numChannels);
   settings.setValue("samplingRate", samplingRate);
   settings.setValue("bgColor", bgColor);
   settings.setValue("fgColor", fgColor);

   for (int i = 0; i < maxChannels; ++i) {
      settings.setValue(QString("maxVoltage") + QString::number(i+1), 
            maxVoltage[i]);
      settings.setValue(QString("minVoltage") + QString::number(i+1), 
            minVoltage[i]);
      settings.setValue(QString("color") + QString::number(i+1), 
            color[i]);
   }
}

void DAQSettings::restore() 
{
   QSettings settings("Chiel Lab", "GDAQrec");

   numChannels = settings.value("numChannels", 2).toInt();
   samplingRate = settings.value("samplingRate", 100).toInt();
   bgColor = settings.value("bgColor", Qt::black).value<QColor>();
   fgColor = settings.value("fgColor", Qt::white).value<QColor>();

   static const QColor defaultColors[8] = {
       Qt::yellow,   Qt::green,  Qt::white,     Qt::red, 
       Qt::blue,     Qt::cyan,   Qt::magenta,   Qt::darkGreen
    };

   for (int i = 0; i < maxChannels; ++i) {
      maxVoltage[i] = settings.value(
            QString("maxVoltage") + QString::number(i+1), 10.0).toDouble();
      minVoltage[i] = settings.value(
            QString("minVoltage") + QString::number(i+1), -10.0).toDouble();
      color[i] = settings.value(
            QString("color") + QString::number(i+1), 
            defaultColors[i]).value<QColor>();
   }
}

DAQSettingsDialog::DAQSettingsDialog(const DAQSettings& settings_, 
      QWidget* parent) 
   : QDialog(parent)
{
   setupUi(this);
   settings = settings_;

   numChannels->setValue(settings.numChannels);
   samplingRate->setText(QString::number(settings.samplingRate));
   maxV1->setText(QString::number(settings.maxVoltage[0],'f',2));
   maxV2->setText(QString::number(settings.maxVoltage[1],'f',2));
   maxV3->setText(QString::number(settings.maxVoltage[2],'f',2));
   maxV4->setText(QString::number(settings.maxVoltage[3],'f',2));
   maxV5->setText(QString::number(settings.maxVoltage[4],'f',2));
   maxV6->setText(QString::number(settings.maxVoltage[5],'f',2));
   maxV7->setText(QString::number(settings.maxVoltage[6],'f',2));
   maxV8->setText(QString::number(settings.maxVoltage[7],'f',2));

   numChannelsChanged(settings.numChannels);

   samplingRate->setValidator(
         new QRegExpValidator(QRegExp(
               "(500000|[1-4][0-9]{5}|[1-9][0-9]{,4})"), this));
   QRegExp voltageRegExp(
         "(([1-9][0-9]?\\.?[0-9]*)|([0-9]{,2}\\.[0-9]?[1-9][0-9]*))"
         );
   maxV1->setValidator(new QRegExpValidator(voltageRegExp, this));
   maxV2->setValidator(new QRegExpValidator(voltageRegExp, this));
   maxV3->setValidator(new QRegExpValidator(voltageRegExp, this));
   maxV4->setValidator(new QRegExpValidator(voltageRegExp, this));
   maxV5->setValidator(new QRegExpValidator(voltageRegExp, this));
   maxV6->setValidator(new QRegExpValidator(voltageRegExp, this));
   maxV7->setValidator(new QRegExpValidator(voltageRegExp, this));
   maxV8->setValidator(new QRegExpValidator(voltageRegExp, this));

   bgColor->setPalette(settings.bgColor);
   fgColor->setPalette(settings.fgColor);
   color1->setPalette(settings.color[0]);
   color2->setPalette(settings.color[1]);
   color3->setPalette(settings.color[2]);
   color4->setPalette(settings.color[3]);
   color5->setPalette(settings.color[4]);
   color6->setPalette(settings.color[5]);
   color7->setPalette(settings.color[6]);
   color8->setPalette(settings.color[7]);

   connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
   connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
   connect(fgColor, SIGNAL(clicked()), this, SLOT(fgColorClicked()));
   connect(bgColor, SIGNAL(clicked()), this, SLOT(bgColorClicked()));
   connect(color1, SIGNAL(clicked()), this, SLOT(color1Clicked()));
   connect(color2, SIGNAL(clicked()), this, SLOT(color2Clicked()));
   connect(color3, SIGNAL(clicked()), this, SLOT(color3Clicked()));
   connect(color4, SIGNAL(clicked()), this, SLOT(color4Clicked()));
   connect(color5, SIGNAL(clicked()), this, SLOT(color5Clicked()));
   connect(color6, SIGNAL(clicked()), this, SLOT(color6Clicked()));
   connect(color7, SIGNAL(clicked()), this, SLOT(color7Clicked()));
   connect(color8, SIGNAL(clicked()), this, SLOT(color8Clicked()));

   connect(numChannels, SIGNAL(valueChanged(int)), this, 
         SLOT(numChannelsChanged(int)));
   connect(samplingRate, SIGNAL(textChanged(const QString&)), this, 
         SLOT(textChanged()));
   connect(maxV1, SIGNAL(textChanged(const QString&)), this, 
         SLOT(textChanged()));
   connect(maxV2, SIGNAL(textChanged(const QString&)), this, 
         SLOT(textChanged()));
   connect(maxV3, SIGNAL(textChanged(const QString&)), this,
         SLOT(textChanged()));
   connect(maxV4, SIGNAL(textChanged(const QString&)), this,
         SLOT(textChanged()));
   connect(maxV5, SIGNAL(textChanged(const QString&)), this,
         SLOT(textChanged()));
   connect(maxV6, SIGNAL(textChanged(const QString&)), this,
         SLOT(textChanged()));
   connect(maxV7, SIGNAL(textChanged(const QString&)), this, 
         SLOT(textChanged()));
   connect(maxV8, SIGNAL(textChanged(const QString&)), this,
         SLOT(textChanged()));
}

void DAQSettingsDialog::fgColorClicked()
{
   QColor color = QColorDialog::getColor(settings.fgColor, this);
   
   if (color.isValid()) {
      fgColor->setPalette(QPalette(color));
      settings.fgColor = color;
   }
}


void DAQSettingsDialog::bgColorClicked()
{
   QColor color = QColorDialog::getColor(settings.bgColor, this);
   
   if (color.isValid()) {
      bgColor->setPalette(QPalette(color));
      settings.bgColor = color;
   }
}


void DAQSettingsDialog::color1Clicked()
{
   QColor color = QColorDialog::getColor(settings.color[0], this);
   
   if (color.isValid()) {
      color1->setPalette(QPalette(color));
      settings.color[0] = color;
   }
}


void DAQSettingsDialog::color2Clicked()
{
   QColor color = QColorDialog::getColor(settings.color[1], this);
   
   if (color.isValid()) {
      color2->setPalette(QPalette(color));
      settings.color[1] = color;
   }
}


void DAQSettingsDialog::color3Clicked()
{
   QColor color = QColorDialog::getColor(settings.color[2], this);
   
   if (color.isValid()) {
      color3->setPalette(QPalette(color));
      settings.color[2] = color;
   }
}


void DAQSettingsDialog::color4Clicked()
{
   QColor color = QColorDialog::getColor(settings.color[3], this);
   
   if (color.isValid()) {
      color4->setPalette(QPalette(color));
      settings.color[3] = color;
   }
}


void DAQSettingsDialog::color5Clicked()
{
   QColor color = QColorDialog::getColor(settings.color[4], this);
   
   if (color.isValid()) {
      color5->setPalette(QPalette(color));
      settings.color[4] = color;
   }
}


void DAQSettingsDialog::color6Clicked()
{
   QColor color = QColorDialog::getColor(settings.color[5], this);
   
   if (color.isValid()) {
      color6->setPalette(QPalette(color));
      settings.color[5] = color;
   }
}


void DAQSettingsDialog::color7Clicked()
{
   QColor color = QColorDialog::getColor(settings.color[6], this);
   
   if (color.isValid()) {
      color7->setPalette(QPalette(color));
      settings.color[6] = color;
   }
}


void DAQSettingsDialog::color8Clicked()
{
   QColor color = QColorDialog::getColor(settings.color[7], this);
   
   if (color.isValid()) {
      color8->setPalette(QPalette(color));
      settings.color[7] = color;
   }
}


void DAQSettingsDialog::textChanged() 
{
   bool valid = samplingRate->hasAcceptableInput()
         && maxV1->hasAcceptableInput()
         && maxV2->hasAcceptableInput()
         && maxV3->hasAcceptableInput()
         && maxV4->hasAcceptableInput()
         && maxV5->hasAcceptableInput()
         && maxV6->hasAcceptableInput()
         && maxV7->hasAcceptableInput()
         && maxV8->hasAcceptableInput();

   if (valid) 
   {
      int newSamplingRate = samplingRate->text().toInt();  
      settings.samplingRate = newSamplingRate;

      // make sure the total sampling rate isn't greater than 
      // 250000/numChannels (unless the number of channels is 1)
      int maxChannels = std::max(1, 250000/newSamplingRate);

      if (numChannels->value() > maxChannels) {
         numChannels->setValue(maxChannels);
      }

      settings.minVoltage[0] = 
         -(settings.maxVoltage[0] = maxV1->text().toDouble());
      settings.minVoltage[1] = 
         -(settings.maxVoltage[1] = maxV2->text().toDouble());
      settings.minVoltage[2] = 
         -(settings.maxVoltage[2] = maxV3->text().toDouble());
      settings.minVoltage[3] = 
         -(settings.maxVoltage[3] = maxV4->text().toDouble());
      settings.minVoltage[4] = 
         -(settings.maxVoltage[4] = maxV5->text().toDouble());
      settings.minVoltage[5] = 
         -(settings.maxVoltage[5] = maxV6->text().toDouble());
      settings.minVoltage[6] = 
         -(settings.maxVoltage[6] = maxV7->text().toDouble());
      settings.minVoltage[7] = 
         -(settings.maxVoltage[7] = maxV8->text().toDouble());

   }

   okButton->setEnabled(valid);
}

void DAQSettingsDialog::numChannelsChanged(int newNumChannels) 
{
   settings.numChannels = newNumChannels;

   if (newNumChannels > 1) {
      int maxSamplingRate = 250000/newNumChannels;

      if (samplingRate->text().toInt() > maxSamplingRate) {
         samplingRate->setText(QString::number(maxSamplingRate));
      }
   }

   maxV2->setEnabled(newNumChannels >= 2);
   maxV3->setEnabled(newNumChannels >= 3);
   maxV4->setEnabled(newNumChannels >= 4);
   maxV5->setEnabled(newNumChannels >= 5);
   maxV6->setEnabled(newNumChannels >= 6);
   maxV7->setEnabled(newNumChannels >= 7);
   maxV8->setEnabled(newNumChannels >= 8);

   color2->setEnabled(newNumChannels >= 2);
   color3->setEnabled(newNumChannels >= 3);
   color4->setEnabled(newNumChannels >= 4);
   color5->setEnabled(newNumChannels >= 5);
   color6->setEnabled(newNumChannels >= 6);
   color7->setEnabled(newNumChannels >= 7);
   color8->setEnabled(newNumChannels >= 8);
}

