#ifndef SNAKE_H
#define SNAKE_H

#include <QWidget>
#include <QVariant>

#define WIDTH 20
#define HEIGHT 20



class SnakeWidget : public QWidget
{
    Q_OBJECT

public:
    SnakeWidget(QWidget *parent = nullptr);
    ~SnakeWidget();

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void updateSnakeMovement();


private:
    void drawMap(QPainter *);
    void drawSnake(QPainter *, int, int, int, int);
    void drawFood(QPainter*);
    void fillGrids(); // starting empty grids
    void generateFood();
    void foodColission();
    int  randGenerator(int);
    void colision();
    void clearMap();

    enum {Empty = 0, SnakePart, Food};
    int gameGrid[WIDTH][HEIGHT];
    QPoint directionMap[WIDTH][HEIGHT];
    QVector<QPoint> snake;
    QPoint foodPosition;
    QTimer *timer;
    int interval;
    bool isFood;
    QPoint direction;
    QString title;
    int score;

};
#endif // SNAKE_H
