#ifndef PUBLIC_H
#define PUBLIC_H

#include <QFrame>

class Line : public QFrame{
    Q_OBJECT

public:
    explicit Line(QWidget *parent = nullptr)
    {
        this->setFixedHeight(1);
        this->setStyleSheet("background:grey");
    };
    ~Line(){};
};

#endif // PUBLIC_H
