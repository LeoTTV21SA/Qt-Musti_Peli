#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    //Connect timer to the slot that will handle the timer
    connect(timer, SIGNAL(timeout()), this, SLOT(updateStatus()));

    //Connect each button to the same slot, which will figure out which button was pressed and show its associated image file accordingly
    connect(ui->card01, SIGNAL(clicked()), this, SLOT(uncoveredCard()));
    connect(ui->card02, SIGNAL(clicked()), this, SLOT(uncoveredCard()));
    connect(ui->card03, SIGNAL(clicked()), this, SLOT(uncoveredCard()));
    connect(ui->card04, SIGNAL(clicked()), this, SLOT(uncoveredCard()));
    connect(ui->card05, SIGNAL(clicked()), this, SLOT(uncoveredCard()));
    connect(ui->card06, SIGNAL(clicked()), this, SLOT(uncoveredCard()));
    connect(ui->card07, SIGNAL(clicked()), this, SLOT(uncoveredCard()));
    connect(ui->card08, SIGNAL(clicked()), this, SLOT(uncoveredCard()));
    connect(ui->card09, SIGNAL(clicked()), this, SLOT(uncoveredCard()));
    connect(ui->card10, SIGNAL(clicked()), this, SLOT(uncoveredCard()));
    connect(ui->card11, SIGNAL(clicked()), this, SLOT(uncoveredCard()));
    connect(ui->card12, SIGNAL(clicked()), this, SLOT(uncoveredCard()));

     initializeGame();
}


void MainWindow::uncoveredCard(){
    //get the tile that was pressed
    currentcard=qobject_cast<QPushButton*>(sender());

    //get the image linked to that tile in the map and set tile background to it
    showimage();

    //disable current tile so it can't be clicked again (unless there is no match, in which case it will be re-enabled)
    currentcard->setEnabled(false);

    //do something depending on whether the revealed tile is the first or the second tile in the turn
    if (!playstarted){
        cardPrevious=currentcard;
        playstarted=true;
    }
    else{
        //change score and display it
        definePartialResult();

        //reset turn
        playstarted=false;
    }
}


void MainWindow::showimage(){
    QString card_name=currentcard->objectName();
    QString img=distribution[card_name];
    currentcard->setStyleSheet("#" + card_name + "{ background-image: url(://" + img + ") }");
}


void MainWindow::resetCards(){
    //return tiles from current turn to the default state (remove backgrounds)
    cardPrevious->setStyleSheet("#" + cardPrevious->objectName() + "{ }");
    currentcard->setStyleSheet("#" + currentcard->objectName() + "{ }");

    //re-enable both tiles so they can be used on another turn
    currentcard->setEnabled(true);
    cardPrevious->setEnabled(true);

    //re-enable the whole tile section
    ui->frame->setEnabled(true);
}


void MainWindow::defineFinalResult(){
    msgBox.setWindowTitle("it is finished");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setEscapeButton(QMessageBox::No);

    if (couplesRemaining==0){
        timer->stop();
        msgBox.setText("Won! final score: " + QString::number(score) + "\n Play again?");
        if (QMessageBox::Yes == msgBox.exec()){
             initializeGame();
        }
        else{
            QCoreApplication::quit();
        }
    }
    else{
        if (time.toString()=="00:00:00"){
            timer->stop();
            ui->frame->setEnabled(false);
            msgBox.setText("You lost ;( \n¿Play again?");
            if (QMessageBox::Yes == msgBox.exec()){

                initializeGame();
            }
            else{
                QCoreApplication::quit();
            }
        }
    }
}


void MainWindow::definePartialResult(){
    //check if there is a match (the current tile matches the previous tile in the turn)
    if (distribution[currentcard->objectName()]==distribution[cardPrevious->objectName()]){
        score+=15;
        ui->lblPuntaje->setText(QString::number(score));
        couplesRemaining--;

        //if there is a match, find out if all tiles have been matched.
        defineFinalResult();
    }
    else{
        score-=5;
        ui->lblPuntaje->setText(QString::number(score));

        //disable the whole tile section so no tiles can be turned during the 1-second "memorizing period"
        ui->frame->setEnabled(false);

        //if there is no match, let user memorize tiles and after 1 second hide tiles from current turn so they can be used on another turn
        QTimer::singleShot(1000, this, SLOT(resetCards()));
    }
}

void MainWindow::initializeGame(){
    //start turn
    playstarted=false;

    //Set score
    score=0;
    ui->lblPuntaje->setText(QString::number(score));;

    //Set matches counter
    couplesRemaining=6;

    //Set clock for countdown
    time.setHMS(0,1,0);

    //Initialize countdown
    ui->chronometer->setText(time.toString("m:ss"));

    // Start timer with a value of 1000 milliseconds, indicating that it will time out every second.
    timer->start(1000);

    //Randomly sort tiles in container
    scramble(cards);

    //Grab pairs of tiles and bind the name of an image file to each pair
    distribute(cards, distribution);

    //enable tiles frame
    ui->frame->setEnabled(true);

    //enable every tile and reset its image
    QList<QPushButton *> buttons =  ui->centralWidget->findChildren<QPushButton*>();
    foreach (QPushButton* b, buttons) {
        b->setEnabled(true);
        b->setStyleSheet("#" + b->objectName() + "{ }");
    }
}


void MainWindow::updateStopwatch(){
    time=time.addSecs(-1);
    ui->chronometer->setText(time.toString("m:ss"));
}


void MainWindow::updateStatus(){
    updateStopwatch();
    defineFinalResult();
}


void MainWindow::scramble(QVector<QString> &cards){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle (cards.begin(), cards.end(), std::default_random_engine(seed));
}


void MainWindow::distribute(QVector<QString> &cards, QHash<QString, QString> &distribution){
    auto iterador=cards.begin();
    for (int i=1; i<=6; i++){
        QString file_name="0"+QString::number(i)+".png";
        distribution[(*iterador)]=file_name;
        iterador++;
        distribution[(*iterador)]=file_name;
        iterador++;
    }
}


MainWindow::~MainWindow(){
    delete ui;
}
