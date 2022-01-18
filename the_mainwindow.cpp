#include "the_mainwindow.h"

MainWindow::MainWindow()
{
    themeValue=true;
}

void MainWindow::changeTheme()
{
    themeValue = !themeValue;
    if(themeValue){
        this->setStyleSheet("background-color: black");
    }
    else{
        this->setStyleSheet("background-color: white");
    }
}
