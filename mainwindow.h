#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include <QList>
#include <QFont>
#include <QPair>
#include <QVector>
#include <QStringList>
#include "alarmevent.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateClock();
    void setAlarm();
    void deleteSelectedAlarm();
    void startCountdown();
    void updateCountdown();
    void toggleCountdown();
    void resetCountdown();

    void applyNextClockColor();
    void applyNextClockFont();
    void applyNextBackground();

private:
    Ui::MainWindow *ui;

    QTimer *clockTimer;
    QTimer *countdownTimer;
    QList<TimeEvent*> alarms;
    int countdownSeconds = 0;
    bool countdownRunning = false;

    void checkAlarms();
    void updateCountdownDisplay();
    void setClockStyle(const QString &color, const QFont &font);

    // Styl zegara - osobno kolor i czcionka
    QStringList clockColors;
    int clockColorIndex = 0;

    QVector<QFont> clockFonts;
    int clockFontIndex = 0;

    // Tło
    QStringList backgroundColors;
    int backgroundIndex = 0;
};

#endif // MAINWINDOW_H
