#ifndef OBJ_H
#define OBJ_H

#include <QObject>
#include <QPoint>
#include <QVector>

class Obj : public QObject
{
    Q_OBJECT
public:
    explicit Obj(QObject *parent = nullptr);

    QVector<QPoint> internalPoits() const;
    void setInternalPoits(const QVector<QPoint> &internalPoits);

    QVector<QPoint> contourPointns() const;
    void setContourPointns(const QVector<QPoint> &contourPointns);

    void pushInternalPoint(QPoint point);
    void pushInternalPoint(int x, int y);

    void pushContourPoint(QPoint point);
    void pushContourPoint(int x, int y);

signals:

private:
    int _id;
    QVector<QPoint> _internalPoits;
    QVector<QPoint> _contourPointns;

public slots:
};

#endif // OBJ_H
