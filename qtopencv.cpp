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
 * Criado por:
 * Andy Maloney
 * 23 November 2013
 * http://asmaloney.com/2013/11/code/converting-between-cvmat-and-qimage-or-qpixmap
 */
QImage cvtCvMat2QImage(const cv::Mat &inMat){
    switch (inMat.type()){
             // 8-bit, 4 channels
             case CV_8UC4: {
                QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );
                return image;
             }
             // 8-bit, 3 channels
             case CV_8UC3: {
                QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );
                return image.rgbSwapped();
             }
             // 8-bit, 1 channel
             case CV_8UC1: {
                static QVector<QRgb>  sColorTable;
                // só cria color table uma vez
                if (sColorTable.isEmpty()){
                   for ( int i = 0; i < 256; ++i )
                      sColorTable.push_back(qRgb(i, i, i));
                }
                QImage image(inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8);
                image.setColorTable(sColorTable);
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
 * @return QImage convertida para cv::Mat
 *
  Copyright (c) 2011-2014, Mathieu Labbe - IntRoLab - Universite de Sherbrooke
  All rights reserved.
  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Universite de Sherbrooke nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

cv::Mat cvtQImage2CvMat(const QImage & image){
    QImage image_converted = image.convertToFormat(QImage::Format_RGB32);
    cv::Mat cvImage;
    if(!image_converted.isNull() && image_converted.depth() == 32 && image_converted.format() == QImage::Format_RGB32){
        // espera RGB (3 channels)
        int channels = 3;
        cvImage = cv::Mat(image_converted.height(), image_converted.width(), CV_8UC3);
        unsigned char * data = cvImage.data;
        for(int y = 0; y < image.height(); ++y, data+=cvImage.cols*cvImage.elemSize()){
            for(int x = 0; x < image.width(); ++x){
                QRgb rgb = image.pixel(x, y);
                data[x * channels+2] = qRed(rgb); //r
                data[x * channels+1] = qGreen(rgb); //g
                data[x * channels] = qBlue(rgb); //b
            }
        }
    }
    else{
        qDebug("Falha ao converter imagem : depth=%d(!=32) format=%d(!=%d)\n", image.depth(), image.format(), QImage::Format_RGB32);
    }
    return cvImage;
}
