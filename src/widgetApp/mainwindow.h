#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace widgetApp{
    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:

        void on_CalculateButton_clicked();

        void on_drawButton_clicked();

        void on_hostNumberSpinBox_valueChanged(int arg1);

        void on_radioButton_clicked(bool checked);

        void drawNetworkGraph(bool isVertical);

        void calculateBitsetFromInput(QWidget *inputWidget, QWidget *displayWidget);

    private:
        Ui::MainWindow *ui;
    };
}

#endif // MAINWINDOW_H
