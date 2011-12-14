#include <QApplication>
#include "DAQSettingsDialog.h"
#include <stdio.h>

int main(int argc, char** argv) 
{
   QApplication app(argc, argv);
   DAQSettings settings;
   settings.restore();
   DAQSettingsDialog *dialog = new DAQSettingsDialog(settings);
   dialog->show();
   
   int result = app.exec();

   if (dialog->result() == QDialog::Accepted) {
      printf("OK!\n");
      dialog->settings.save();
   }
   else {
      printf("Canceled!\n");
   }
   return result;
}
