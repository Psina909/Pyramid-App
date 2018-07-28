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
    void make_pyramid(QImage &image); //Builds pyramid in the scene
    void fill_comboBox_Layers(QImage &image); //Fills comboBox with numbers of layers

private slots:
    void on_actionOpen_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;

    //Graphics objects
    QGraphicsScene* scene;
    QGraphicsView* view;
};

#endif // MAINWINDOW_H
