//
// Created by twak on 11/11/2019.
//

#include "the_player.h"

using namespace std;

// all buttons have been setup, store pointers here
void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;
    jumpTo(buttons -> at(0) -> info);
}

void ThePlayer::jumpTo (TheButtonInfo* button) {
    setMedia( * button -> url);
    play();

    // we only want the button that was clicked to be highlighted, the rest stay the same.
    // The button selected was "highlighted" in clicked() in the_button.cpp
    for(TheButton *b: *buttons)
    {
        if(b->info->url == button->url)
            continue;
        b->setStyleSheet("background: white");
    }
}

//takes the slider's position and converts it to a position the video can set itself to
void ThePlayer::seek (int time) {
    setPosition(time*1000);
}
