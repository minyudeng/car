#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QPainter>

class Dashboard : public QWidget
{
    Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);
    ~Dashboard();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateData();
    void togglePower();
    void changeDriveMode();

private:
    void setupUI();
    void drawSpeedometer(QPainter &painter);
    void drawBatteryStatus(QPainter &painter);
    void drawPowerConsumption(QPainter &painter);

private:
    // UI
    QLabel *speedValueLabel;
    QLabel *batteryPercentageLabel;
    QLabel *rangeLabel;
    QLabel *timeLabel;
    QLabel *tempLabel;
    QLabel *driveModeLabel;
    QProgressBar *batteryBar;
    QPushButton *powerButton;
    QPushButton *driveModeButton;
    // data
    int currentSpeed;
    int maxSpeed;
    double batteryLevel;
    int range;
    int powerConsumption;
    int outsideTemp;
    bool isPoweredOn;
    QString driveMode;

    QTimer *dataTimer;
    
    // colors
    QColor primaryColor;
    QColor secondaryColor;
    QColor alertColor;
};

#endif // DASHBOARD_H
