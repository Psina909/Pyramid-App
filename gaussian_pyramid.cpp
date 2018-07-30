#include "gaussian_pyramid.h"

#include <QMatrix3x3>
#include <QtMath>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QMessageBox>

Gaussian_Pyramid::Gaussian_Pyramid()
{

}

void Gaussian_Pyramid::make_layer(QImage &image)
{
    //Gaussian blur algorithm
    QImage blur = image;

    QMatrix3x3 kernel;
    kernel(0, 0) = 1; kernel(0, 1) = 2; kernel(0, 2) = 1;
    kernel(1, 0) = 2; kernel(1, 1) = 4; kernel(1, 2) = 2;
    kernel(2, 0) = 1; kernel(2, 1) = 2; kernel(2, 2) = 1;
    float kernel_sum = 16.0;

    for(int i=1; i<image.width()-1; i++)
    {
        for(int j=1; j<image.height()-1; j++)
        {
            float red = 0, green = 0, blue = 0;
            // =====================================================
            red =
                    kernel(0, 0) * qRed(image.pixel(i+1, j+1)) +
                    kernel(0, 1) * qRed(image.pixel(i, j+1)) +
                    kernel(0, 2) * qRed(image.pixel(i-1, j+1)) +

                    kernel(1, 0) * qRed(image.pixel(i+1, j)) +
                    kernel(1, 1) * qRed(image.pixel(i, j)) +
                    kernel(1, 2) * qRed(image.pixel(i-1, j)) +

                    kernel(2, 0) * qRed(image.pixel(i+1, j-1)) +
                    kernel(2, 1) * qRed(image.pixel(i, j-1)) +
                    kernel(2, 2) * qRed(image.pixel(i-1, j-1));

            // ======================================================
            green =
                    kernel(0, 0) * qGreen(image.pixel(i+1, j+1)) +
                    kernel(0, 1) * qGreen(image.pixel(i, j+1)) +
                    kernel(0, 2) * qGreen(image.pixel(i-1, j+1)) +

                    kernel(1, 0) * qGreen(image.pixel(i+1, j)) +
                    kernel(1, 1) * qGreen(image.pixel(i, j)) +
                    kernel(1, 2) * qGreen(image.pixel(i-1, j)) +

                    kernel(2, 0) * qGreen(image.pixel(i+1, j-1)) +
                    kernel(2, 1) * qGreen(image.pixel(i, j-1)) +
                    kernel(2, 2) * qGreen(image.pixel(i-1, j-1));

            // ======================================================
            blue =
                    kernel(0, 0) * qBlue(image.pixel(i+1, j+1)) +
                    kernel(0, 1) * qBlue(image.pixel(i, j+1)) +
                    kernel(0, 2) * qBlue(image.pixel(i-1, j+1)) +

                    kernel(1, 0) * qBlue(image.pixel(i+1, j)) +
                    kernel(1, 1) * qBlue(image.pixel(i, j)) +
                    kernel(1, 2) * qBlue(image.pixel(i-1, j)) +

                    kernel(2, 0) * qBlue(image.pixel(i+1, j-1)) +
                    kernel(2, 1) * qBlue(image.pixel(i, j-1)) +
                    kernel(2, 2) * qBlue(image.pixel(i-1, j-1));

            blur.setPixel(i,j, qRgb(QVariant(red/kernel_sum).toInt(), QVariant(green/kernel_sum).toInt(), QVariant(blue/kernel_sum).toInt()));
        }
    }

    //Scale
    int h = qCeil(image.height()/coeff);
    int w = qCeil(image.width()/coeff);

    image = blur.scaled(w, h);
}

void Gaussian_Pyramid::make_layer(QImage &image, int layer)
{
    //Build needed layer
    for(int i=0; i!=layer; i++)
        make_layer(image);
}

double Gaussian_Pyramid::get_coeff()
{
    return coeff;
}

void Gaussian_Pyramid::set_coeff(double new_coeff)
{
    coeff = new_coeff;
}

int Gaussian_Pyramid::numer_of_layer(QImage &image)
{
    int N=0; //Number of layers
    int w = image.width();
    int h = image.height();

    int w1 = w, h1 = h; // width and height on the previos step

    // Find number of layers
    while(qMin(w,h)!=1)
    {
        w = qCeil(w/get_coeff());
        h = qCeil(h/get_coeff());

        if(w == w1 && h==h1) //Check if image is not scaling anymore
            break;

        w1 = w;
        h1 = h;
        ++N;
    }

    return N;
}
