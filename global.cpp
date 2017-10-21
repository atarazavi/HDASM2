#include "global.h"
#include <qstring.h>

int StartExportPoint = 0;
int EndExportPoint = 0;
int tobeexported[256] = {0};
int requestexport = 0;
bool hasgraph0 = false;
bool hassmoothgraph = false;
bool hasgraph1 = false;
bool hasgraph2 = false;
bool hasgraph3 = false;
bool hasgraph4 = false;
bool hasgraph5 = false;
bool hasgraph6 = false;
bool hasgraph7 = false;
bool hasgraph8 = false;
bool has2Dgraph = false;
bool has2Dgraph2 = false;
bool has2Dgraph3 = false;
QString export_selected_name = "export";
QString export_Entire_Buffer = "exportObject";
int smooth_point_number = 5;
int FirstDrive_Num = 5;
double FinalSigmaH = 0;
double FinalSigmaL = 0;
//unsigned long long channelnum1dint = 0;
