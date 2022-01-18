//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H


#include <QApplication>
#include <QMediaPlayer>
#include "the_button.h"
#include <vector>
#include <QTimer>
#include "the_controls.h"
#include <QHBoxLayout>

using namespace std;

class ThePlayer : public QMediaPlayer {

Q_OBJECT

private:
    vector<TheButtonInfo>* infos;
    vector<TheButton*>* buttons;
    long updateCount = 0;

public:    
    ThePlayer() : QMediaPlayer(NULL) {
        setVolume(0); // be slightly less annoying
    }

    // all buttons have been setup, store pointers here
    void setContent(vector<TheButton*>* b, vector<TheButtonInfo>* i);

//private slots:


public slots:

    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);
    void seek (int time);
};

#endif //CW2_THE_PLAYER_H
