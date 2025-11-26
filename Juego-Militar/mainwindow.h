#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "gamecontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnLanzar_clicked();
    void actualizarProgreso(int v1, int v2);
    void mostrarGanador(int jugador);
    void actualizarTurno(int t);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    GameController *control;
};

#endif
