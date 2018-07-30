#ifndef GAUSSIAN_PYRAMID_H
#define GAUSSIAN_PYRAMID_H

#include <QGraphicsScene>
#include <QMultiMap>


class Gaussian_Pyramid
{
public:
    Gaussian_Pyramid();
    void make_layer(QImage &image); //Builds next layer of the pyramid
    void make_layer(QImage &image, int layer); //Builds needed layer of the pyramid

    double get_coeff(); // Returns current value of scaling coefficient
    void set_coeff(double new_coeff); // Sets new value of scaling coefficient

    int numer_of_layer(QImage &image); //Counts max number of layers for image

private:
    double coeff = 2; //scale coefficient
};

#endif // GAUSSIAN_PYRAMID_H
