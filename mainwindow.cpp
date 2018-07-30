#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
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
        pyramid.make_layer(image, layer);

        //Show size of the layer
        ui->label->setText("Size: "+QVariant(image.width()).toString()+"x"+QVariant(image.height()).toString());

        image = image.scaled(w,h);//Scale to original resolution

        //Add this layer to the scene
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        scene->addItem(item);
    }
}

void MainWindow::fill_comboBox_Layers(QImage &image)
{
    if(!(ui->comboBox->isEnabled()))
        ui->comboBox->setEnabled(true);
    if(!(ui->doubleSpinBox->isEnabled()))
        ui->doubleSpinBox->setEnabled(true);

    ui->comboBox->clear();

    int N = pyramid.numer_of_layer(image);

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
QMessageBox::information(this,"About","v5-0-0: Open any image (.png, .jpg) and select any layer of the Gaussian pyramid. It will be scaled to original resolution. You can open several images. You can also select the scaling coefficient.");
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
    pyramid.set_coeff(arg1);

    //Get image
    QImage image(ui->comboBox_2->currentText());

    if(isAvailable(image))
    {
        fill_comboBox_Layers(image);
        show_layer(image, 0);
    }
}
