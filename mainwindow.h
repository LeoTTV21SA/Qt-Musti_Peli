#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QString>
#include <QVector>
#include <QHash>
#include <QPushButton>
#include <QMessageBox>
#include <random>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTimer *timer=new QTimer();
    QTime time;
    QVector<QString> cards{"card01", "card02", "card03", "card04",
                              "card05", "card06", "card07", "card08",
                              "card09", "card10", "card11", "card12"};
    QHash<QString, QString> distribution;
    int score=0;
    bool playstarted;
    QPushButton* cardPrevious;
    QPushButton* currentcard;
    int couplesRemaining;
    QMessageBox msgBox;


private slots:
    void updateStopwatch();
    void uncoveredCard();
    void scramble(QVector<QString> &cards);
    void distribute(QVector<QString> &cards, QHash<QString, QString> &distribution);
    void definePartialResult();
    void resetCards();
    void showimage();
    void defineFinalResult();
    void updateStatus();
    void initializeGame();


private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
