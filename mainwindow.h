#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <qcustomplot.h>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QTimer *timer;
    QTimer *timer2;
    QTimer *timer3;
    QTimer *timer4;
    QTimer *timer5;
private slots:
    void runonopenedbuffer();
    void makePlot();
    void makePlot_2D();
    void makeAlgrithmedPlot();
    void checkexport();
    void doexport();
    void CheckDeviceConnection();
    void on_Button_Draw1D_clicked();
    void on_StopRePlotting_clicked();
    void on_ReStartPlotting_clicked();
    void on_Button_Draw2D_clicked();
    void on_StartProgramButton_clicked();
    void on_StopProgramButton_clicked();
    void on_Plotsmoothed_clicked();
    void slotMouseMove(QMouseEvent * event);

    void on_ChannelNumber1D_valueChanged();
    void on_FirstChannelSelector_currentTextChanged(const QString &arg1);
    void on_SecondChannelSelector_currentTextChanged(const QString &arg1);
    void on_tab_2D_CoherencyTime_editingFinished();
    void on_smooth_point_num_valueChanged(int arg1);
    void on_SaveEntireBuffer_clicked();
    void on_OpenBuffer_clicked();

    void on_Peak_1stDrive_clicked();

    void on_FirstDrive_Num_valueChanged(int arg1);


    void on_DoSigmaLcalc_clicked();

    void on_DoSigmaHcalc_clicked();

    void on_ChannelsViewNumber_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QCustomPlot *wGraphic;
    QCPCurve *verticalLine;
    QCPItemTracer *tracer;
    QCPItemTracer *tracer2d;
    QCPItemTracer *tracersmoothed;
    QCPItemTracer *tracer1;
    QCPItemTracer *tracer2;
    QCPItemTracer *tracer3;
    QCPItemTracer *tracer4;
    QCPItemTracer *tracer5;
    QCPItemTracer *tracer6;
    QCPItemTracer *tracer7;
    QCPItemTracer *tracer8;

};

#endif // MAINWINDOW_H
