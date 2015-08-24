#include "qtopencv.h"
#include <opencv2/core/core_c.h>
#include <QDebug>


/**
 * @brief cvtCvMat2QImage
 *
 * Esta funcao faz a conversao de cv::Mat para QImage;
 *
 * @param cv::Mat & image
 * @return cv::Mat convertida para QImage
 */
QImage cvtCvMat2QImage(const cv::Mat &inMat)
{
    /*QImage qtemp;
    if(!image.empty() && image.depth() == CV_8U)
    {
        const unsigned char * data = image.data;
        qtemp = QImage(image.cols, image.rows, QImage::Format_RGB32);
        for(int y = 0; y < image.rows; ++y, data += image.cols*image.elemSize())
        {
            for(int x = 0; x < image.cols; ++x)
            {
                QRgb * p = ((QRgb*)qtemp.scanLine (y)) + x;
                *p = qRgb(data[x * image.channels()+2], data[x * image.channels()+1], data[x * image.channels()]);
            }
        }
    }
    else if(!image.empty() && image.depth() != CV_8U)
    {
        qDebug("Wrong image format, must be 8_bits\n");
    }
    return qtemp;*/
    switch (inMat.type()){
             // 8-bit, 4 channel
             case CV_8UC4:
             {
                QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );

                return image;
             }

             // 8-bit, 3 channel
             case CV_8UC3:
             {
                QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );

                return image.rgbSwapped();
             }

             // 8-bit, 1 channel
             case CV_8UC1:
             {
                static QVector<QRgb>  sColorTable;

                // only create our color table once
                if ( sColorTable.isEmpty() )
                {
                   for ( int i = 0; i < 256; ++i )
                      sColorTable.push_back( qRgb( i, i, i ) );
                }

                QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );

                image.setColorTable( sColorTable );

                return image;
             }

             default:
                qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
                break;
          }

          return QImage();
}


/**
 * @brief cvtQImage2CvMat
 *
 * Esta funcao faz a conversao de QImage para cv::Mat
 *
 * @param QImage & image
 * @return QImage convertia para cv::Mat
 */
cv::Mat cvtQImage2CvMat(const QImage & image)
{
    QImage image_converted = image.convertToFormat(QImage::Format_RGB32);
    cv::Mat cvImage;
    //qDebug() << "cvtQImage2CvMat - isNull: " << image_converted.isNull() << " depth: " << image_converted.depth() << " format: " << image_converted.format();
    if(!image_converted.isNull() && image_converted.depth() == 32 && image_converted.format() == QImage::Format_RGB32)
    {
        // assume RGB (3 channels)
        int channels = 3;
        cvImage = cv::Mat(image_converted.height(), image_converted.width(), CV_8UC3);
        unsigned char * data = cvImage.data;
        //const IplImage test = cvImage;
        //qDebug("%d vs %d\n", cvImage.cols*int(cvImage.elemSize()), test.widthStep);
        for(int y = 0; y < image.height(); ++y, data+=cvImage.cols*cvImage.elemSize())
        {
            for(int x = 0; x < image.width(); ++x)
            {
                QRgb rgb = image.pixel(x, y);
                data[x * channels+2] = qRed(rgb); //r
                data[x * channels+1] = qGreen(rgb); //g
                data[x * channels] = qBlue(rgb); //b
            }
        }
    }
    else
    {
        qDebug("Failed to convert image : depth=%d(!=32) format=%d(!=%d)\n", image.depth(), image.format(), QImage::Format_RGB32);
    }
    return cvImage;
}

