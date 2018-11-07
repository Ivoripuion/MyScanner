#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QTcpSocket>
#include<QFileDialog>
#include<QTextStream>
#include<QThread>
#include<QMessageBox>
int staport;
int endport;
QString log_name="log.txt";//日志名称
QString ip;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
}

bool MainWindow::checkip(QString ip)
{
    QRegExp regex("\\d+\\.\\d+\\.\\d+\\.\\d+");
    if( !regex.exactMatch(ip) )//通过正则的方式匹配是否是...的形式来表示ip地址的
    {
        return false;
    }
    return true;
}

//扫描
void MainWindow::startscanner(QString ip, int startport, int endport)
{
   if(startport>endport)
   {
       int tmp=startport;
       startport=endport;
       endport=tmp;
   }
   ui->progressBar->setRange(0,endport-startport);
   QTcpSocket socket(0);
   ui->resultOutput->append("start scanning!");
   log.append("start scanning!\n");
   for(int i=startport;i<=endport;i++)
   {
       socket.abort();
       socket.connectToHost(ip,i);
       if(socket.waitForConnected(500)){
           QString msg="rhost :"+ip+" port: "+QString::number(i)+" connected";
           ui->resultOutput->append(msg);
           log.append(msg);
           log.append('\n');
       }
       ui->progressBar->setValue(i);
   }
   ui->resultOutput->append("Scanning finished!");
   log.append("Scanning finished!\n");
}
//文件操作部分
bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly|QFile::Text)){
        QMessageBox::warning(this,tr("textEdit"),tr("保存失败"));
        return false;
    }
    QTextStream out(&file);
    out<<log;
    //鼠标状态的设置
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::restoreOverrideCursor();
    return true;
}

bool MainWindow::saveAs()
{
    QString file_name=QFileDialog::getSaveFileName(this,tr("另存为"),log_name);
    if(file_name.isEmpty())
        return false;
    return saveFile(log_name);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    QString desip=ui->startRhostContent->text();
    if(!checkip(desip)){
        QMessageBox box;
        box.setWindowTitle(tr("警告"));
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("ip地址输入错误"));
        box.exec();
    }else{
        ip=desip;
        staport=ui->startPortSpinBox->value();
        endport=ui->endPortSpinBox->value();
        startscanner(desip,staport,endport);
    }
}

void MainWindow::on_actionSave_triggered()
{
    saveAs();
}
