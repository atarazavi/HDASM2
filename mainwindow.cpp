#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QMainWindow>
#include <QHoverEvent>
#include <QMouseEvent>
#include <QMessageBox>
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPointer>
#include <QtGui>
#include <qobject.h>
#include "ui_mainwindow.h"
#include <algorithm>
#include <future>
#include <memory>
#include <vector>
#include "buffers.h"
#include "data_frame.h"
#include "usb_helper.cpp"
#include <tuple>
#include <libusb-1.0/libusb.h>
#include <thread>
#include <QtConcurrent/QtConcurrentMap>
#include <QtConcurrent/QtConcurrentRun>
#include <QTabWidget>
#include "global.h"
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QInputDialog>
#include "Engine.h"
#include <iostream>
#include <QtCore>
#include <QCoreApplication>
#include <QDebug>
#include <QTextCodec>
#include <QtGlobal>
#include <QString>
#include <string>
#include <QFileDialog>
#include "chn.h"

CHN * mychn;
int ChannelsViewNumbers = 256;
bool offlinedrawing = false;
QCPColorMap *colorMap;
QCPColorScale *colorScale;
QCPMarginGroup *marginGroup;
int plot2d_channel_first = 1;
int plot2d_channel_second = 2;
int plot2d_coherency_time = 10000;
bool tracer_is_defiende = false;
//bool smooth_tracer_is_defined = false;
int starttt = 0;
int enddd = 0;

int timeractive = 0;
int timer1active = 0;
int timer2active = 0;
int timer3active = 0;
int timer4active = 0;

bool active_dev_channel[5][8]={0};
unsigned long long sample_limits[5][8]={0};
unsigned long long time_limits[5][8]={0};

//Milad-select_cypress_devices-device
std::vector<std::shared_ptr<cypress_device>> dev;
QMessageBox* msgBox;

buffers data_buffer;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(makePlot()));

    timer2 = new QTimer(this);
    QObject::connect(timer2, SIGNAL(timeout()), this, SLOT(checkexport()));

    timer3 = new QTimer(this);
    QObject::connect(timer3, SIGNAL(timeout()), this, SLOT(makePlot_2D()));

    timer4 = new QTimer(this);
    QObject::connect(timer4, SIGNAL(timeout()), this, SLOT(CheckDeviceConnection()));


    connect(ui->customPlot, &QCustomPlot::mouseMove, this, &MainWindow::slotMouseMove);
    connect(ui->customPlot_2D, &QCustomPlot::mouseMove, this, &MainWindow::slotMouseMove);
    connect(ui->customPlot_smoothed, &QCustomPlot::mouseMove, this, &MainWindow::slotMouseMove);
    connect(ui->customPlot_Channel1, &QCustomPlot::mouseMove, this, &MainWindow::slotMouseMove);
    connect(ui->customPlot_Channel2, &QCustomPlot::mouseMove, this, &MainWindow::slotMouseMove);
    connect(ui->customPlot_Channel3, &QCustomPlot::mouseMove, this, &MainWindow::slotMouseMove);
    connect(ui->customPlot_Channel4, &QCustomPlot::mouseMove, this, &MainWindow::slotMouseMove);
    connect(ui->customPlot_Channel5, &QCustomPlot::mouseMove, this, &MainWindow::slotMouseMove);
    connect(ui->customPlot_Channel6, &QCustomPlot::mouseMove, this, &MainWindow::slotMouseMove);
    connect(ui->customPlot_Channel7, &QCustomPlot::mouseMove, this, &MainWindow::slotMouseMove);
    connect(ui->customPlot_Channel8, &QCustomPlot::mouseMove, this, &MainWindow::slotMouseMove);

    ui->tab_1D_2D->setCurrentIndex(0);

    ui->Device1_Channel1_Counternum->setMaximum(999999999);
    ui->Device1_Channel1_Counternum->setSingleStep(10000);
    ui->Device1_Channel2_Counternum->setMaximum(999999999);
    ui->Device1_Channel2_Counternum->setSingleStep(10000);
    ui->Device1_Channel3_Counternum->setMaximum(999999999);
    ui->Device1_Channel3_Counternum->setSingleStep(10000);
    ui->Device1_Channel4_Counternum->setMaximum(999999999);
    ui->Device1_Channel4_Counternum->setSingleStep(10000);
    ui->Device1_Channel5_Counternum->setMaximum(999999999);
    ui->Device1_Channel5_Counternum->setSingleStep(10000);
    ui->Device1_Channel6_Counternum->setMaximum(999999999);
    ui->Device1_Channel6_Counternum->setSingleStep(10000);
    ui->Device1_Channel7_Counternum->setMaximum(999999999);
    ui->Device1_Channel7_Counternum->setSingleStep(10000);
    ui->Device1_Channel8_Counternum->setMaximum(999999999);
    ui->Device1_Channel8_Counternum->setSingleStep(10000);

    ui->Device2_Channel1_Counternum->setMaximum(999999999);
    ui->Device2_Channel1_Counternum->setSingleStep(10000);
    ui->Device2_Channel2_Counternum->setMaximum(999999999);
    ui->Device2_Channel2_Counternum->setSingleStep(10000);
    ui->Device2_Channel3_Counternum->setMaximum(999999999);
    ui->Device2_Channel3_Counternum->setSingleStep(10000);
    ui->Device2_Channel4_Counternum->setMaximum(999999999);
    ui->Device2_Channel4_Counternum->setSingleStep(10000);
    ui->Device2_Channel5_Counternum->setMaximum(999999999);
    ui->Device2_Channel5_Counternum->setSingleStep(10000);
    ui->Device2_Channel6_Counternum->setMaximum(999999999);
    ui->Device2_Channel6_Counternum->setSingleStep(10000);
    ui->Device2_Channel7_Counternum->setMaximum(999999999);
    ui->Device2_Channel7_Counternum->setSingleStep(10000);
    ui->Device2_Channel8_Counternum->setMaximum(999999999);
    ui->Device2_Channel8_Counternum->setSingleStep(10000);

    ui->Device3_Channel1_Counternum->setMaximum(999999999);
    ui->Device3_Channel1_Counternum->setSingleStep(10000);
    ui->Device3_Channel2_Counternum->setMaximum(999999999);
    ui->Device3_Channel2_Counternum->setSingleStep(10000);
    ui->Device3_Channel3_Counternum->setMaximum(999999999);
    ui->Device3_Channel3_Counternum->setSingleStep(10000);
    ui->Device3_Channel4_Counternum->setMaximum(999999999);
    ui->Device3_Channel4_Counternum->setSingleStep(10000);
    ui->Device3_Channel5_Counternum->setMaximum(999999999);
    ui->Device3_Channel5_Counternum->setSingleStep(10000);
    ui->Device3_Channel6_Counternum->setMaximum(999999999);
    ui->Device3_Channel6_Counternum->setSingleStep(10000);
    ui->Device3_Channel7_Counternum->setMaximum(999999999);
    ui->Device3_Channel7_Counternum->setSingleStep(10000);
    ui->Device3_Channel8_Counternum->setMaximum(999999999);
    ui->Device3_Channel8_Counternum->setSingleStep(10000);

    ui->Device4_Channel1_Counternum->setMaximum(999999999);
    ui->Device4_Channel1_Counternum->setSingleStep(10000);
    ui->Device4_Channel2_Counternum->setMaximum(999999999);
    ui->Device4_Channel2_Counternum->setSingleStep(10000);
    ui->Device4_Channel3_Counternum->setMaximum(999999999);
    ui->Device4_Channel3_Counternum->setSingleStep(10000);
    ui->Device4_Channel4_Counternum->setMaximum(999999999);
    ui->Device4_Channel4_Counternum->setSingleStep(10000);
    ui->Device4_Channel5_Counternum->setMaximum(999999999);
    ui->Device4_Channel5_Counternum->setSingleStep(10000);
    ui->Device4_Channel6_Counternum->setMaximum(999999999);
    ui->Device4_Channel6_Counternum->setSingleStep(10000);
    ui->Device4_Channel7_Counternum->setMaximum(999999999);
    ui->Device4_Channel7_Counternum->setSingleStep(10000);
    ui->Device4_Channel8_Counternum->setMaximum(999999999);
    ui->Device4_Channel8_Counternum->setSingleStep(10000);

    ui->Device5_Channel1_Counternum->setMaximum(999999999);
    ui->Device5_Channel1_Counternum->setSingleStep(10000);
    ui->Device5_Channel2_Counternum->setMaximum(999999999);
    ui->Device5_Channel2_Counternum->setSingleStep(10000);
    ui->Device5_Channel3_Counternum->setMaximum(999999999);
    ui->Device5_Channel3_Counternum->setSingleStep(10000);
    ui->Device5_Channel4_Counternum->setMaximum(999999999);
    ui->Device5_Channel4_Counternum->setSingleStep(10000);
    ui->Device5_Channel5_Counternum->setMaximum(999999999);
    ui->Device5_Channel5_Counternum->setSingleStep(10000);
    ui->Device5_Channel6_Counternum->setMaximum(999999999);
    ui->Device5_Channel6_Counternum->setSingleStep(10000);
    ui->Device5_Channel7_Counternum->setMaximum(999999999);
    ui->Device5_Channel7_Counternum->setSingleStep(10000);
    ui->Device5_Channel8_Counternum->setMaximum(999999999);
    ui->Device5_Channel8_Counternum->setSingleStep(10000);

    ui->Button_Draw1D->setEnabled(false);
    ui->Button_Draw2D->setEnabled(false);
    ui->ReStartPlotting->setEnabled(false);
    ui->StopRePlotting->setEnabled(false);
    ui->Device_1->setEnabled(false);
    ui->Device_2->setEnabled(false);
    ui->Device_3->setEnabled(false);
    ui->Device_4->setEnabled(false);
    ui->Device_5->setEnabled(false);
    ui->StopProgramButton->setEnabled(false);
    ui->tab_1D_2D->setEnabled(false);
    ui->SaveEntireBuffer->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::CheckDeviceConnection(){
//    dev.clear();
//    dev = initialize_and_select_cypress_devices();
    if(dev.size() == 1){
        ui->Device_1->setEnabled(true);
        ui->Button_Draw1D->setEnabled(true);
        ui->Button_Draw2D->setEnabled(true);
    }
    if(dev.size() == 2){
        ui->Device_1->setEnabled(true);
        ui->Device_2->setEnabled(true);
        ui->Button_Draw1D->setEnabled(true);
        ui->Button_Draw2D->setEnabled(true);
    }
    if(dev.size() == 3){
        ui->Device_1->setEnabled(true);
        ui->Device_2->setEnabled(true);
        ui->Device_3->setEnabled(true);
        ui->Button_Draw1D->setEnabled(true);
        ui->Button_Draw2D->setEnabled(true);
    }
    if(dev.size() == 4){
        ui->Device_1->setEnabled(true);
        ui->Device_2->setEnabled(true);
        ui->Device_3->setEnabled(true);
        ui->Device_4->setEnabled(true);
        ui->Button_Draw1D->setEnabled(true);
        ui->Button_Draw2D->setEnabled(true);
    }
    if(dev.size() == 5){
        ui->Device_1->setEnabled(true);
        ui->Device_2->setEnabled(true);
        ui->Device_3->setEnabled(true);
        ui->Device_4->setEnabled(true);
        ui->Device_5->setEnabled(true);
        ui->Button_Draw1D->setEnabled(true);
        ui->Button_Draw2D->setEnabled(true);
    }
    if(dev.size()){
        ui->tab_1D_2D->setEnabled(true);
        ui->StopProgramButton->setEnabled(true);
    }
    if(!dev.size()){
        ui->StartProgramButton->setEnabled(true);
    }
}

void MainWindow::makePlot_2D(){
    if(timer4active == 1){
        timer4->stop();
        timer4active = 0;
    }

    ui->customPlot_2D->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming

    if(ui->xaxis_2d_check->isChecked()){
        ui->customPlot_2D->axisRect()->setRangeZoom(Qt::Horizontal);
    }
    if(ui->yaxis_2d_check->isChecked()){
        ui->customPlot_2D->axisRect()->setRangeZoom(Qt::Vertical);
    }
    if(ui->yaxis_2d_check->isChecked() && ui->xaxis_2d_check->isChecked()){
        ui->customPlot_2D->axisRect()->setRangeZoom(Qt::Vertical | Qt::Horizontal);
    }
    ui->customPlot_2D->axisRect()->setupFullAxesBox(true);
    ui->customPlot_2D->xAxis->setLabel("x");
    ui->customPlot_2D->yAxis->setLabel("y");

    if (!has2Dgraph){
        colorMap = new QCPColorMap(ui->customPlot_2D->xAxis, ui->customPlot_2D->yAxis);

//        tracer2d->setGraph(ui->customPlot_2D->graph(0));
        has2Dgraph = true;
    }

    int nx = 256;
    int ny = 256;
    colorMap->data()->setSize(nx, ny);
        colorMap->data()->setRange(QCPRange(0, 256), QCPRange(0, 256));
    double x, y, z;
//    std::cout<<"plot2d_channel_first:  "<<plot2d_channel_first<<"  plot2d_channel_second:   "<<plot2d_channel_second<<"  plot2d_coherency_time:  "<< plot2d_coherency_time << endl;
    int **buffer_2d = data_buffer.buffer_2D(plot2d_channel_first,plot2d_channel_second,plot2d_coherency_time);

    for (int xIndex=0; xIndex<nx; ++xIndex)
    {
      for (int yIndex=0; yIndex<ny; ++yIndex)
      {
        colorMap->data()->cellToCoord(xIndex, yIndex, &x, &y);
        z=static_cast<double>(*(buffer_2d+xIndex)[yIndex]);
        colorMap->data()->setCell(xIndex, yIndex, z);
      }
    }
    for(int i=0;i<4096;i++){
        delete [] *(buffer_2d+i);
    }

    if (!has2Dgraph2){
        colorScale = new QCPColorScale(ui->customPlot_2D);
        marginGroup = new QCPMarginGroup(ui->customPlot_2D);
        has2Dgraph2 = true;
        ui->customPlot_2D->plotLayout()->addElement(0, 1, colorScale);
    }
//    ui->customPlot_2D->plotLayout()->addElement(0, 1, colorScale);
    colorScale->setType(QCPAxis::atRight);
    colorMap->setColorScale(colorScale);
    colorScale->axis()->setLabel("2Channel");

    colorMap->setGradient(QCPColorGradient::gpPolar);
    colorMap->rescaleDataRange();

    ui->customPlot_2D->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    if(ui->rescale_2d_check->isChecked()){
        ui->customPlot_2D->rescaleAxes();
    }
    ui->customPlot_2D->replot();
}
int y000[32768]={0};
long int y111[32768]={0};
int y222[32768]={0};
long int test[32768]={0};

void MainWindow::makePlot()
{
    unsigned long long channelnum1dint = ui->ChannelNumber1D->value();

    for (int i=0; i<ChannelsViewNumbers; i++){
        //Milad-calling data_buffer
        y000[i] = data_buffer.histograms[(channelnum1dint-1)][i];
    }
    if (!hasgraph0){
        ui->customPlot->addGraph();
        tracer->setGraph(ui->customPlot->graph(0));
        hasgraph0 = true;
    }
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    QVector<double> x(ChannelsViewNumbers), y0(ChannelsViewNumbers);

    for (int i=0; i<ChannelsViewNumbers; ++i)
    {
      x[i] = i;
      y0[i] = y000[i];
    }
    ui->customPlot->graph(0)->setData(x, y0);

    if(ui->rescale_1d->isChecked()){
        ui->customPlot->graph(0)->rescaleAxes();
    }
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    if (y000[20] < 1000){
        ui->customPlot->yAxis->setRange(0, 1000);
    }else if (y000[20] > 1000 && y000[20] < 10000) {
        ui->customPlot->yAxis->setRange(0, 10000);
    }else if(y000[20] > 10000){
        ui->customPlot->yAxis->setRange(0, 100000);
    }
    ui->customPlot->replot();

    //______________ Next


    for (int i=0; i<ChannelsViewNumbers; i++){

        y000[i] = data_buffer.histograms[0][i];
    }
    if (!hasgraph1){
        ui->customPlot_Channel1->addGraph();
        tracer1->setGraph(ui->customPlot_Channel1->graph(0));
        hasgraph1 = true;
    }

    ui->customPlot_Channel1->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot_Channel1->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    QVector<double> x1(ChannelsViewNumbers), y10(ChannelsViewNumbers);
    for (int i=0; i<ChannelsViewNumbers; ++i)
    {
      x1[i] = i;
      y10[i] = y000[i];
    }
    ui->customPlot_Channel1->graph(0)->setData(x1, y10);

    ui->customPlot_Channel1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);


    if(ui->rescale_channels->isChecked()){
        ui->customPlot_Channel1->graph(0)->rescaleAxes();
    }

    ui->customPlot_Channel1->replot();


    //______________ Next

    for (int i=0; i<ChannelsViewNumbers; i++){

        y000[i] = data_buffer.histograms[1][i];
    }

    if (!hasgraph2){
        ui->customPlot_Channel2->addGraph();
        tracer2->setGraph(ui->customPlot_Channel2->graph(0));
        hasgraph2 = true;
    }
    ui->customPlot_Channel2->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot_Channel2->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    QVector<double> x2(ChannelsViewNumbers), y20(ChannelsViewNumbers);
    for (int i=0; i<ChannelsViewNumbers; ++i)
    {
      x2[i] = i;
      y20[i] = y000[i];
    }
    ui->customPlot_Channel2->graph(0)->setData(x2, y20);
    ui->customPlot_Channel2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    if(ui->rescale_channels->isChecked()){
        ui->customPlot_Channel2->graph(0)->rescaleAxes();
    }
    ui->customPlot_Channel2->replot();

    //______________ Next



    for (int i=0; i<ChannelsViewNumbers; i++){

        y000[i] = data_buffer.histograms[2][i];
    }
    if (!hasgraph3){
        ui->customPlot_Channel3->addGraph();
        tracer3->setGraph(ui->customPlot_Channel3->graph(0));
        hasgraph3 = true;
    }
    ui->customPlot_Channel3->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot_Channel3->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    QVector<double> x3(ChannelsViewNumbers), y30(ChannelsViewNumbers);
    for (int i=0; i<ChannelsViewNumbers; ++i)
    {
      x3[i] = i;
      y30[i] = y000[i];
    }
    ui->customPlot_Channel3->graph(0)->setData(x3, y30);
    ui->customPlot_Channel3->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    if(ui->rescale_channels->isChecked()){
        ui->customPlot_Channel3->graph(0)->rescaleAxes();
    }
    ui->customPlot_Channel3->replot();


    //______________ Next

    for (int i=0; i<ChannelsViewNumbers; i++){

        y000[i] = data_buffer.histograms[3][i];
    }
    if (!hasgraph4){
        ui->customPlot_Channel4->addGraph();
        tracer4->setGraph(ui->customPlot_Channel4->graph(0));
        hasgraph4 = true;
    }
    ui->customPlot_Channel4->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot_Channel4->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    QVector<double> x4(ChannelsViewNumbers), y40(ChannelsViewNumbers);
    for (int i=0; i<ChannelsViewNumbers; ++i)
    {
      x4[i] = i;
      y40[i] = y000[i];
    }
    ui->customPlot_Channel4->graph(0)->setData(x4, y40);
    ui->customPlot_Channel4->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    if(ui->rescale_channels->isChecked()){
        ui->customPlot_Channel4->graph(0)->rescaleAxes();
    }
    ui->customPlot_Channel4->replot();



    //______________ Next


    for (int i=0; i<ChannelsViewNumbers; i++){

        y000[i] = data_buffer.histograms[4][i];
    }
    if (!hasgraph5){
        ui->customPlot_Channel5->addGraph();
        tracer5->setGraph(ui->customPlot_Channel5->graph(0));
        hasgraph5 = true;
    }
    ui->customPlot_Channel5->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot_Channel5->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    QVector<double> x5(ChannelsViewNumbers), y50(ChannelsViewNumbers);
    for (int i=0; i<ChannelsViewNumbers; ++i)
    {
      x5[i] = i;
      y50[i] = y000[i];
    }
    ui->customPlot_Channel5->graph(0)->setData(x5, y50);

    if(ui->rescale_channels->isChecked()){
        ui->customPlot_Channel5->graph(0)->rescaleAxes();
    }
    ui->customPlot_Channel5->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->customPlot_Channel5->replot();


    //______________ Next


    for (int i=0; i<ChannelsViewNumbers; i++){

        y000[i] = data_buffer.histograms[5][i];
    }
    if (!hasgraph6){
        ui->customPlot_Channel6->addGraph();
        tracer6->setGraph(ui->customPlot_Channel6->graph(0));
        hasgraph6 = true;
    }
    ui->customPlot_Channel6->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot_Channel6->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    QVector<double> x6(ChannelsViewNumbers), y60(ChannelsViewNumbers);
    for (int i=0; i<ChannelsViewNumbers; ++i)
    {
      x6[i] = i;
      y60[i] = y000[i]; // exponentially decaying cosine
    }
    ui->customPlot_Channel6->graph(0)->setData(x6, y60);

    if(ui->rescale_channels->isChecked()){
        ui->customPlot_Channel6->graph(0)->rescaleAxes();
    }
    ui->customPlot_Channel6->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->customPlot_Channel6->replot();


    //______________ Next



    for (int i=0; i<ChannelsViewNumbers; i++){

        y000[i] = data_buffer.histograms[6][i];
    }
    if (!hasgraph7){
        ui->customPlot_Channel7->addGraph();
        tracer7->setGraph(ui->customPlot_Channel7->graph(0));
        hasgraph7 = true;
    }
    ui->customPlot_Channel7->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot_Channel7->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    QVector<double> x7(ChannelsViewNumbers), y70(ChannelsViewNumbers);
    for (int i=0; i<ChannelsViewNumbers; ++i)
    {
      x7[i] = i;
      y70[i] = y000[i];
    }
    ui->customPlot_Channel7->graph(0)->setData(x7, y70);

    if(ui->rescale_channels->isChecked()){
        ui->customPlot_Channel7->graph(0)->rescaleAxes();
    }
    ui->customPlot_Channel7->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->customPlot_Channel7->replot();



    //______________ Next


    for (int i=0; i<ChannelsViewNumbers; i++){

        y000[i] = data_buffer.histograms[7][i];
    }
    if (!hasgraph8){
        ui->customPlot_Channel8->addGraph();
        tracer8->setGraph(ui->customPlot_Channel8->graph(0));
        hasgraph8 = true;
    }
    ui->customPlot_Channel8->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot_Channel8->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    QVector<double> x8(ChannelsViewNumbers), y80(ChannelsViewNumbers);
    for (int i=0; i<ChannelsViewNumbers; ++i)
    {
      x8[i] = i;
      y80[i] = y000[i];
    }
    ui->customPlot_Channel8->graph(0)->setData(x8, y80);

    if(ui->rescale_channels->isChecked()){
        ui->customPlot_Channel8->graph(0)->rescaleAxes();
    }
    ui->customPlot_Channel8->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->customPlot_Channel8->replot();

    //________________Next


    for (int i=0; i<ChannelsViewNumbers; i++){

        y000[i] = data_buffer.histograms[(channelnum1dint-1)][i];
        y111[i] = static_cast<long int>(y000[i]);
    }
    if(!hassmoothgraph){
        tracersmoothed->setGraph(ui->customPlot_smoothed->graph(0));
        hassmoothgraph = true;
    }
    quadraticCubicSmoothing(y111,test,smooth_point_number,ChannelsViewNumbers);

    ui->customPlot_smoothed->addGraph();
    ui->customPlot_smoothed->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot_smoothed->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    QVector<double> xS(ChannelsViewNumbers), yS0(ChannelsViewNumbers);
    for (int i=0; i<ChannelsViewNumbers; ++i)
    {
      xS[i] = i;
      yS0[i] = test[i];
    }
    ui->customPlot_smoothed->graph(0)->setData(xS, yS0);

    if(ui->rescale_smooth_check->isChecked()){
        ui->customPlot_smoothed->graph(0)->rescaleAxes();
    }
    ui->customPlot_smoothed->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    if (yS0[20] < 1000){
        ui->customPlot_smoothed->yAxis->setRange(0, 1000);
    }else if (yS0[20] > 1000 && yS0[20] < 10000) {
        ui->customPlot_smoothed->yAxis->setRange(0, 10000);
    }else if(yS0[20] > 10000){
        ui->customPlot_smoothed->yAxis->setRange(0, 100000);
    }
    ui->customPlot_smoothed->replot();



    if(ui->xaxis_1d->isChecked()){
        ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal);
    }
    if(ui->xaxis_smooth_check->isChecked()){
        ui->customPlot_smoothed->axisRect()->setRangeZoom(Qt::Horizontal);
    }
    if(ui->xaxis_channels_check->isChecked()){
        ui->customPlot_Channel1->axisRect()->setRangeZoom(Qt::Horizontal);
        ui->customPlot_Channel2->axisRect()->setRangeZoom(Qt::Horizontal);
        ui->customPlot_Channel3->axisRect()->setRangeZoom(Qt::Horizontal);
        ui->customPlot_Channel4->axisRect()->setRangeZoom(Qt::Horizontal);
        ui->customPlot_Channel5->axisRect()->setRangeZoom(Qt::Horizontal);
        ui->customPlot_Channel6->axisRect()->setRangeZoom(Qt::Horizontal);
        ui->customPlot_Channel7->axisRect()->setRangeZoom(Qt::Horizontal);
        ui->customPlot_Channel8->axisRect()->setRangeZoom(Qt::Horizontal);
    }

    if(ui->yaxis_1d->isChecked()){
        ui->customPlot->axisRect()->setRangeZoom(Qt::Vertical);
    }
    if(ui->yaxis_smooth_check->isChecked()){
        ui->customPlot_smoothed->axisRect()->setRangeZoom(Qt::Vertical);
    }
    if(ui->yaxis_channels_check->isChecked()){
        ui->customPlot_Channel1->axisRect()->setRangeZoom(Qt::Vertical);
        ui->customPlot_Channel2->axisRect()->setRangeZoom(Qt::Vertical);
        ui->customPlot_Channel3->axisRect()->setRangeZoom(Qt::Vertical);
        ui->customPlot_Channel4->axisRect()->setRangeZoom(Qt::Vertical);
        ui->customPlot_Channel5->axisRect()->setRangeZoom(Qt::Vertical);
        ui->customPlot_Channel6->axisRect()->setRangeZoom(Qt::Vertical);
        ui->customPlot_Channel7->axisRect()->setRangeZoom(Qt::Vertical);
        ui->customPlot_Channel8->axisRect()->setRangeZoom(Qt::Vertical);
    }


    if(ui->yaxis_1d->isChecked() && ui->xaxis_1d->isChecked()){
        ui->customPlot->axisRect()->setRangeZoom(Qt::Vertical | Qt::Horizontal);
    }
    if(ui->xaxis_smooth_check->isChecked() && ui->yaxis_smooth_check->isChecked()){
        ui->customPlot_smoothed->axisRect()->setRangeZoom(Qt::Vertical | Qt::Horizontal);
    }
    if(ui->yaxis_channels_check->isChecked() && ui->xaxis_channels_check->isChecked()){
        ui->customPlot_Channel1->axisRect()->setRangeZoom(Qt::Vertical | Qt::Horizontal);
        ui->customPlot_Channel2->axisRect()->setRangeZoom(Qt::Vertical | Qt::Horizontal);
        ui->customPlot_Channel3->axisRect()->setRangeZoom(Qt::Vertical | Qt::Horizontal);
        ui->customPlot_Channel4->axisRect()->setRangeZoom(Qt::Vertical | Qt::Horizontal);
        ui->customPlot_Channel5->axisRect()->setRangeZoom(Qt::Vertical | Qt::Horizontal);
        ui->customPlot_Channel6->axisRect()->setRangeZoom(Qt::Vertical | Qt::Horizontal);
        ui->customPlot_Channel7->axisRect()->setRangeZoom(Qt::Vertical | Qt::Horizontal);
        ui->customPlot_Channel8->axisRect()->setRangeZoom(Qt::Vertical | Qt::Horizontal);
    }

}
void MainWindow::makeAlgrithmedPlot(){
    unsigned long long channelnum1dint = ui->ChannelNumber1D->value();
    for (int i=0; i<ChannelsViewNumbers; i++){
        y000[i] = data_buffer.histograms[(channelnum1dint-1)][i];
        y111[i] = static_cast<long int>(y000[i]);
    }

    y111[190] = 10000;
    y111[100] = 15000;
    y111[220] = 9000;
    if(!hassmoothgraph){
        tracersmoothed->setGraph(ui->customPlot_Algorithmes->graph(0));
        hassmoothgraph = true;
    }
    PeakSearchFirstDerivative(y111,test,FirstDrive_Num,ChannelsViewNumbers,y222);

    ui->customPlot_Algorithmes->addGraph();
    ui->customPlot_Algorithmes->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot_Algorithmes->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    ui->customPlot_Algorithmes->addGraph();
    ui->customPlot_Algorithmes->graph(1)->setLineStyle(QCPGraph::lsImpulse);
    ui->customPlot_Algorithmes->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssStar, 12));
    ui->customPlot_Algorithmes->graph(1)->setPen(QPen(Qt::red));
    QVector<double> xS(ChannelsViewNumbers), yS0(ChannelsViewNumbers), yS2, jS;

    for (int i=0; i<ChannelsViewNumbers; ++i)
    {
      xS[i] = i;
      yS0[i] = y111[i];
      if(y222[i] != 0){
         yS2.push_back(y111[y222[i]]);
         jS.push_back(y222[i]);
      }
    }
    ui->customPlot_Algorithmes->graph(0)->setData(xS, yS0);
    ui->customPlot_Algorithmes->graph(1)->setData(jS, yS2);
    ui->customPlot_Algorithmes->graph(0)->rescaleAxes();
//    ui->customPlot_Algorithmes->graph(1)->rescaleAxes(true);
    ui->customPlot_Algorithmes->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    if (yS0[20] < 1000){
        ui->customPlot_Algorithmes->yAxis->setRange(0, 1000);
    }else if (yS0[20] > 1000 && yS0[20] < 10000) {
        ui->customPlot_Algorithmes->yAxis->setRange(0, 10000);
    }else if(yS0[20] > 10000){
        ui->customPlot_Algorithmes->yAxis->setRange(0, 100000);
    }
    ui->customPlot_Algorithmes->replot();

}

void MainWindow::on_Button_Draw1D_clicked()
{

    if(timer4active == 1){
        timer4->stop();
        timer4active = 0;
    }
    ui->tab_1D_2D->setCurrentIndex(2);

    unsigned long long contentvalue = ui->Device1_Channel1_Counternum->value();
    if( contentvalue != 0){
        active_dev_channel[0][0] = true;
        sample_limits[0][0] = contentvalue;
    }

    if(offlinedrawing == true){
        std::cout<<"khdaffffezzzzzzzz";
    }else{
        std::cout<<"zalaaaaaaaaaaam";
        dev[0]->open_device();
        dev[0]->set();
        dev[0]->start();
        //libusb_device_handle *dev_handle = dev[0]->device_handle;
        QtConcurrent::run(get_x_data_samples,std::ref(data_buffer),dev[0],sample_limits[0][0]);
    }

    timer->start(500); //time specified in ms
    timeractive = 1;
    if(!tracer_is_defiende){
        tracer = new QCPItemTracer(ui->customPlot);
        tracer2d = new QCPItemTracer(ui->customPlot_2D);
        tracersmoothed = new QCPItemTracer(ui->customPlot_smoothed);
        tracer1 = new QCPItemTracer(ui->customPlot_Channel1);
        tracer2 = new QCPItemTracer(ui->customPlot_Channel2);
        tracer3 = new QCPItemTracer(ui->customPlot_Channel3);
        tracer4 = new QCPItemTracer(ui->customPlot_Channel4);
        tracer5 = new QCPItemTracer(ui->customPlot_Channel5);
        tracer6 = new QCPItemTracer(ui->customPlot_Channel6);
        tracer7 = new QCPItemTracer(ui->customPlot_Channel7);
        tracer8 = new QCPItemTracer(ui->customPlot_Channel8);
        tracer_is_defiende=true;
    }

    timer2->start(500);
    timer2active = 1;

    ui->ReStartPlotting->setEnabled(false);
    ui->StopRePlotting->setEnabled(true);
    ui->tab_1D->setEnabled(false);
}

void MainWindow::checkexport(){
    if(requestexport == 1){

        if(timeractive == 1){
            timer->stop();
            timeractive = 0;
        }
        if(timer2active == 1){
            timer2->stop();
            timer2active = 0;
        }
        doexport();
    }
}
void MainWindow::doexport(){

    int channelnumberfor1d = ui->ChannelNumber1D->value();
    QString strexported = "";
    starttt = StartExportPoint;
    enddd = EndExportPoint;
    if(EndExportPoint < StartExportPoint){
        starttt = EndExportPoint;
        enddd = StartExportPoint;
    }
    for (int i=starttt; i<=enddd; i++){
        tobeexported[i] = data_buffer.histograms[channelnumberfor1d][i];
        strexported += QString::number(tobeexported[i]);
        strexported += "\r\n";
    }

    bool ok;
    QInputDialog* inputDialog = new QInputDialog();
    inputDialog->setOptions(QInputDialog::NoButtons);

    QString text =  inputDialog->getText(NULL ,"Define Export FileName",
                                          "FileName:", QLineEdit::Normal,
                                          QDir::home().dirName(), &ok);

     if (ok && !text.isEmpty())
     {
        export_selected_name = text.toStdString().c_str();
     }

    QString filename="c:\\"+export_selected_name+".txt";
    QFile file(filename);
    if ( file.open(QIODevice::WriteOnly) )
    {
        QTextStream stream( &file );
        stream <<  "from " + QString::number(starttt) + "to " + QString::number(enddd) << ":\r\n" << strexported ;
    }else{
        std::cout<<"cannot open!!!";
    }
    file.flush();
    file.close();

}
void MainWindow::on_StopRePlotting_clicked()
{
    if(timer3active == 1){
        timer3->stop();
        timer3active = 0;
    }
    if(timeractive == 1){
        timer->stop();
        timeractive = 0;
    }
    ui->StopRePlotting->setEnabled(false);
    ui->ReStartPlotting->setEnabled(true);
}
void MainWindow::on_ReStartPlotting_clicked()
{

    if(timer3active == 0){
        timer3->start(700);
        timer3active = 1;
    }
    if(timeractive == 0){
        timer->start(700);
        timeractive = 1;
    }
    ui->StopRePlotting->setEnabled(true);
    ui->ReStartPlotting->setEnabled(false);
}
void MainWindow::on_Button_Draw2D_clicked()
{
    ui->tab_1D_2D->setCurrentIndex(4);
    ui->tab_2D->setEnabled(false);
    timer3->start(500); //time specified in ms
    timer3active = 1;
}
void MainWindow::on_StartProgramButton_clicked()
{
    if(data_buffer.buffer[0].size() > 0){
        data_buffer.clear_buffer();}
    offlinedrawing = false;
    ui->OpenBuffer->setEnabled(false);
    ui->SaveEntireBuffer->setEnabled(true);

    ui->tab_1D_2D->setCurrentIndex(0);
    ui->tab_1D->setEnabled(true);
    ui->tab_2D->setEnabled(true);
    dev = initialize_and_select_cypress_devices();
    timer4->start(2000); //time specified in ms
    timer4active = 1;
    ui->StartProgramButton->setEnabled(false);
}

void MainWindow::on_StopProgramButton_clicked()
{
    ui->OpenBuffer->setEnabled(true);
    if(timeractive == 1){
        timer->stop();
        timeractive = 0;
    }
    if(timer3active == 1){
        timer3->stop();
        timer3active = 0;
    }
    ui->tab_1D_2D->setCurrentIndex(0);
    ui->StopRePlotting->setEnabled(false);
    ui->ReStartPlotting->setEnabled(false);
    ui->tab_1D_2D->setEnabled(false);
    ui->StopProgramButton->setEnabled(false);

    if(timer4active == 1){
        timer4->stop();
        timer4active = 0;
    }
    ui->StartProgramButton->setEnabled(true);
//    for(auto devv : dev)
//    {
//        devv->stop();
//    }
//    dev.clear();
}

void MainWindow::slotMouseMove(QMouseEvent *event)
{
    double coordX = ui->customPlot->xAxis->pixelToCoord(event->pos().x());
    double coordX_2d = ui->customPlot_2D->xAxis->pixelToCoord(event->pos().x());
    double coordX_smoothe = ui->customPlot_smoothed->xAxis->pixelToCoord(event->pos().x());
    double coordX1 = ui->customPlot_Channel1->xAxis->pixelToCoord(event->pos().x());
    double coordX2 = ui->customPlot_Channel2->xAxis->pixelToCoord(event->pos().x());
    double coordX3 = ui->customPlot_Channel3->xAxis->pixelToCoord(event->pos().x());
    double coordX4 = ui->customPlot_Channel4->xAxis->pixelToCoord(event->pos().x());
    double coordX5 = ui->customPlot_Channel5->xAxis->pixelToCoord(event->pos().x());
    double coordX6 = ui->customPlot_Channel6->xAxis->pixelToCoord(event->pos().x());
    double coordX7 = ui->customPlot_Channel7->xAxis->pixelToCoord(event->pos().x());
    double coordX8 = ui->customPlot_Channel8->xAxis->pixelToCoord(event->pos().x());
    tracer->setGraphKey(coordX);
    tracersmoothed->setGraphKey(coordX_smoothe);
    tracer2d->setGraphKey(coordX_2d);
    tracer1->setGraphKey(coordX1);
    tracer2->setGraphKey(coordX2);
    tracer3->setGraphKey(coordX3);
    tracer4->setGraphKey(coordX4);
    tracer5->setGraphKey(coordX5);
    tracer6->setGraphKey(coordX6);
    tracer7->setGraphKey(coordX7);
    tracer8->setGraphKey(coordX8);
    ui->lineEdit->setText("x: " + QString::number(tracer->position->key()) +
                          " y: " + QString::number(tracer->position->value()));
    ui->Plot_Value_Channel1->setText("x: " + QString::number(tracer1->position->key()) +
                          " y: " + QString::number(tracer1->position->value()));
    ui->Plot_Value_Channel2->setText("x: " + QString::number(tracer2->position->key()) +
                          " y: " + QString::number(tracer2->position->value()));
    ui->Plot_Value_Channel3->setText("x: " + QString::number(tracer3->position->key()) +
                          " y: " + QString::number(tracer3->position->value()));
    ui->Plot_Value_Channel4->setText("x: " + QString::number(tracer4->position->key()) +
                          " y: " + QString::number(tracer4->position->value()));
    ui->Plot_Value_Channel5->setText("x: " + QString::number(tracer5->position->key()) +
                          " y: " + QString::number(tracer5->position->value()));
    ui->Plot_Value_Channel6->setText("x: " + QString::number(tracer6->position->key()) +
                          " y: " + QString::number(tracer6->position->value()));
    ui->Plot_Value_Channel7->setText("x: " + QString::number(tracer7->position->key()) +
                          " y: " + QString::number(tracer7->position->value()));
    ui->Plot_Value_Channel8->setText("x: " + QString::number(tracer8->position->key()) +
                          " y: " + QString::number(tracer8->position->value()));
    ui->customPlot->replot();
    ui->customPlot_2D->replot();
    ui->customPlot_smoothed->replot();
    ui->customPlot_Channel1->replot();
    ui->customPlot_Channel2->replot();
    ui->customPlot_Channel3->replot();
    ui->customPlot_Channel4->replot();
    ui->customPlot_Channel5->replot();
    ui->customPlot_Channel6->replot();
    ui->customPlot_Channel7->replot();
    ui->customPlot_Channel8->replot();
}

void MainWindow::on_ChannelNumber1D_valueChanged()
{
    makePlot();
}

void MainWindow::on_FirstChannelSelector_currentTextChanged(const QString &arg1)
{
    if(arg1 == "Channel1"){
        plot2d_channel_first = 0;
    }else if (arg1 == "Channel2") {
        plot2d_channel_first = 1;
    }else if (arg1 == "Channel3") {
        plot2d_channel_first = 2;
    }else if (arg1 == "Channel4") {
        plot2d_channel_first = 3;
    }else if (arg1 == "Channel5") {
        plot2d_channel_first = 4;
    }else if (arg1 == "Channel6") {
        plot2d_channel_first = 5;
    }else if (arg1 == "Channel7") {
        plot2d_channel_first = 6;
    }else if (arg1 == "Channel8") {
        plot2d_channel_first = 7;
    }
}

void MainWindow::on_SecondChannelSelector_currentTextChanged(const QString &arg1)
{
    if(arg1 == "Channel1"){
        plot2d_channel_second = 0;
    }else if (arg1 == "Channel2") {
        plot2d_channel_second = 1;
    }else if (arg1 == "Channel3") {
        plot2d_channel_second = 2;
    }else if (arg1 == "Channel4") {
        plot2d_channel_second = 3;
    }else if (arg1 == "Channel5") {
        plot2d_channel_second = 4;
    }else if (arg1 == "Channel6") {
        plot2d_channel_second = 5;
    }else if (arg1 == "Channel7") {
        plot2d_channel_second = 6;
    }else if (arg1 == "Channel8") {
        plot2d_channel_second = 7;
    }
}

void MainWindow::on_tab_2D_CoherencyTime_editingFinished()
{
    plot2d_coherency_time = ui->tab_2D_CoherencyTime->value();
}

void MainWindow::on_smooth_point_num_valueChanged(int arg1)
{
    smooth_point_number = arg1;
}

void MainWindow::on_SaveEntireBuffer_clicked()
{

    bool DoExport;
    QInputDialog* inputDialog2 = new QInputDialog();
    inputDialog2->setOptions(QInputDialog::NoButtons);

    QString text2 =  inputDialog2->getText(NULL ,"Define Export FileName",
                                          "FileName:", QLineEdit::Normal,
                                          QDir::home().dirName(), &DoExport);

     if (DoExport && !text2.isEmpty())
     {
        export_Entire_Buffer = text2.toStdString().c_str();
     }

    QString filename2 = "c:\\"+export_Entire_Buffer+".fck";
    QFile file2(filename2);
    if (file2.open(QIODevice::WriteOnly)){
        QDataStream streamobject (&file2);
        streamobject << data_buffer;
    }
    file2.close();
}

void MainWindow::on_OpenBuffer_clicked()
{

    if(data_buffer.buffer[0].size() > 0){
        data_buffer.clear_buffer();}

    offlinedrawing = true;
//    bool OpenExport;
//    QInputDialog* inputDialog3 = new QInputDialog();
//    inputDialog3->setOptions(QInputDialog::NoButtons);

//    QString text3 =  inputDialog3->getText(NULL ,"To Be Opened FileName",
//                                          "FileName:", QLineEdit::Normal,
//                                          QDir::home().dirName(), &OpenExport);

    QString tobeopenedfileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "C://", tr("Any Files (*)"));

    QString fileextension = tobeopenedfileName.split(".").last();
     if (fileextension == "fck" )
     {
         std::cout<<"filename11"<<tobeopenedfileName.toStdString();
         std::cout<<"fileextension11"<<fileextension.toStdString();
//        QString tobeopened = text3.toStdString().c_str();
//        QString filename3 = "c:\\"+tobeopened+".txt";
        QFile file3(tobeopenedfileName);
        buffers input_buffer;
        if (file3.open(QIODevice::ReadOnly)){
            QDataStream streamobject3 (&file3);
            streamobject3 >> input_buffer;
        }
//        input_buffer.print_histograms();
        data_buffer = input_buffer;
//        data_buffer.print_histograms();
        runonopenedbuffer();
     }
     if(fileextension == "chn"){
         std::cout<<"filename22"<<tobeopenedfileName.toStdString();
         std::cout<<"fileextension22"<<fileextension.toStdString();
         mychn=new CHN(tobeopenedfileName);
//         data_buffer.clear_buffer();
         std::cout<<"data size "<<mychn->data_size<<"\n";
         for(int i=0;i<mychn->data_size;i++){
             data_buffer.histograms[0][i]=mychn->data[i];
             std::cout<<mychn->data[i]<<"\n";
         }
         runonopenedbuffer();
     }
}
void MainWindow::runonopenedbuffer(){

    ui->OpenBuffer->setEnabled(false);
    ui->StartProgramButton->setEnabled(false);
    ui->SaveEntireBuffer->setEnabled(true);
    ui->tab_1D_2D->setCurrentIndex(0);
    ui->tab_1D_2D->setEnabled(true);
    ui->tab_1D->setEnabled(true);
    ui->tab_2D->setEnabled(true);
    ui->Device_1->setEnabled(true);
    ui->Button_Draw1D->setEnabled(true);
    ui->Button_Draw2D->setEnabled(true);
    ui->StopProgramButton->setEnabled(true);

}

void MainWindow::on_Plotsmoothed_clicked()
{
    ui->tab_1D_2D->setCurrentIndex(5);
}
void MainWindow::on_Peak_1stDrive_clicked()
{
    ui->tab_1D_2D->setCurrentIndex(6);
    makeAlgrithmedPlot();
}

void MainWindow::on_FirstDrive_Num_valueChanged(int arg1)
{
    FirstDrive_Num = arg1;
}

void MainWindow::on_DoSigmaLcalc_clicked()
{
    unsigned long long channelnum1dint = ui->ChannelNumber1D->value();
    for (int i=0; i<ChannelsViewNumbers; i++){
        y000[i] = data_buffer.histograms[(channelnum1dint-1)][i];
        y111[i] = static_cast<long int>(y000[i]);
    }
    SigmaLCalc(y111, (ui->Startch_Sigma->value()), (ui->Endch_Sigma->value()), (ui->a_Coef_Sigma->value()));
    ui->SigmaResult->setText(QString::number(FinalSigmaL));
}

void MainWindow::on_DoSigmaHcalc_clicked()
{

    unsigned long long channelnum1dint = ui->ChannelNumber1D->value();
    for (int i=0; i<ChannelsViewNumbers; i++){
        y000[i] = data_buffer.histograms[(channelnum1dint-1)][i];
        y111[i] = static_cast<long int>(y000[i]);
    }
    SigmaHCalc(y111, (ui->Startch_Sigma->value()), (ui->Endch_Sigma->value()), (ui->a_Coef_Sigma->value()));
    ui->SigmaResult->setText(QString::number(FinalSigmaH));
}

void MainWindow::on_ChannelsViewNumber_valueChanged(int arg1)
{
    ChannelsViewNumbers = arg1;
}
