#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void make_layer(QImage &image); //Builds next layer of the pyramid
    void show_layer(QImage &image, int layer); //Adds needed layer to the scene
    void fill_comboBox_Layers(QImage &image); //Fills comboBox with numbers of layers

private slots:
    void on_actionOpen_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void on_comboBox_activated(int index);

private:
    Ui::MainWindow *ui;

    //Graphics objects
    QGraphicsScene* scene;
    QGraphicsView* view;
};

#endif // MAINWINDOW_H
