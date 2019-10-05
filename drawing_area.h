#ifndef DRAWING_AREA_H
#define DRAWING_AREA_H

#include <QLabel>

class drawing_area : public QLabel
{
    Q_OBJECT
public:
    drawing_area(QWidget* parent = nullptr);

private:
    void paintEvent(QPaintEvent*) override;
};

#endif // DRAWING_AREA_H
