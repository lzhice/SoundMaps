#include "circularseries.h"
#include "eDSP/include/utility/vector.h"

CircularSeries::CircularSeries(QObject *parent) : ArraySeries (parent)
{
}

void CircularSeries::init() {
    eDSP::utility::set(std::begin(m_y), std::end(m_y), 0);
    eDSP::utility::linspace(std::begin(m_x), std::end(m_x), 0, m_x.size());

    m_axisX = {m_x[0], m_x[m_x.size() - 1]};
    m_axisY = {-1, 1};
    emit axisMinXChanged(m_axisX.first);
    emit axisMaxXChanged(m_axisY.second);
    emit axisMinYChanged(m_axisY.first);
    emit axisMaxYChanged(m_axisX.second);
}
