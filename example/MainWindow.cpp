#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Windows.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->m_ps = new QPowerShell(this);

    QObject::connect(this->m_ps, &QPowerShell::pipeFinished, this, &MainWindow::psFinished);
    this->m_currentPipe = this->m_ps->newPipe();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (this->m_currentPipe)
    {
        this->m_currentPipe->addCommand("Get-ChildItem")
                .addParam("-Path", "C:\\")
                .addCommand("Where-Object")
                .addParam("-FilterScript", "{$_.Extension -eq \".txt\"}")
                .addStatement("ECHO \"Well aren't you a Qt pie...\"")
                .addStatement("ECHO \";)\"");
        this->m_currentPipe->setDescription("Number One!!!111");
        this->m_currentPipe->invoke();
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    this->m_currentPipe->addCommand("Get-CafehildItem")
            .addStatement("Get-Host");
    this->m_currentPipe->invoke();
}

void MainWindow::psFinished(QPowerShellPipeline *pipe, bool success, QPowerShellPipeline::QPSError error, const QByteArray &result)
{
    pipe = pipe;
    qDebug() << pipe->description();

    if (!success)
    {
        qDebug() << "Success:" << success;
        qDebug() << "Error Source: " << error.errorSource;
        qDebug() << "Error Message" << error.errorMessage;
    }
    else
    {
        qDebug() << "Success:" << success;
        qDebug() << "Result:\n" << result;
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    this->m_ps->closeAllPipes();
    qDebug() << "Closed All - Rem:" << this->m_ps->openPipes();
}

void MainWindow::on_pushButton_3_clicked()
{
    qDebug() << "Available:" << this->m_ps->availablePipes();
}

void MainWindow::on_pushButton_4_clicked()
{
    qDebug() << "Will crash if there are no pipes open! :D";
    this->m_ps->closePipe(this->m_currentPipe);
}


