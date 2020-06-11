#include <QCoreApplication>
#include "gmath.h"
#include "cicomm.h"
#include <QDebug>
#include <iostream>
#include <QString>
#include <list>
#include <QFile>





#ifdef win32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

typedef struct AxisLab
{
    double X;
    double Y;
    double Z;
    double A;
    double C;


} AxisLab;


bool initParameter(int *addr)
{
    FILE * fp = fopen(CIPATH,"rb");
    if(fp == NULL)
    {
        qDebug()<<CIPATH;
        FILE * fp = fopen(CIPATH,"wb");
        if(fp == NULL)
        {
            return false;
        }
        fprintf(fp,"600,601,602,603#");
        addr[0] = 600;
        addr[1] = 600;
        addr[2] = 600;
        addr[3] = 600;
    }
    else
    {
        fscanf(fp,"%ld,%ld,%ld,%ld#",addr,addr+1,addr+2,addr+3);
    }
    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int m_showDebug = 0;
    if(argc == 2)
    {
        if(QString(argv[1]) == "1")
        {
            m_showDebug = 1;
        }
        if(QString(argv[1]) == "V")
        {
            cout<<"V1.0\n";
        }
    }

    /*获取宏变量*/
    int CIValue[4];
    if(!initParameter(CIValue))
    {
        cout<<"open file error!\n";
    }
    else
    {
        cout<<"open file success!\n";
    }
    int CIkey = CreateCIKernel();


    double FristVector[3];
    double SecondVector[3];

    for(int i = 0; i < 3; i++)
    {
        GetMacroVal(CIkey, THEFIRSTROTATIONAXISDEFLECTIONANGLEVECTOR + i,FristVector[i]);
        GetMacroVal(CIkey, THESECONDROTATIONAXISDEFLECTIONANGLEVECTOR + i,SecondVector[i]);

        if(m_showDebug)
        {
            cout<<"firstVector :"<<FristVector[i]<<","<<SecondVector[i]<<endl;
        }
    }

    double TheFirstRotationAxisNumber,TheSecondRotationAxisNumber;
    GetMacroVal(CIkey, FIRSTAXISOFROTATION ,TheFirstRotationAxisNumber);
    GetMacroVal(CIkey, SECONDAXISOFROTATION ,TheSecondRotationAxisNumber);

    TheFirstRotationAxisNumber+=0.00001;
    TheSecondRotationAxisNumber+=0.00001;
    list<AxisLab>  AxisLablist;

    while (1)
    {
        double mCIstart;
        GetMacroVal(CIkey, CIValue[0],mCIstart);
        mCIstart +=0.00001;
        if((int)mCIstart == 1)
        {
            double mC1;
            GetMacroVal(CIkey, CIValue[1],mC1);
            mC1+=0.00001;
            if((int)mC1 == 1)
            {
                double axis5[5];
                for(int i = 0;i<3;i++)
                {
                    GetMacroVal(CIkey, AXISDATA + i*50,axis5[i]);
                }
                GetMacroVal(CIkey, AXISDATA + ((int)TheFirstRotationAxisNumber-1)*50,axis5[3]);
                GetMacroVal(CIkey, AXISDATA + ((int )TheSecondRotationAxisNumber-1)*50,axis5[4]);

                double douXYZ[3];

                double cahH;
                GetMacroVal(CIkey, CIValue[2],cahH);
                CalDeltaMoveCoord(douXYZ,cahH,FristVector,SecondVector,axis5[3],axis5[4]);

                axis5[0] += douXYZ[0];
                axis5[1] += douXYZ[1];
                axis5[2] += douXYZ[2];

                AxisLab linaxiss;
                linaxiss.X = axis5[0];
                linaxiss.Y = axis5[1];
                linaxiss.Z = axis5[2];
                linaxiss.A = axis5[3];
                linaxiss.C = axis5[4];

                AxisLablist.push_back(linaxiss);

                SetMacroVal(CIkey,CIValue[1],0);
            }

        }
        else if ((int)mCIstart == 2)
        {
            QFile file(NCPATH);
            if(file.open(QIODevice::WriteOnly|QIODevice::Text))
            {
                list<AxisLab >::iterator it;
                it = AxisLablist.begin();
                QString lineStr = "";
                while (it != AxisLablist.end()) {

                    lineStr+="X"+QString::number(it->X);
                    lineStr+=" Y"+QString::number(it->Y);
                    lineStr+=" Z"+QString::number(it->Z);
                    lineStr+=" "+ QString((char)((int )TheFirstRotationAxisNumber+61))+QString::number(it->A);
                    lineStr+=" "+ QString((char)((int)TheSecondRotationAxisNumber+61))+QString::number(it->C);

                    file.write(lineStr.toUtf8());
                    file.write("\n");
                    lineStr.clear();
                    *it++;
                }
            }

            AxisLablist.clear();
            SetMacroVal(CIkey,CIValue[0],0);
        }
        sleep(1);
    }

    return a.exec();
}
