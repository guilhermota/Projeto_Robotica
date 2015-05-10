#include "video.h"

Video::Video()
{

}

Video::~Video()
{
    this->close();
}


//TODO Escolher qual camera abrir
bool Video::open()
{
    qDebug("Abrindo Video");
    cap.open(0);

    qDebug("Video aberto");
    if(cap.isOpened()){
        return true;
    } else{
        return false;
    }
}

void Video::close()
{
    cap.release();
}
