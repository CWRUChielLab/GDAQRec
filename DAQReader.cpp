#include <QtGui>
#include <cmath>

#include "DAQReader.h"

#if defined(USE_NIDAQMXBASE)
   #include <NIDAQmxBase.h>
#elif defined(USE_COMEDI)
   #include <comedilib.h>
#else
   #error No DAQ library was defined!
#endif

#define DAQCheck(x) DAQCheckHandler(#x,x)


DAQReader::DAQReader() : 
   shouldStop(false),
   numChannels(1),
   dt(0.01),
   mutex(QMutex::Recursive)
{
   Vmins[0] = -10.0;
   Vmaxes[0] = 10.0;
}


void DAQReader::updateDAQSettings(const DAQSettings& settings)
{
   numChannels = settings.numChannels;
   dt = 1.0/settings.samplingRate;

   for (int chan = 0; chan < numChannels; ++chan) {
      Vmins[chan] = settings.minVoltage[chan];
      Vmaxes[chan] = settings.maxVoltage[chan];
   }
 }


int DAQReader::appendData(QMap<int, QVector<QPointF> >* curveMap) 
{
   QMutexLocker lock(&mutex);
   int numScans = newDataBuffer[0].count();

   double tStart = ((*curveMap)[0].isEmpty() 
      ? 0.0 : ((*curveMap)[0].last().x() + dt));

   for (int chan = 0; chan < numChannels; ++chan) {
      QVector<QPointF>& points = (*curveMap)[chan];
      points.reserve(points.count() + newDataBuffer[chan].count());

      for (int scan = 0; scan < newDataBuffer[chan].count(); ++scan) {
         points.push_back(QPointF(
                  qreal(tStart + scan*dt),
                  newDataBuffer[chan][scan]
                  ));
      }

      newDataBuffer[chan].clear();
   }

   return numScans;
}


void DAQReader::stop() 
{
   shouldStop=true;
}  

#ifdef USE_NIDAQMXBASE

void DAQReader::run()
{
   TaskHandle recordingTask = NULL;

   char channelNames[256];
   sprintf(channelNames, "Dev1/ai0:%d", numChannels-1);
   
   if (
         DAQCheck( DAQmxBaseCreateTask("",&recordingTask) )
         && DAQCheck( DAQmxBaseCreateAIVoltageChan(
               recordingTask, channelNames, NULL, 
               DAQmx_Val_Diff, Vmins[0], Vmaxes[0], DAQmx_Val_Volts, NULL))
         && DAQCheck( DAQmxBaseCfgSampClkTiming(
               recordingTask, "OnboardClock", int(1/dt), 
               DAQmx_Val_Rising, DAQmx_Val_ContSamps, 0)) 
         && DAQCheck(DAQmxBaseStartTask(recordingTask))
      ) {

      const int updateInterval = 10; // ms
      float64 buffer[maxChannels*maxScansPerSecond*updateInterval/1000];
      const int scansPerRead = int(1/dt)*updateInterval/1000; 
      const float64 timeout = 0.1; // seconds

      emit startedRecording();
      
      while (!shouldStop) {
         int32 numScansRead;
         
         // HACK: because we need to run this on the GUI thread on the mac,
         // it may take longer than our update interval to run the emit call
         // and thus we may need to read more than one update's worth of 
         // data per emit to avoid having large amounts of data queue up.  
         // The only way to find out if there's queued data is to do a read
         // with a timeout of zero and see if you get a timeout error, so 
         // that's what we do.  

         float64 nextTimeout = timeout;
         bool emitNewData = false;
         int daqReadError;

         while (0 == (daqReadError = DAQmxBaseReadAnalogF64(
               recordingTask, scansPerRead, nextTimeout, 
               DAQmx_Val_GroupByScanNumber, buffer,
               scansPerRead*numChannels, &numScansRead, NULL
            ))) {

            QMutexLocker lock(&mutex);
            
            nextTimeout = 0;
            emitNewData = true;

            for (int chan = 0; chan < numChannels; ++chan) {
               newDataBuffer[chan].reserve(
                     newDataBuffer[chan].count() + numScansRead
                     );
            }

            for (int scan = 0; scan < numScansRead; ++scan) {
               for (int chan = 0; chan < numChannels; ++chan) {
                  newDataBuffer[chan].push_back(
                     buffer[scan*numChannels + chan]
                     );
               }
            }
         }
         
         if (emitNewData)
             emit newData();
         
         const int32 timeoutError = -200284;
         if (daqReadError != timeoutError) 
            DAQCheck(daqReadError);
      }

      shouldStop = false;
      
      emit stoppedRecording();
   }

   if (recordingTask != NULL) {
      DAQCheck( DAQmxBaseStopTask(recordingTask) );
      DAQCheck( DAQmxBaseClearTask(recordingTask) );
   }
}


bool DAQReader::DAQCheckHandler(const char* cmd, int error)
{
   if( DAQmxFailed(error) ) {
      
      shouldStop = true;

      int messageBufLength = DAQmxBaseGetExtendedErrorInfo(NULL,0);
      char* messageBuf = new char[messageBufLength];

      DAQmxBaseGetExtendedErrorInfo(messageBuf, messageBufLength);

      emit daqError(
            QString::number(error) + tr(": \"") + QString(messageBuf) 
             + tr("\"\nWhile processing command:\n\"") + QString(cmd)
            );

      delete messageBuf;

      return false;
   }
   else
   {
      return true;
   }
}
#endif

#ifdef USE_COMEDI

void DAQReader::run()
{
   comedi_t *dev;
   comedi_cmd c,*cmd=&c;
   unsigned int chanlist[maxChannels];
   int aref = AREF_DIFF;
   int subdevice = 0;
   const int updateInterval = 100; // ms

	const int targetSamplingRate = 250000/numChannels;
	int overSampling = std::max(1, int(targetSamplingRate*dt));

   dev = comedi_open("/dev/comedi0");
   
   if(!dev){
      DAQCheckHandler("comedi_open",-1); 
   }
   else {
      int range[maxChannels];
      comedi_range* crange[maxChannels];
      int maxdata[maxChannels];

      /* Set up channel list */
      for(int chan=0; chan<numChannels; ++chan){
         range[chan] = comedi_find_range(dev, subdevice, chan, 
               UNIT_volt, Vmins[chan], Vmaxes[chan]);
         crange[chan] = comedi_get_range(dev, subdevice, chan, range[chan]);
         maxdata[chan] = comedi_get_maxdata(dev, subdevice, chan);

         chanlist[chan]=CR_PACK(chan,range[chan],aref);
      }

      memset(cmd,0,sizeof(*cmd));

      if (DAQCheck(
               comedi_get_cmd_generic_timed(dev,subdevice,cmd,
                  numChannels,
                  (unsigned int)(1e9*dt/overSampling)))
            ) {


         /* Modify parts of the command */
         cmd->chanlist     = chanlist;
         cmd->chanlist_len = numChannels;
         cmd->convert_arg /= numChannels;

         cmd->scan_end_arg = numChannels;
         cmd->stop_src=TRIG_NONE;
         cmd->stop_arg=0;

         int secondComediTestResult;

         if (DAQCheck(comedi_command_test(dev,cmd))
               && DAQCheck( secondComediTestResult = 
                  comedi_command_test(dev, cmd) )
               && DAQCheck( (secondComediTestResult == 0) ? 0 : -1 )
               && DAQCheck( comedi_command(dev, cmd) )) {
      
            // make sure we have an accurate sampling rate
            dt = cmd->scan_begin_arg*1.0e-9*overSampling;

            emit startedRecording();

            const int bufferSize = 
               maxChannels*maxScansPerSecond/1000*updateInterval;
            sampl_t buffer[bufferSize];
            int bytesRead;
            int nextChan = 0;
            bool stopping = false;
				double overSampleSum[maxChannels];
				int overSampleCount = 0;

				for (int chan = 0; chan < numChannels; ++chan) {
					overSampleSum[chan] = 0.0;
				}

            while (DAQCheck(bytesRead = 
                     read(comedi_fileno(dev),buffer, bufferSize))
                  && bytesRead > 0) {

               if (shouldStop) {
                  shouldStop = false;
                  stopping = true;
                  comedi_cancel(dev, subdevice);
               }

               {
                  QMutexLocker lock(&mutex);
                  
                  DAQCheck((bytesRead & 1) ? -1 : 0); // no partial samples!

                  int numScansRead = bytesRead/sizeof(sampl_t)/numChannels;

                  for (int chan = 0; chan < numChannels; ++chan) {
                     newDataBuffer[chan].reserve(
                           newDataBuffer[chan].count() 
                            + numScansRead/overSampling + 1
                           );
                  }

                  for (unsigned i = 0; i < bytesRead/sizeof(sampl_t); ++i, 
                        nextChan = (nextChan+1)%numChannels) {
							overSampleSum[nextChan] += comedi_to_phys(
									buffer[i], crange[nextChan], maxdata[nextChan]
									);

							if (nextChan == numChannels-1) {
								++overSampleCount;

								if (overSampleCount == overSampling) {
									overSampleCount = 0;
									for (int chan = 0; chan < numChannels; ++chan) {
                     			newDataBuffer[chan].push_back(
                                    overSampleSum[chan]/overSampling);
										overSampleSum[chan] = 0.0;
									}
								}
							}
                  }
               }

               emit newData();       
            }

            emit stoppedRecording();
         }         
      }

      comedi_close(dev);
   }
}

bool DAQReader::DAQCheckHandler(const char* cmd, int error)
{
   if( error < 0 ) {
      
      shouldStop = true;

      int errorNumber = comedi_errno();
      QString message = QString(comedi_strerror(errorNumber));

      emit daqError(
            QString::number(errorNumber) + tr(": \"") + message 
             + tr("\"\nWhile processing command:\n\"") + QString(cmd)
            );

      return false;
   }
   else
   {
      return true;
   }
}

#endif
