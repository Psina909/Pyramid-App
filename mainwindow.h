#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "gaussian_pyramid.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void show_layer(QImage &image, int layer); //Adds needed layer to the scene
    void fill_comboBox_Layers(QImage &image); //Fills comboBox with numbers of layers
    void rebuild_comboBox_files(); // Updates comboBox of files with actual information
    bool isAvailable(QImage &image); // Checks if image is not null, prevent mistakes

private slots:
    void on_actionOpen_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void on_comboBox_activated(int index); //comboBox of layers
    void on_comboBox_2_activated(const QString &arg1); //comboBox of files
    void on_doubleSpinBox_valueChanged(double arg1); // SpinBox for scale coefficient

private:
    Ui::MainWindow *ui;
    QMultiMap<int, QString> map; // contains <diagonal, fileName>
    Gaussian_Pyramid pyramid; // Pyramid object

    //Graphics objects
    QGraphicsScene* scene;
    QGraphicsView* view;
};

#endif // MAINWINDOW_H
