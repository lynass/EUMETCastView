#ifndef SEGMENTLIST_H
#define SEGMENTLIST_H

#include <QObject>
#include <QPainter>
#include <QDebug>
#include <QPen>
#include <QFutureWatcher>
#include "globals.h"

class Segment;

class SegmentList : public QObject
{
    Q_OBJECT

public:

    explicit SegmentList(QObject *parent = 0);
    void CalculateSunPosition(double first_julian, double last_julian, QVector3D *sunPosition);
    void SetNbrOfVisibleSegments(int nbr);
    int GetNbrOfVisibleSegments();
    void SetIndexFirstVisible(int cnt) { indexfirstvisible = cnt; }
    void SetIndexLastVisible(int cnt) { indexlastvisible = cnt; }
    void SetTotalSegmentsInDirectory(long nbr) { TotalSegmentsInDirectory = nbr; }
    int GetTotalSegmentsInDirectory() { return TotalSegmentsInDirectory; }
    void SetDirectoryName(QString its) { directoryname = its; }
    QString GetDirectoryName() { return directoryname; }
    QList<Segment *> *GetSegmentlistptr(void) { return &segmentlist; }
    QList<Segment *> *GetSegsSelectedptr(void) { return &segsselected; }
    eSegmentType GetSegmentType() { return seglisttype; }
    void ClearSegments();
    int NbrOfSegments();
    int NbrOfSegmentsSelected();
    int NbrOfSegmentsSelectedinMemory();
    int NbrOfSegmentsShown();
    int NbrOfSegmentLinesSelected();

    void GetFirstLastVisible( double *first_julian,  double *last_julian);
    void GetFirstLastVisible( QDateTime *first_date,  QDateTime *last_date);
    void GetFirstLastVisibleFilename( QString *first_filename,  QString *last_filename);
    void RenderEarthLocationsGL();
    void ShowSegment(int value);
    void ShowWinvec(QPainter *painter, float distance, const QMatrix4x4 modelview);

    bool ComposeImage();
    void ComposeImage1();
    bool TestForSegment(double *deg_lon, double *deg_lat, bool leftbuttondown, bool showallsegments);
    void RenderSegments(QPainter *painter, QColor col, bool renderall);
    int NbrOfEartviewsPerScanline();
    void ComposeGVProjection(int inputchannel);
    void ComposeLCCProjection(int inputchannel);
    void ComposeSGProjection(int inputchannel);
    void SmoothProjectionImageBilinear();
    void SmoothProjectionImageBicubic();
    void Compose48bitProjectionPNG(QString fileName, bool mapto65535);


    static void doReadSegmentInMemory(Segment *t);
    static void doComposeSegmentImage(Segment *t);
    //static void doComposeGVProjection(Segment *t);
    int GetTotalNbrOfLines() { return this->totalnbroflines; }
    int GetEartViewsPerScanline() { return this->earth_views_per_scanline; }
    bool TestForSegmentGL(int x, int realy, float distance, const QMatrix4x4 &m, bool showallsegments, QString &segmentname);
    bool TestForSegmentGLextended(int x, int realy, float distance, const QMatrix4x4 &m, bool showallsegments, QString &segmentname);

protected:
    void BilinearInterpolation(Segment *segm, bool combine);
    void BilinearInterpolation12bits(Segment *segm);
    void BilinearBetweenSegments(Segment *segmfirst, Segment *segmnext, bool combine);
    void BilinearBetweenSegments12bits(Segment *segmfirst, Segment *segmnext);
    bool bhm_line(int x1, int y1, int x2, int y2, QRgb rgb1, QRgb rgb2, QRgb *canvas, int dimx);
    bool bhm_line12bits(int x1, int y1, int x2, int y2, quint16 col1[3], quint16 col2[3], quint16 *canvasred, quint16 *canvasgreen, quint16 *canvasblue, quint8 *canvasalpha, int dimx);
    void MapInterpolation(QRgb *canvas, quint16 dimx, quint16 dimy);
    void MapInterpolation1(QRgb *canvas, quint16 dimx, quint16 dimy);
    void MapInterpolation12bits(quint16 *canvasred, quint16 *canvasgreen, quint16 *canvasblue, quint8 *canvasalpha, quint16 dimx, quint16 dimy);

    void MapCanvas(QRgb *canvas, qint32 anchorX, qint32 anchorY, quint16 dimx, quint16 dimy, bool combine);
    void MapCanvas12bits(quint16 *canvasred, quint16 *canvasgreen, quint16 *canvasblue, quint8 *canvasalpha, qint32 anchorX, qint32 anchorY, quint16 dimx, quint16 dimy);

    double cubicInterpolate (double p[4], double x);
    double bicubicInterpolate (double p[4][4], double x, double y);

    qint32 Min(const qint32 v11, const qint32 v12, const qint32 v21, const qint32 v22);
    qint32 Max(const qint32 v11, const qint32 v12, const qint32 v21, const qint32 v22);

    int nbrofvisiblesegments;
    int indexfirstvisible;
    int indexlastvisible;
    int earth_views_per_scanline;
    int totalnbroflines;

    QString segmenttype;
    eSegmentType seglisttype;

    QString directoryname;
    long TotalSegmentsInDirectory;
    long stat_max_ch[5];
    long stat_min_ch[5];
    long stat_max_norm_ch[3];
    long stat_min_norm_ch[3];
    long stat_max_proj_ch[3];
    long stat_min_proj_ch[3];

    int progressresultready; // for progresscounter
    int projectioninputchannel;
    bool channel_3_select;

    QList<bool> bandlist;
    QList<int> colorlist;
    QList<bool> inverselist;


signals:
    void segmentlistfinished(bool settoolboxbuttons);
    void segmentprojectionfinished(bool settoolboxbuttons);
    void progressCounter(int);

public slots:

protected:
    QList<Segment *> segmentlist;
    QList<Segment *> segsselected;
    QFutureWatcher<void> *watcherread;
    QFutureWatcher<void> *watchercompose;
    //QFutureWatcher<void> *watchercomposeprojection;
    float scale;

protected slots:
    void readfinished();
    void composefinished();
    void progressvaluechanged(int segmentnbr);
    void resultcomposeisready(int segmentnbr);
//    void composeprojectionfinished();
    void composeprojectionreadyat(int segmentnbr);


};

#endif // SEGMENTLIST_H
