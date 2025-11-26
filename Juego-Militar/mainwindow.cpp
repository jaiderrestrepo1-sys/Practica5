#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(0,0,800,600,this))
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);

    control = new GameController(scene, this);

    connect(control, &GameController::progreso, this, &MainWindow::actualizarProgreso);
    connect(control, &GameController::jugadorGana, this, &MainWindow::mostrarGanador);
    connect(control, &GameController::turnoCambiado, this, &MainWindow::actualizarTurno);

    control->colocarObstaculo(300, 500, 150, 1);
    control->colocarObstaculo(350, 500, 150, 1);

    control->colocarObstaculo(450, 500, 150, 2);
    control->colocarObstaculo(500, 500, 150, 2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnLanzar_clicked()
{
    double ang = ui->spinAngulo->value();
    double vel = ui->spinVelocidad->value();

    control->lanzar(ang, vel);
}

void MainWindow::actualizarProgreso(int v1, int v2)
{
    ui->lblInfo->setText(
        QString("Jugador 1 vida: %1  |  Jugador 2 vida: %2")
            .arg(v1).arg(v2)
        );
}

void MainWindow::mostrarGanador(int jugador)
{
    QMessageBox::information(this, "FIN DEL JUEGO",
                             QString("¡Jugador %1 gana!").arg(jugador));
}

void MainWindow::actualizarTurno(int t)
{
    ui->lblTurno->setText(QString("Turno actual: Jugador %1").arg(t));
}
