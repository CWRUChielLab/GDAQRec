#ifndef DAQSETTINGSDIALOG_H
#define DAQSETTINGSDIALOG_H 

#include <QDialog>
#include <ui_DAQSettingsDialog.h>

struct DAQSettings 
{
   enum { maxChannels = 8 };

   int samplingRate;
   int numChannels;
   QColor fgColor;
   QColor bgColor;
   double maxVoltage[maxChannels];
   double minVoltage[maxChannels];
   QColor color[maxChannels];

   DAQSettings();
   
   void save();
   void restore();
};

class DAQSettingsDialog : public QDialog, public Ui::DAQSettingsDialog 
{
   Q_OBJECT

public:
      DAQSettings settings;

public:
      DAQSettingsDialog(const DAQSettings& settings_, 
            QWidget* parent = NULL);

private slots:
      void bgColorClicked();
      void fgColorClicked();
      void color1Clicked();
      void color2Clicked();
      void color3Clicked();
      void color4Clicked();
      void color5Clicked();
      void color6Clicked();
      void color7Clicked();
      void color8Clicked();
      void textChanged();
      void numChannelsChanged(int numChannels);
};

#endif /* DAQSETTINGSDIALOG_H */
