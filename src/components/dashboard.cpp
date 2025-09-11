#include "dashboard.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTime>
#include <QRandomGenerator>
#include <QFile>

Dashboard::Dashboard(QWidget *parent) : QWidget(parent)
{
    currentSpeed = 0;       // km/h
    maxSpeed = 240;         // km/h
    batteryLevel = 0.85;    // %
    range = 320;            // km
    powerConsumption = 168; // Wh/km
    outsideTemp = 22;       // °C
    isPoweredOn = true;
    driveMode = "NORMAL";

    primaryColor = QColor(0, 150, 255);     // Electric blue
    secondaryColor = QColor(240, 240, 240); // Light gray
    alertColor = QColor(255, 80, 80);       // Red for alerts

    QFile styleFile(":/styles/dashboard.css");
    if (styleFile.open(QFile::ReadOnly | QFile::Text))
    {
        QString style = styleFile.readAll();
        this->setStyleSheet(style);
        styleFile.close();
    }
    else
    {
        qDebug() << "无法打开dashboard.css文件:" << styleFile.errorString();
    }

    setupUI();

    // Timer to simulate data updates
    dataTimer = new QTimer(this);
    connect(dataTimer, &QTimer::timeout, this, &Dashboard::updateData);
    dataTimer->start(1000); // 1s
}

Dashboard::~Dashboard()
{
}

void Dashboard::paintEvent(QPaintEvent *event)
{
}

void Dashboard::updateData()
{
    // Simulate data changes
    if (isPoweredOn)
    {
        // Simulate speed changes
        static bool increasing = true;
        if (increasing)
        {
            currentSpeed += 2;
            if (currentSpeed >= 120)
                increasing = false;
        }
        else
        {
            currentSpeed -= 3;
            if (currentSpeed <= 20)
                increasing = true;
        }

        // Simulate battery drain
        batteryLevel -= 0.0005;
        if (batteryLevel < 0)
            batteryLevel = 0;

        // Update range based on battery and consumption
        range = static_cast<int>(batteryLevel * 500 * (150.0 / powerConsumption));

        powerConsumption = 100 + currentSpeed / 2 + QRandomGenerator::global()->bounded(20);

        // Update time
        timeLabel->setText(QTime::currentTime().toString("h:mm AP"));
    }

    // Update UI
    speedValueLabel->setText(QString::number(currentSpeed));
    batteryPercentageLabel->setText(QString::number(batteryLevel * 100, 'f', 0) + "%");
    batteryBar->setValue(batteryLevel * 100);
    rangeLabel->setText(QString::number(range) + " km");

    update();
}

void Dashboard::togglePower()
{
    isPoweredOn = !isPoweredOn;
    powerButton->setText(isPoweredOn ? "POWER ON" : "POWER OFF");
    powerButton->setStyleSheet(
        "QPushButton {"
        "  background-color: " +
        (isPoweredOn ? primaryColor.name() : "#444") + ";"
                                                       "  color: white;"
                                                       "  border: none;"
                                                       "  border-radius: 5px;"
                                                       "  font-weight: bold;"
                                                       "}"
                                                       "QPushButton:pressed {"
                                                       "  background-color: #0080ff;"
                                                       "}");

    if (!isPoweredOn)
    {
        currentSpeed = 0;
        speedValueLabel->setText("0");
    }
}

void Dashboard::changeDriveMode()
{
    if (driveMode == "NORMAL")
    {
        driveMode = "ECO";
        primaryColor = QColor(0, 200, 0); // Green for eco mode
    }
    else if (driveMode == "ECO")
    {
        driveMode = "SPORT";
        primaryColor = QColor(255, 80, 80); // Red for sport mode
    }
    else
    {
        driveMode = "NORMAL";
        primaryColor = QColor(0, 150, 255); // Blue for normal mode
    }

    driveModeLabel->setText(driveMode);
    driveModeLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: " + primaryColor.name() + ";");
    driveModeButton->setText("MODE: " + driveMode);

    // Update range based on mode
    if (driveMode == "ECO")
    {
        range = static_cast<int>(range * 1.2);
    }
    else if (driveMode == "SPORT")
    {
        range = static_cast<int>(range * 0.8);
    }

    rangeLabel->setText(QString::number(range) + " km");

    // Request repaint to update colors
    update();
}

void Dashboard::setupUI()
{
    setFixedSize(800, 480);
    setStyleSheet("background-color: black; color: white;");

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(10);

    // Top info bar
    QHBoxLayout *topLayout = new QHBoxLayout();

    timeLabel = new QLabel("10:30 AM");
    timeLabel->setAlignment(Qt::AlignLeft);
    timeLabel->setStyleSheet("font-size: 16px;");

    driveModeLabel = new QLabel(driveMode);
    driveModeLabel->setAlignment(Qt::AlignCenter);
    driveModeLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: " + primaryColor.name() + ";");

    tempLabel = new QLabel(QString::number(outsideTemp) + "°C");
    tempLabel->setAlignment(Qt::AlignRight);
    tempLabel->setStyleSheet("font-size: 16px;");

    topLayout->addWidget(timeLabel);
    topLayout->addWidget(driveModeLabel);
    topLayout->addWidget(tempLabel);

    // Center dashboard
    QHBoxLayout *centerLayout = new QHBoxLayout();

    // Left panel - Battery info
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->setAlignment(Qt::AlignCenter);

    QLabel *batteryTitle = new QLabel("BATTERY");
    batteryTitle->setAlignment(Qt::AlignCenter);
    batteryTitle->setStyleSheet("font-size: 14px; color: gray;");

    batteryPercentageLabel = new QLabel(QString::number(batteryLevel * 100, 'f', 0) + "%");
    batteryPercentageLabel->setAlignment(Qt::AlignCenter);
    batteryPercentageLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

    batteryBar = new QProgressBar();
    batteryBar->setRange(0, 100);
    batteryBar->setValue(batteryLevel * 100);
    batteryBar->setTextVisible(false);
    batteryBar->setFixedHeight(20);
    batteryBar->setStyleSheet(
        "QProgressBar {"
        "  border: 2px solid #444;"
        "  border-radius: 10px;"
        "  background: #222;"
        "}"
        "QProgressBar::chunk {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "  stop:0 " +
        primaryColor.name() + ", stop:1 #00ffaa);"
                              "  border-radius: 8px;"
                              "}");

    rangeLabel = new QLabel(QString::number(range) + " km");
    rangeLabel->setAlignment(Qt::AlignCenter);
    rangeLabel->setStyleSheet("font-size: 18px;");

    QLabel *rangeText = new QLabel("RANGE");
    rangeText->setAlignment(Qt::AlignCenter);
    rangeText->setStyleSheet("font-size: 14px; color: gray;");

    leftLayout->addWidget(batteryTitle);
    leftLayout->addWidget(batteryPercentageLabel);
    leftLayout->addWidget(batteryBar);
    leftLayout->addSpacing(10);
    leftLayout->addWidget(rangeLabel);
    leftLayout->addWidget(rangeText);

    // Center panel - Speedometer
    QVBoxLayout *centerPanelLayout = new QVBoxLayout();
    centerPanelLayout->setAlignment(Qt::AlignCenter);

    speedValueLabel = new QLabel("0");
    speedValueLabel->setAlignment(Qt::AlignCenter);
    speedValueLabel->setStyleSheet("font-size: 64px; font-weight: bold;");

    QLabel *speedUnitLabel = new QLabel("km/h");
    speedUnitLabel->setAlignment(Qt::AlignCenter);
    speedUnitLabel->setStyleSheet("font-size: 16px; color: gray;");

    centerPanelLayout->addWidget(speedValueLabel);
    centerPanelLayout->addWidget(speedUnitLabel);

    // Right panel - Power consumption
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setAlignment(Qt::AlignCenter);

    QLabel *consumptionTitle = new QLabel("POWER");
    consumptionTitle->setAlignment(Qt::AlignCenter);
    consumptionTitle->setStyleSheet("font-size: 14px; color: gray;");

    QLabel *consumptionValue = new QLabel(QString::number(powerConsumption) + " Wh/km");
    consumptionValue->setAlignment(Qt::AlignCenter);
    consumptionValue->setStyleSheet("font-size: 18px;");

    rightLayout->addWidget(consumptionTitle);
    rightLayout->addWidget(consumptionValue);

    centerLayout->addLayout(leftLayout, 1);
    centerLayout->addLayout(centerPanelLayout, 2);
    centerLayout->addLayout(rightLayout, 1);

    // Bottom controls
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setAlignment(Qt::AlignCenter);

    powerButton = new QPushButton(isPoweredOn ? "POWER ON" : "POWER OFF");
    powerButton->setFixedSize(120, 40);
    powerButton->setStyleSheet(
        "QPushButton {"
        "  background-color: " +
        (isPoweredOn ? primaryColor.name() : "#444") + ";"
                                                       "  color: white;"
                                                       "  border: none;"
                                                       "  border-radius: 5px;"
                                                       "  font-weight: bold;"
                                                       "}"
                                                       "QPushButton:pressed {"
                                                       "  background-color: #0080ff;"
                                                       "}");
    connect(powerButton, &QPushButton::clicked, this, &Dashboard::togglePower);

    driveModeButton = new QPushButton("MODE: " + driveMode);
    driveModeButton->setFixedSize(150, 40);
    driveModeButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #333;"
        "  color: white;"
        "  border: 1px solid #555;"
        "  border-radius: 5px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #444;"
        "}");
    connect(driveModeButton, &QPushButton::clicked, this, &Dashboard::changeDriveMode);

    bottomLayout->addWidget(powerButton);
    bottomLayout->addSpacing(20);
    bottomLayout->addWidget(driveModeButton);

    // Add all layouts to main layout
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(centerLayout);
    mainLayout->addLayout(bottomLayout);
}

void Dashboard::drawSpeedometer(QPainter &painter)
{
    int centerX = width() / 2;
    int centerY = height() / 2 - 20;
    int radius = 180;

    // Draw outer arc
    QPen pen(secondaryColor);
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawArc(centerX - radius, centerY - radius, radius * 2, radius * 2, 30 * 16, 120 * 16);

    // Draw speed markers
    pen.setWidth(2);
    painter.setPen(pen);

    for (int i = 0; i <= 180; i += 30)
    {
        double angle = (i + 30) * M_PI / 180;
        int x1 = centerX + (radius - 10) * cos(angle);
        int y1 = centerY + (radius - 10) * sin(angle);
        int x2 = centerX + radius * cos(angle);
        int y2 = centerY + radius * sin(angle);

        painter.drawLine(x1, y1, x2, y2);

        // Draw speed numbers
        if (i % 60 == 0)
        {
            int speedValue = i * maxSpeed / 180;
            QString speedText = QString::number(speedValue);
            QFont font = painter.font();
            font.setPointSize(10);
            painter.setFont(font);

            int textX = centerX + (radius - 30) * cos(angle) - 10;
            int textY = centerY + (radius - 30) * sin(angle) + 5;

            painter.drawText(textX, textY, speedText);
        }
    }

    // Draw needle
    double speedRatio = static_cast<double>(currentSpeed) / maxSpeed;
    double needleAngle = (speedRatio * 120 + 30) * M_PI / 180;

    QPolygon needle;
    needle << QPoint(centerX, centerY);
    needle << QPoint(centerX + (radius - 20) * cos(needleAngle - 0.1),
                     centerY + (radius - 20) * sin(needleAngle - 0.1));
    needle << QPoint(centerX + radius * cos(needleAngle),
                     centerY + radius * sin(needleAngle));
    needle << QPoint(centerX + (radius - 20) * cos(needleAngle + 0.1),
                     centerY + (radius - 20) * sin(needleAngle + 0.1));

    painter.setBrush(alertColor);
    painter.setPen(Qt::NoPen);
    painter.drawPolygon(needle);

    // Draw center circle
    painter.setBrush(primaryColor);
    painter.drawEllipse(centerX - 10, centerY - 10, 20, 20);
}

void Dashboard::drawBatteryStatus(QPainter &painter)
{
    int batteryX = width() / 4 - 30;
    int batteryY = height() / 2 + 20;

    // Draw battery outline
    QPen pen(secondaryColor);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    painter.drawRect(batteryX, batteryY, 120, 40);
    painter.drawRect(batteryX + 120, batteryY + 10, 10, 20);

    // Draw battery level
    int fillWidth = static_cast<int>(batteryLevel * 115);

    QLinearGradient gradient(batteryX, 0, batteryX + fillWidth, 0);
    gradient.setColorAt(0, primaryColor);
    gradient.setColorAt(1, QColor(0, 255, 170));

    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawRect(batteryX + 2, batteryY + 2, fillWidth, 36);
}

void Dashboard::drawPowerConsumption(QPainter &painter)
{
    int powerX = 3 * width() / 4 - 30;
    int powerY = height() / 2 + 20;

    // Draw power consumption bar
    int barWidth = 120;
    int barHeight = 40;

    // Draw background
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(40, 40, 40));
    painter.drawRect(powerX, powerY, barWidth, barHeight);

    // Draw consumption level (normalized to 0-300 Wh/km)
    double consumptionRatio = static_cast<double>(powerConsumption) / 300;
    consumptionRatio = qMin(consumptionRatio, 1.0);

    int fillWidth = static_cast<int>(consumptionRatio * barWidth);

    QLinearGradient gradient(powerX, 0, powerX + fillWidth, 0);
    if (consumptionRatio < 0.5)
    {
        gradient.setColorAt(0, QColor(0, 200, 0));
        gradient.setColorAt(1, QColor(100, 255, 100));
    }
    else if (consumptionRatio < 0.8)
    {
        gradient.setColorAt(0, QColor(200, 200, 0));
        gradient.setColorAt(1, QColor(255, 255, 100));
    }
    else
    {
        gradient.setColorAt(0, QColor(200, 0, 0));
        gradient.setColorAt(1, QColor(255, 100, 100));
    }

    painter.setBrush(gradient);
    painter.drawRect(powerX, powerY, fillWidth, barHeight);

    // Draw outline
    painter.setPen(secondaryColor);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(powerX, powerY, barWidth, barHeight);
}
