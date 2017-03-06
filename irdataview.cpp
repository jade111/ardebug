/* irdataview.cpp
 * This class encapsulates the UI panel for displaying a robot's IR sensor data
 *
 * (C) Alistair Jewers, Feb 2017
 */

#include "irdataview.h"

#include <QPainter>

// Default constructor
IRDataView::IRDataView(QWidget *parent) : QWidget(parent) { }

/* Constructor
 * Sets up the data model pointer
 */
IRDataView::IRDataView(DataModel *dataModelRef) {
    this->dataModelRef = dataModelRef;
}

/* paintEvent
 * Override. Called to re-draw the widget.
 */
void IRDataView::paintEvent(QPaintEvent*) {
    // Geometries for the bars
    int maxHeight = this->size().height() - 60;
    int baseLine = this->size().height() - 40;

    // Clear the panel
    QPainter painter(this);
    painter.fillRect( 0, 0, this->size().width(), this->size().height(), Qt::white);

    // If a robot is selected display its data
    if (dataModelRef->selectedRobotID != -1) {
        RobotData* robot = dataModelRef->getRobotByID(dataModelRef->selectedRobotID);

        // Robot name
        painter.drawText(10, 2, 300, 20, 0, robot->getName());

        // Draw a bar for each of the IR sensors on the robot
        for (int i = 0; i < PROX_SENS_COUNT; i++) {
            int raw = robot->getProximitySensorData(i);
            int background = robot->getBackgroundIR(i);

            int barHeight[] = { (int)(maxHeight * ((float)raw/4095.0f)), (int)(maxHeight * ((float)background/4095.0f))};

            painter.fillRect(60 + i * 100, baseLine, 20, -barHeight[0], Qt::red);
            painter.fillRect(80 + i * 100, baseLine, 20, -barHeight[1], Qt::blue);

            // Add the raw data value beneath
            painter.drawText(60 + i * 100, baseLine + 20, 100, 20, 0, QString::number(raw) + " / " + QString::number(background));
        }
    }

    painter.end();
}
