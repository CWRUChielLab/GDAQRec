#include <QtGui>
#include <cmath>

#include "plotter.h"

using namespace std;

Plotter::Plotter(QWidget *parent) :
#ifdef Q_WS_MAC
    recording(false),
#endif
    QWidget(parent)
{
    daqSettings.restore();
    daqReader.updateDAQSettings(daqSettings);

    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);
    rubberBandIsShown = false;

    newButton = new QToolButton(this);
    newButton->setText("New");
    newButton->adjustSize();
    connect(newButton, SIGNAL(clicked()), this, SLOT(newDocument()));

    openButton = new QToolButton(this);
    openButton->setText("Open");
    openButton->adjustSize();
    connect(openButton, SIGNAL(clicked()), this, SLOT(open()));

    saveButton = new QToolButton(this);
    saveButton->setText("Save");
    saveButton->adjustSize();
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));

    settingsButton = new QToolButton(this);
    settingsButton->setText("Settings");
    settingsButton->adjustSize();
    connect(settingsButton, SIGNAL(clicked()), this, SLOT(settings()));

    recordButton = new QToolButton(this);
    recordButton->setIcon(QIcon(":/images/record.png"));
    recordButton->adjustSize();
    connect(recordButton, SIGNAL(clicked()), this, SLOT(toggleRecording()));

    zoomInButton = new QToolButton(this);
    zoomInButton->setIcon(QIcon(":/images/zoomin.png"));
    zoomInButton->adjustSize();
    connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));

    zoomOutButton = new QToolButton(this);
    zoomOutButton->setIcon(QIcon(":/images/zoomout.png"));
    zoomOutButton->adjustSize();
    connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));

    connect(&daqReader, SIGNAL(newData()), this, SLOT(newData()));
    connect(&daqReader, SIGNAL(daqError(const QString&)), this,
          SLOT(daqError(const QString&)));
    connect(&daqReader, SIGNAL(startedRecording()), this, SLOT(startedRecording()));
#ifdef Q_WS_MAC
    connect(&daqReader, SIGNAL(stoppedRecording()), this, SLOT(stoppedRecording()));
#else
    connect(&daqReader, SIGNAL(finished()), this, SLOT(stoppedRecording()));
#endif
    startTime = QDateTime::currentDateTimeUtc();
    saved = true;
    traceOffset = 0.0;

    clearPlot();
}

void Plotter::clearPlot()
{
    zoomStack.clear();

    PlotSettings settings;
    settings.minY = -10;
    settings.maxY = 10;

    zoomStack.append(settings);
    zoomStack.append(settings); // start zoomed in, since the top zoom shows the entire waveform
    curZoom = 1;

    // make the top level zoom as wide as the data
    if (!curveMap[0].isEmpty()
          && zoomStack[0].maxX < curveMap[0].last().x()) {
       zoomStack[0].maxX = curveMap[0].last().x();
    }

    zoomInButton->hide();
    zoomOutButton->show();
    refreshPixmap();
}

void Plotter::zoomOut()
{
    if (curZoom > 0) {
        --curZoom;
        zoomOutButton->setEnabled(curZoom > 0);
        zoomInButton->setEnabled(true);
        zoomInButton->show();
        refreshPixmap();

        // if this view was sliding right as the trace grew, keep it on the right
        // side of the trace.
        if (zoomStack[curZoom].includesRightEdge) {
           double newMaxX = curveMap[0].empty()
                  ? zoomStack[curZoom].maxX : curveMap[0].last().x();
           double dx = newMaxX - zoomStack[curZoom].maxX;
           zoomStack[curZoom].minX += dx;
           zoomStack[curZoom].maxX += dx;
           zoomStack[curZoom].includesRightEdge = true;
        }
    }
}

void Plotter::zoomIn()
{
    if (curZoom < zoomStack.count() - 1) {
        ++curZoom;
        zoomInButton->setEnabled(curZoom < zoomStack.count() - 1);
        zoomOutButton->setEnabled(true);
        zoomOutButton->show();
        refreshPixmap();

        // if this view was sliding right as the trace grew, keep it on the right
        // side of the trace.
        if (zoomStack[curZoom].includesRightEdge) {
           double newMaxX = curveMap[0].empty()
                  ? zoomStack[curZoom].maxX : curveMap[0].last().x();
           double dx = newMaxX - zoomStack[curZoom].maxX;
           zoomStack[curZoom].minX += dx;
           zoomStack[curZoom].maxX += dx;
           zoomStack[curZoom].includesRightEdge = true;
        }
     }
}

void Plotter::startedRecording()
{
   recordButton->setIcon(QIcon(":/images/stop.png"));
   recordButton->setEnabled(true);
}

void Plotter::stoppedRecording()
{
   recordButton->setIcon(QIcon(":/images/record.png"));
   recordButton->setEnabled(true);
   settingsButton->setEnabled(true);
}

void Plotter::toggleRecording()
{
#ifdef Q_WS_MAC
   if (recording) {
#else
   if (daqReader.isRunning()) {
#endif
      recordButton->setEnabled(false);
      daqReader.stop();
   }
   else {
      recordButton->setEnabled(false);
      settingsButton->setEnabled(false);
#ifdef Q_WS_MAC
      recording = true;
      daqReader.run();
      recording = false;
#else
      daqReader.start();
#endif
  }
}

bool Plotter::offerToSave()
{
   if (!saved) {
      int result = QMessageBox::warning(this, tr("GDAQrec"),
            tr("There are unsaved data.\nWould you like to save them?"),
            QMessageBox::Save | QMessageBox::Default,
            QMessageBox::Discard,
            QMessageBox::Cancel | QMessageBox::Escape);

      switch (result) {
         case QMessageBox::Save:
            save();
            if (saved) {
               return true;
            }
            else {
               return false;
            }

         case QMessageBox::Discard:
            return true;

         case QMessageBox::Cancel:
            return false;

         // should never be reached...
         default:
            return false;
      }
   }
   else {
      return true;
   }
 }

void Plotter::newDocument()
{
   if (offerToSave()) {
      saved = true;
      filename.clear();
      curveMap.clear();
      clearPlot();
   }
}

void Plotter::open()
{
   if (offerToSave()) {
      QString newFilename = QFileDialog::getOpenFileName(
            this, tr("Open data"), QString(),
            tr("Data files (*.csv);;All Files (*)"));

      if (!newFilename.isEmpty()) {
         QFile file(newFilename);

         if (!file.open(QIODevice::ReadOnly)) {
             QMessageBox::critical(this, tr("GDAQrec"),
                tr("Could not open file ") + newFilename,
                QMessageBox::Ok | QMessageBox::Default
             );
         }
         else
         {
             saved = true;
             filename = newFilename;
             curveMap.clear();

             QTextStream in(&file);

             while (!in.atEnd()) {
               QString line = in.readLine();
               QStringList coords = line.split(',', QString::SkipEmptyParts);

               // TODO: real error checking/recovery
               if (coords.count() >= 2) {
                  double time = coords[0].toDouble();
                  for (int i=1; i < coords.size(); ++i) {
                     curveMap[i-1].append(QPointF(time, coords[i].toDouble()));
                  }
               }
             }

             clearPlot();
         }
      }
   }
}

void Plotter::save()
{
   if (filename.isEmpty()) {
      QString suggestedFilename = (startTime.toString(Qt::ISODate) + ".csv");
      suggestedFilename.remove(':');
      filename = QFileDialog::getSaveFileName(this, tr("Save data"),
          suggestedFilename,
          tr("Data files (*.csv);;All Files (*)"));
   }

   if (!filename.isEmpty()) {
      FILE* file = fopen(filename.toAscii(), "w");

      if (file != NULL) {
         int maxScans = curveMap[0].count()-1;
         // the -1 is to ignore partial scans on comedi

         for (int scan = 0; scan < maxScans; ++scan) {
            fprintf(file, "%.6f", curveMap[0][scan].x());

            for (CurveMap::iterator chanIter = curveMap.begin();
                  chanIter != curveMap.end(); ++chanIter) {
               fprintf(file, ",%.6f", (*chanIter)[scan].y());
            }

            fprintf(file, "\n");
         }

         fclose(file);
         saved = true;
      }
      else {
         filename = QString();

         int result = QMessageBox::critical(this, tr("GDAQrec"),
            tr("I was unable to create the file \"")
             + filename + tr("\"\n") +
            tr("Would you like to try again with a new file name?"),
            QMessageBox::Yes | QMessageBox::Default,
            QMessageBox::No | QMessageBox::Escape);

         if (result == QMessageBox::Yes) {
            save();
         }
      }
   }
}

void Plotter::settings()
{
   DAQSettingsDialog dialog(daqSettings, this);

   if (dialog.exec()) {
      daqSettings = dialog.settings;
      daqSettings.save();
      updateSettings();
   }
}

class UpdateTimer
{
   QTime timer;
public:
   UpdateTimer() {
      timer.start();
   }

   bool shouldSkip() {
      if (timer.elapsed() > 100) {
         timer.restart();
         return false;
      }
      else {
         return true;
      }
   }
} updateTimer;

void Plotter::newData()
{
   if (updateTimer.shouldSkip())
      return;

   double oldMaxX = curveMap[0].empty()
      ? zoomStack[curZoom].maxX : curveMap[0].last().x();

   int numScansRead = daqReader.appendData(&curveMap);

   if (numScansRead > 0) {
      if (saved) {
          startTime = QDateTime::currentDateTimeUtc();
          saved = false;
      }

      // expand the top level zoom, if needed.
      if (zoomStack[0].maxX < curveMap[0].last().x()) {
         zoomStack[0].maxX = curveMap[0].last().x();
      }


      // scroll right if this causes the plot to go from on the page to off of
      // the page
      double newMaxX = curveMap[0].empty()
         ? zoomStack[curZoom].maxX : curveMap[0].last().x();
      if (zoomStack[curZoom].minX <= oldMaxX
          && oldMaxX <= zoomStack[curZoom].maxX
          && newMaxX > zoomStack[curZoom].maxX
          )
      {
         double dx = newMaxX - zoomStack[curZoom].maxX;
         zoomStack[curZoom].minX += dx;
         zoomStack[curZoom].maxX += dx;
         zoomStack[curZoom].includesRightEdge = true;
      }
      else
      {
         zoomStack[curZoom].includesRightEdge = false;
      }

      refreshPixmap();
   }
}

void Plotter::daqError(const QString& errorMessage)
{
   QMessageBox::critical(this, tr("GDAQrec"),
      tr("The following DAQ error occured:\n") + errorMessage,
      QMessageBox::Ok | QMessageBox::Default
      );
}

QSize Plotter::minimumSizeHint() const
{
    return QSize(6 * Margin, 4 * Margin);
}

QSize Plotter::sizeHint() const
{
    return QSize(12 * Margin, 8 * Margin);
}

void Plotter::paintEvent(QPaintEvent * /* event */)
{
    QStylePainter painter(this);
    painter.drawPixmap(0, 0, pixmap);

    if (rubberBandIsShown) {
        painter.setPen(daqSettings.fgColor);
        painter.drawRect(rubberBandRect.normalized()
                                       .adjusted(0, 0, -1, -1));
    }

    if (hasFocus()) {
        QStyleOptionFocusRect option;
        option.initFrom(this);
        option.backgroundColor = daqSettings.bgColor;
        painter.drawPrimitive(QStyle::PE_FrameFocusRect, option);
    }
}

void Plotter::resizeEvent(QResizeEvent * /* event */)
{
    const int gap = 5; // pixels
    int x = width() - (
                     newButton->width() + gap
                     + openButton->width() + gap
                     + saveButton->width() + gap
                     + settingsButton->width() + gap
                     + recordButton->width() + gap
                     + zoomInButton->width() + gap
                     + zoomOutButton->width() + gap);

    newButton->move(x, gap);
    x += newButton->width() + gap;
    openButton->move(x, gap);
    x += openButton->width() + gap;
    saveButton->move(x, gap);
    x += saveButton->width() + gap;
    settingsButton->move(x, gap);
    x += settingsButton->width() + gap;
    recordButton->move(x, gap);
    x += recordButton->width() + gap;
    zoomInButton->move(x, gap);
    x += zoomInButton->width() + gap;
    zoomOutButton->move(x, gap);
    x += zoomOutButton->width() + gap;

    refreshPixmap();
}

void Plotter::mousePressEvent(QMouseEvent *event)
{
    QRect rect(Margin, Margin,
               width() - 2 * Margin, height() - 2 * Margin);

    if (event->button() == Qt::LeftButton) {
        if (rect.contains(event->pos())) {
            rubberBandIsShown = true;
            rubberBandRect.setTopLeft(event->pos());
            rubberBandRect.setBottomRight(event->pos());
            updateRubberBandRegion();
            setCursor(Qt::CrossCursor);
        }
    }
}

void Plotter::mouseMoveEvent(QMouseEvent *event)
{
    if (rubberBandIsShown) {
        updateRubberBandRegion();
        rubberBandRect.setBottomRight(event->pos());
        updateRubberBandRegion();
    }
}

void Plotter::mouseReleaseEvent(QMouseEvent *event)
{
    if ((event->button() == Qt::LeftButton) && rubberBandIsShown) {
        rubberBandIsShown = false;
        updateRubberBandRegion();
        unsetCursor();

        QRect rect = rubberBandRect.normalized();
        if (rect.width() < 4 || rect.height() < 4)
            return;
        rect.translate(-Margin, -Margin);

        PlotSettings prevSettings = zoomStack[curZoom];
        PlotSettings settings;
        double dx = prevSettings.spanX() / (width() - 2 * Margin);
        double dy = prevSettings.spanY() / (height() - 2 * Margin);
        settings.minX = prevSettings.minX + dx * rect.left();
        settings.maxX = prevSettings.minX + dx * rect.right();
        settings.minY = prevSettings.maxY - dy * rect.bottom();
        settings.maxY = prevSettings.maxY - dy * rect.top();
        settings.adjust();

        zoomStack.resize(curZoom + 1);
        zoomStack.append(settings);
        zoomIn();
    }
}

void Plotter::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Left:
        zoomStack[curZoom].scroll(-1, 0);
        refreshPixmap();
        break;
    case Qt::Key_Right:
        zoomStack[curZoom].scroll(+1, 0);
        refreshPixmap();
        break;
    case Qt::Key_Down:
        zoomStack[curZoom].scroll(0, -1);
        refreshPixmap();
        break;
    case Qt::Key_Up:
        zoomStack[curZoom].scroll(0, +1);
        refreshPixmap();
        break;
    case Qt::Key_S:
        if (traceOffset == 0.0) {
            traceOffset = 0.1;
        } else {
            traceOffset = 0.0;
        }
        refreshPixmap();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void Plotter::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numTicks = numDegrees / 15;

    if (event->orientation() == Qt::Horizontal) {
        zoomStack[curZoom].scroll(numTicks, 0);
    } else {
        zoomStack[curZoom].scroll(0, numTicks);
    }
    refreshPixmap();
}

void Plotter::closeEvent(QCloseEvent* event)
{
   if (offerToSave())
      event->accept();
   else
      event->ignore();

  #ifdef Q_WS_MAC
   daqReader.stop();
#endif
   if (event->isAccepted() && daqReader.isRunning()) {
      daqReader.stop();
      daqReader.wait();
   }
}

void Plotter::updateRubberBandRegion()
{
    QRect rect = rubberBandRect.normalized();
    update(rect.left(), rect.top(), rect.width(), 1);
    update(rect.left(), rect.top(), 1, rect.height());
    update(rect.left(), rect.bottom(), rect.width(), 1);
    update(rect.right(), rect.top(), 1, rect.height());
}

void Plotter::refreshPixmap()
{
    pixmap = QPixmap(size());
    pixmap.fill(this, 0, 0);
    pixmap.fill(daqSettings.bgColor);

    QPainter painter(&pixmap);
    painter.initFrom(this);
    drawGrid(&painter);
    drawCurves(&painter);
    update();
}

void Plotter::drawGrid(QPainter *painter)
{
    QRect rect(Margin, Margin,
               width() - 2 * Margin, height() - 2 * Margin);
    if (!rect.isValid())
        return;

    PlotSettings settings = zoomStack[curZoom];
    QPen quiteDark = QColor(
          (daqSettings.fgColor.red() + daqSettings.bgColor.red())/2,
          (daqSettings.fgColor.green() + daqSettings.bgColor.green())/2,
          (daqSettings.fgColor.blue() + daqSettings.bgColor.blue())/2
          );
    QPen light = daqSettings.fgColor;

    for (int i = 0; i <= settings.numXTicks; ++i) {
        int x = rect.left() + (i * (rect.width() - 1)
                                 / settings.numXTicks);
        double label = settings.minX + (i * settings.spanX()
                                          / settings.numXTicks);
        painter->setPen(quiteDark);
        painter->drawLine(x, rect.top(), x, rect.bottom());
        painter->setPen(light);
        painter->drawLine(x, rect.bottom(), x, rect.bottom() + 5);
        painter->drawText(x - 50, rect.bottom() + 5, 100, Margin,
                          Qt::AlignHCenter | Qt::AlignTop,
                          QString::number(label));
    }
    for (int j = 0; j <= settings.numYTicks; ++j) {
        int y = rect.bottom() - (j * (rect.height() - 1)
                                   / settings.numYTicks);
        double label = settings.minY + (j * settings.spanY()
                                          / settings.numYTicks);
        painter->setPen(quiteDark);
        painter->drawLine(rect.left(), y, rect.right(), y);
        painter->setPen(light);
        painter->drawLine(rect.left() - 5, y, rect.left(), y);
        painter->drawText(rect.left() - Margin, y - 10, Margin - 5, 20,
                          Qt::AlignRight | Qt::AlignVCenter,
                          QString::number(label));
    }
    painter->drawRect(rect.adjusted(0, 0, -1, -1));
}

void Plotter::drawCurves(QPainter *painter)
{
    PlotSettings settings = zoomStack[curZoom];
    QRect rect(Margin, Margin,
               width() - 2 * Margin, height() - 2 * Margin);
    if (!rect.isValid())
        return;

    painter->setClipRect(rect.adjusted(+1, +1, -1, -1));

    QMapIterator<int, QVector<QPointF> > i(curveMap);
    double offset = 0.0;
    while (i.hasNext()) {
        i.next();

        int id = i.key();
        const QVector<QPointF> &data = i.value();

         if (data.count() != 0) {
            QVector<QPointF> points;

            int j;

            // skip points off the left edge of the graph
            // (use a binary search, since there may be a lot of data)
            int lowbound = 0;
            int highbound = data.count()-1;

            while (lowbound + 1 < highbound) {
              int mid = (lowbound + highbound)/2;

              if (data[mid].x() < settings.minX) {
                 lowbound = mid;
              }
              else {
                 highbound = mid;
              }
            }
            j = max(0, lowbound-1);

            // since there can be many points per pixel, just draw a line
            // from the minumum in that pixel to the maximum in that pixel
            // (and then to the next pixel) (This speeds up drawing
            // dramatically)
            int prevX = rect.left()-2;
            int minY = 0, maxY = 0; // reinitialized below
            bool firstPoint = true;

            for (; j < data.count()
                  && (j == 0 || data[j-1].x() < settings.maxX); ++j) {
               double dx = data[j].x() - settings.minX;
               double dy = data[j].y() - settings.minY + offset;
               double x = rect.left() + (dx * (rect.width() - 1)
                                            / settings.spanX());
               double y = rect.bottom() - (dy * (rect.height() - 1)
                                              / settings.spanY());
               if (firstPoint) {
                  minY = maxY = (int)y;
                  firstPoint = false;
               }

               if (int(x) != prevX) {
                  points.append(QPointF(x,minY));
                  points.append(QPointF(x,maxY));

                  prevX = int(x);
                  minY = maxY = int(y);
               }
               else {
                  minY = min(int(y), minY);
                  maxY = max(int(y), maxY);
               }
            }

            QPolygonF polyline(points);

            painter->setPen(daqSettings.color[uint(id) % 8]);
            painter->drawPolyline(polyline);

        }
        offset -= traceOffset;
    }
}

void Plotter::updateSettings()
{
   daqReader.updateDAQSettings(daqSettings);
   refreshPixmap();
}

PlotSettings::PlotSettings()
{
    minX = 0.0;
    maxX = 2.0;
    numXTicks = 5;

    minY = -2.0;
    maxY = 2.0;
    numYTicks = 5;

    includesRightEdge = false;
}

void PlotSettings::scroll(double dx, double dy)
{
    double stepX = spanX() / numXTicks;
    minX += dx * stepX;
    maxX += dx * stepX;

    double stepY = spanY() / numYTicks;
    minY += dy * stepY;
    maxY += dy * stepY;
}

void PlotSettings::adjust()
{
    adjustAxis(minX, maxX, numXTicks);
    adjustAxis(minY, maxY, numYTicks);
}

void PlotSettings::adjustAxis(double &min, double &max,
                              int &numTicks)
{
    const int MinTicks = 4;
    double grossStep = (max - min) / MinTicks;
    double step = pow(10.0, floor(log10(grossStep)));

    if (5 * step < grossStep) {
        step *= 5;
    } else if (2 * step < grossStep) {
        step *= 2;
    }

    numTicks = int(ceil(max / step) - floor(min / step));
    if (numTicks < MinTicks)
        numTicks = MinTicks;
    min = floor(min / step) * step;
    max = ceil(max / step) * step;
}


