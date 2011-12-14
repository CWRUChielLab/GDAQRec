#ifndef DAQREADER_H 
#define DAQREADER_H

#include <QThread>
#include <QMutex>
#include <QMap>
#include <QVector>
#include <QPointF>
#include "DAQSettingsDialog/DAQSettingsDialog.h"

class DAQReader : public QThread
{
   Q_OBJECT

public:
   DAQReader();
   int appendData(QMap<int, QVector<QPointF> >* curveMap);
   void stop();    

signals:
   void newData();
   void daqError(const QString& errorMessage);
   void startedRecording();
   void stoppedRecording();

public:
   void run();
   void updateDAQSettings(const DAQSettings& settings);

protected:
   bool DAQCheckHandler(const char* cmd, int error);

   enum { maxChannels = 8, maxScansPerSecond = 35000 };
    
   volatile bool shouldStop;
    
   int numChannels;
   double dt;
   double Vmins[maxChannels], Vmaxes[maxChannels];

   QVector<qreal> newDataBuffer[maxChannels];
   QMutex mutex;
};

#endif
