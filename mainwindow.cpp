#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./testDB.db");
    if(db.open()) qDebug() << "open";
    else qDebug() << "noOpen";

    query = new QSqlQuery(db);
    query -> exec("CREATE TABLE Testme(Time TEXT, Socket TEXT, Message TEXT);");

    model = new QSqlTableModel(this, db);
    model -> setTable("Testme");
    model -> select();

    ui -> tableView->setModel(model);



    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    nextBlockSize = 0;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_6);
    if(in.status() == QDataStream::Ok)
        if(in.status() == QDataStream::Ok)
        {
            /*QString str;
        in >> str;
        ui -> textBrowser -> append(str);*/

            for(;;)
            {
                if(nextBlockSize == 0)
                {
                    if(socket -> bytesAvailable() < 2)
                    {
                        break;
                    }

                    in >> nextBlockSize;
                }
                if(socket->bytesAvailable() < nextBlockSize)
                {
                    break;
                }

                QString str;
                QTime time;
                in >> time >> str;
                nextBlockSize = 0;



                int row = model->rowCount();

                // Добавляем новую строку
                model->insertRow(row);

                QModelIndex index = model->index(row, 0);

                model->setData(index, time.toString());

                index = model->index(row, 1); // Индекс второго столбца
                model->setData(index, "slot?"); // Устанавливаем значение второго столбца

                index = model->index(row, 2); // Индекс третьего столбца
                model->setData(index, str); // Устанавливаем значение третьего столбца





                //ui -> textBrowser -> append(time.toString() + " " + str);
            }
        }

}

void MainWindow::on_pushButton_clicked()
{
    int row = model->rowCount(); // Получаем номер новой строки

    // Добавляем новую строку
    model->insertRow(row);

    // Устанавливаем значения для каждого столбца в новой строке
    QModelIndex index = model->index(row, 0); // Индекс первого столбца
    model->setData(index, "значение_столбца_1"); // Устанавливаем значение первого столбца

    index = model->index(row, 1); // Индекс второго столбца
    model->setData(index, "значение_столбца_2"); // Устанавливаем значение второго столбца

    index = model->index(row, 2); // Индекс третьего столбца
    model->setData(index, "значение_столбца_3"); // Устанавливаем значение третьего столбца
}





void MainWindow::on_pushButton_2_clicked()
{
    model -> removeRow(row);
}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    row = index.row();
}


void MainWindow::on_pushButton_3_clicked()
{
    socket -> connectToHost("127.0.0.1", 2323);
    qDebug() << "connetcedtry";
}

