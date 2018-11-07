#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    bool checkip(QString ip);
    void startscanner(QString ip,int startport,int endport);
    bool saveFile(const QString& fileName);
    bool saveAs();
    ~MainWindow();

private slots:
    void on_startButton_clicked();//开始扫描

    void on_actionSave_triggered();

private:
    QString log;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
