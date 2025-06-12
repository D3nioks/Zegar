#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Zegar czasu rzeczywistego
    clockTimer = new QTimer(this);
    connect(clockTimer, &QTimer::timeout, this, &MainWindow::updateClock);
    clockTimer->start(1000);
    updateClock();

    // Minutnik
    countdownTimer = new QTimer(this);
    connect(countdownTimer, &QTimer::timeout, this, &MainWindow::updateCountdown);

    // Połączenia przycisków
    connect(ui->buttonSetAlarm, &QPushButton::clicked, this, &MainWindow::setAlarm);
    connect(ui->buttonDeleteAlarm, &QPushButton::clicked, this, &MainWindow::deleteSelectedAlarm);
    connect(ui->buttonStartTimer, &QPushButton::clicked, this, &MainWindow::startCountdown);
    connect(ui->buttonPauseTimer, &QPushButton::clicked, this, &MainWindow::toggleCountdown);
    connect(ui->buttonResetTimer, &QPushButton::clicked, this, &MainWindow::resetCountdown);
    connect(ui->buttonChangeClockColor, &QPushButton::clicked, this, &MainWindow::applyNextClockColor);
    connect(ui->buttonChangeClockFont, &QPushButton::clicked, this, &MainWindow::applyNextClockFont);
    connect(ui->buttonChangeBackground, &QPushButton::clicked, this, &MainWindow::applyNextBackground);

    // Kolory zegara
    clockColors = {
        "#007ACC", "#FF5733", "#28A745", "#6F42C1", "#E83E8C",
        "#20C997", "#FD7E14", "#6610F2", "#DC3545", "#343A40"
    };

    // Czcionki zegara
    clockFonts = {
        QFont("Arial", 24, QFont::Bold),
        QFont("Verdana", 20),
        QFont("Courier New", 22, QFont::Bold),
        QFont("Georgia", 26),
        QFont("Tahoma", 23, QFont::Bold),
        QFont("Helvetica", 21),
        QFont("Comic Sans MS", 25, QFont::Bold),
        QFont("Times New Roman", 24),
        QFont("Consolas", 22, QFont::Bold),
        QFont("Segoe UI", 24)
    };

    setClockStyle(clockColors[0], clockFonts[0]);

    // Tło
    backgroundColors = { "#808080", "#001F3F", "#FF851B", "#8B4513" };
    setStyleSheet(QString("background-color: %1;").arg(backgroundColors[0]));
}

MainWindow::~MainWindow()
{
    for (int i = 0; i < alarms.size(); ++i) {
        delete alarms[i];
    }
    alarms.clear();
    delete ui;
}

void MainWindow::updateClock()
{
    QTime now = QTime::currentTime();
    ui->labelClock->setText(now.toString("HH:mm:ss"));
    checkAlarms();
}

void MainWindow::setAlarm()
{
    QDateTime alarmDateTime = QDateTime(ui->dateEditAlarm->date(), ui->timeEditAlarm->time());

    if (alarmDateTime <= QDateTime::currentDateTime()) {
        QMessageBox::warning(this, "Błąd", "Nie można ustawić alarmu w przeszłości.");
        return;
    }

    auto *alarm = new AlarmEvent(alarmDateTime, "Twój alarm");
    alarms.append(alarm);
    ui->listWidgetAlarms->addItem(alarmDateTime.toString("yyyy-MM-dd HH:mm"));
    QMessageBox::information(this, "Alarm", "Alarm ustawiony.");
}

void MainWindow::deleteSelectedAlarm()
{
    int row = ui->listWidgetAlarms->currentRow();
    if (row >= 0 && row < alarms.size()) {
        delete alarms[row];
        alarms.removeAt(row);
        delete ui->listWidgetAlarms->takeItem(row);
    }
}

void MainWindow::checkAlarms()
{
    QDateTime now = QDateTime::currentDateTime();
    for (int i = 0; i < alarms.size(); ++i) {
        if (alarms[i]->getDateTime() <= now) {
            alarms[i]->trigger();
            delete alarms[i];
            alarms.removeAt(i);
            delete ui->listWidgetAlarms->takeItem(i);
            --i;
        }
    }
}

void MainWindow::startCountdown()
{
    countdownSeconds = ui->spinBoxTimer->value();
    if (countdownSeconds > 0) {
        countdownRunning = true;
        countdownTimer->start(1000);
        updateCountdownDisplay();
        ui->buttonPauseTimer->setText("Pauza");
    }
}

void MainWindow::toggleCountdown()
{
    if (countdownRunning) {
        countdownTimer->stop();
        countdownRunning = false;
        ui->buttonPauseTimer->setText("Wznów");
    } else {
        if (countdownSeconds > 0) {
            countdownTimer->start(1000);
            countdownRunning = true;
            ui->buttonPauseTimer->setText("Pauza");
        }
    }
}

void MainWindow::resetCountdown()
{
    countdownTimer->stop();
    countdownRunning = false;
    countdownSeconds = 0;
    ui->labelTimerDisplay->setText("00:00");
    ui->buttonPauseTimer->setText("Pauza");
}

void MainWindow::updateCountdown()
{
    if (--countdownSeconds <= 0) {
        countdownTimer->stop();
        countdownRunning = false;
        ui->labelTimerDisplay->setText("00:00");
        QMessageBox::information(this, "Minutnik", "Minutnik zakończony!");
    } else {
        updateCountdownDisplay();
    }
}

void MainWindow::updateCountdownDisplay()
{
    int minutes = countdownSeconds / 60;
    int seconds = countdownSeconds % 60;
    ui->labelTimerDisplay->setText(QString("%1:%2")
                                       .arg(minutes, 2, 10, QChar('0'))
                                       .arg(seconds, 2, 10, QChar('0')));
}

// Styl zegara

void MainWindow::setClockStyle(const QString &color, const QFont &font)
{
    ui->labelClock->setFont(font);
    ui->labelClock->setStyleSheet(QString("color: %1;").arg(color));
}

void MainWindow::applyNextClockColor()
{
    clockColorIndex = (clockColorIndex + 1) % clockColors.size();
    setClockStyle(clockColors[clockColorIndex], clockFonts[clockFontIndex]);
}

void MainWindow::applyNextClockFont()
{
    clockFontIndex = (clockFontIndex + 1) % clockFonts.size();
    setClockStyle(clockColors[clockColorIndex], clockFonts[clockFontIndex]);
}

// Tło

void MainWindow::applyNextBackground()
{
    backgroundIndex = (backgroundIndex + 1) % backgroundColors.size();
    setStyleSheet(QString("background-color: %1;").arg(backgroundColors[backgroundIndex]));
}
