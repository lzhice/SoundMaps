#include "circularbufferseries.h"
#include <QXYSeries>
#include <QtDebug>
QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)
CircularBufferSeries::CircularBufferSeries(QObject *parent) : QObject(parent), m_series(new QLineSeries(parent))
{

}

void CircularBufferSeries::setSize(int size) {
    m_data.resize(size);
    for (int i=0, size = m_data.size(); i<size; i++) {
        m_data[i].setX(i);
        m_data[i].setY(0.0);
    }
    emit sizeChanged(size);
}

void CircularBufferSeries::appendBuffer(const float* data, ulong size) {
    Q_ASSERT_X(static_cast<ulong>(m_data.size()) <= size,
               __FUNCTION__,
               "Trying to append a buffer with a size higher than the current one");

    const int dataSize = m_data.size();
    for (int i=size; i<dataSize; i++) {
        m_data[i].setY(m_data[i - size].y());
    }

    for (int i=dataSize - size; i <dataSize; i++) {
        m_data[i].setY(data[i - (dataSize - size)] );
    }
}


void CircularBufferSeries::appendBuffer(const QVector<double> &data) {
    Q_ASSERT_X(m_data.size() <= data.size(),
               __FUNCTION__,
               "Trying to append a buffer with a size higher than the current one");

    const int size = data.size();
    const int dataSize = m_data.size();
    for (int i=size; i<dataSize; i++) {
        m_data[i].setY(m_data[i - size].y());
    }

    for (int i=dataSize - size; i <dataSize; i++) {
        m_data[i].setY(100 * data[i - (dataSize - size)] );
    }

}

void CircularBufferSeries::update() {
    m_series->replace(m_data);
}

void CircularBufferSeries::setSeries(QXYSeries *series) {
    m_series = series;
    m_series->setUseOpenGL(true);
    emit seriesChanged(series);
}

