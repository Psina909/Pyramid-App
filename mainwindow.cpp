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
    delete scene;
    delete view;
}

void MainWindow::make_layer(QImage &image)
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

void MainWindow::fill_comboBox_Layers(QImage &image)
{
    if(!(ui->comboBox->isEnabled()))
        ui->comboBox->setEnabled(true);
    if(!(ui->doubleSpinBox->isEnabled()))
        ui->doubleSpinBox->setEnabled(true);

    ui->comboBox->clear();

    int N = qFloor(qLn(qMin(image.height(),image.width()))/qLn(2)); //Number of layers

    //Fill comboBox with numbers of layers
    for(int i=0; i<=N; i++)
        ui->comboBox->addItem(QVariant(i).toString());
}

void MainWindow::rebuild_comboBox_files()
{
    if(!(ui->comboBox_2->isEnabled()))
        ui->comboBox_2->setEnabled(true);

    ui->comboBox_2->clear();

    //Fill comboBox of files with actual information
    QMultiMap<int,QString>::iterator it = map.begin();
    for(; it != map.end(); ++it)
        ui->comboBox_2->addItem(it.value());

}

void MainWindow::show_layer(QImage &image, int layer)
{
    scene->clear();

    int w = image.width(); //Original widght
    int h = image.height(); //Original height

    if(layer == 0)
    {
        //Add original image to the scene
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        scene->addItem(item);

        //Show size of the image
        ui->label->setText("Size: "+QVariant(image.width()).toString()+"x"+QVariant(image.height()).toString());
    }
    else
    {
        //Build needed layer
        for(int i=0; i!=layer; i++)
            make_layer(image);

        //Show size of the layer
        ui->label->setText("Size: "+QVariant(image.width()).toString()+"x"+QVariant(image.height()).toString());

        image = image.scaled(w,h);//Scale to original resolution

        //Add this layer to the scene
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        scene->addItem(item);
    }
}

bool MainWindow::isAvailable(QImage &image)
{
    if(image.isNull())
    {
        //To prevent mistakes
        ui->comboBox->setEnabled(false);
        ui->doubleSpinBox->setEnabled(false);
        QMessageBox::information(this,"Image Viewer","ERROR: Can't display image");
        scene->clear();
        return false;
    } else return true;
}

void MainWindow::on_actionOpen_triggered()
{
    //Get image
    QString fileName = QFileDialog::getOpenFileName(this,"Open Image File",QDir::currentPath(),"Images(*.png, *.jpg)");

    if(!fileName.isEmpty())
    {
        QImage image(fileName);

        if(isAvailable(image))
        {
            double diag = qSqrt(image.width()*image.width()+image.height()*image.height()); //Diagonal

            //Check if this image is already opened
            if(!map.contains(QVariant(diag).toInt(),fileName))
            {
                map.insert(QVariant(diag).toInt(),fileName);
            }
            else
            {
                QMessageBox::information(this,"Image Viewer","This image is already opened!");
                return;
            }

            rebuild_comboBox_files(); //Fill comboBox of files with actual information
            ui->comboBox_2->setCurrentText(fileName);
            fill_comboBox_Layers(image); //Fill comboBox of Layers
            show_layer(image,0); //Add original image to the scene
            ui->graphicsView->setScene(scene);
        }
    }
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionAbout_triggered()
{

}

void MainWindow::on_comboBox_activated(int index)
{
    //Get image
    QImage image(ui->comboBox_2->currentText());

    if(isAvailable(image))
        show_layer(image, index);//Draw layer
}

void MainWindow::on_comboBox_2_activated(const QString &arg1)
{
    scene->clear();

    //Get image
    QImage image(arg1);

    if(isAvailable(image))
    {
        fill_comboBox_Layers(image); //Fill comboBox of Layers
        show_layer(image,0);//Add original image to the scene
    }    
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    coeff = arg1;

    //Get image
    QImage image(ui->comboBox_2->currentText());

    if(isAvailable(image))
    {
        fill_comboBox_Layers(image);
        show_layer(image, 0);
    }
}
