#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QMatrix3x3>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::make_layer(QImage &image)
{
    //Gaussian blur algorithm
    QImage blur = image;

    QMatrix3x3 kernel;
    kernel(0, 0) = 1; kernel(0, 1) = 2; kernel(0, 2) = 1;
    kernel(1, 0) = 2; kernel(1, 1) = 4; kernel(1, 2) = 2;
    kernel(2, 0) = 1; kernel(2, 1) = 2; kernel(2, 2) = 1;
    int kernel_sum = 16.0;

    for(int i=1; i<image.width()-1; i++)
    {
        for(int j=1; j<image.height()-1; j++)
        {
            int red = 0, green = 0, blue = 0;
            // *****************************************************
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

            // *****************************************************
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

            // *****************************************************
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

            blur.setPixel(i,j, qRgb(red/kernel_sum, green/kernel_sum, blue/kernel_sum));
        }
    }

    //Scaling
    int h = image.height()/2;
    int w = image.width()/2;

    image = blur.scaled(w, h);
}

void MainWindow::make_pyramid(QImage &image)
void MainWindow::fill_comboBox_Layers(QImage &image)
{
    if(!(ui->comboBox->isEnabled()))
        ui->comboBox->setEnabled(true);

    ui->comboBox->clear();

    int N = qLn(qMin(image.height(),image.width()))/qLn(2); //Number of layers

    //Fill comboBox with numbers of layers
    for(int i=0; i<=N; i++)
        ui->comboBox->addItem(QVariant(i).toString());
}

{
    scene->clear();

    //Adding original image to the scene
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(item);

    //Bulding other layers and adding them to the scene
    while((qMin(image.height(),image.width()))!=1)
    {
        make_layer(image);
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        scene->addItem(item);
    }

}

void MainWindow::on_actionOpen_triggered()
{
    //Get image
    fileName = QFileDialog::getOpenFileName(this,"Open Image File",QDir::currentPath(),"Images(*.png, *.jpg)");

    if(!fileName.isEmpty())
    {
        QImage image(fileName);
        if(image.isNull())
        {
            QMessageBox::information(this,"Image Viewer","Error: Can't display image");
            return;
        }

        //Fill comboBox of Layers
        fill_comboBox_Layers(image);

        //Add original image to the scene
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        scene->addItem(item);

        ui->label->setText("Size: "+QVariant(image.width()).toString()+"x"+QVariant(image.height()).toString());
        ui->graphicsView->setScene(scene);
    }
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionAbout_triggered()
{

}
