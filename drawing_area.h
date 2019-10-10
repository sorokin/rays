#ifndef DRAWING_AREA_H
#define DRAWING_AREA_H

#include <QLabel>
#include <QTimer>
#include <chrono>

class drawing_area : public QLabel
{
    Q_OBJECT
public:
    drawing_area(QWidget* parent = nullptr);

private:
    void paintEvent(QPaintEvent*) override;

private:
    QTimer timer;
    std::chrono::high_resolution_clock::time_point start_time;
};

#endif // DRAWING_AREA_H
