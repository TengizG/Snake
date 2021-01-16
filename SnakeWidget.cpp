#include "SnakeWidget.h"
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QTime>
#include <QDebug>



SnakeWidget::SnakeWidget(QWidget *parent)
    : QWidget(parent)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    fillGrids();
    isFood = false;
    direction = QPoint(1, 0);
    snake.push_back(QPoint(0, 0));
    directionMap[0][0] = direction;
    gameGrid[0][0] = SnakePart;

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(updateSnakeMovement()));
    interval = 250;
    timer->start(interval);

    score = 0;
    title = "Score: ";
    setWindowTitle(title + QString::number(score));

}

SnakeWidget::~SnakeWidget()
{
}

void SnakeWidget::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    drawMap(&painter);

}

void SnakeWidget::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    switch (key) {
    case Qt::Key_Down:
        if (direction == QPoint(0, -1))
            return;
        direction = QPoint(0, 1);
        break;
    case Qt::Key_Up:
        if (direction == QPoint(0, 1))
            return;
        direction = QPoint(0, -1);
        break;
    case Qt::Key_Right:
        if (direction == QPoint(-1, 0))
            return;
        direction = QPoint(1, 0);
        break;
    case Qt::Key_Left:
        if (direction == QPoint(1, 0))
            return;
        direction = QPoint(-1, 0);
       break;
    default:
        QWidget::keyPressEvent(event);
    }

}



void SnakeWidget::drawMap(QPainter *painter)
{
    double cellWidth = width() / double (WIDTH);
    double cellHeight = height() / double (HEIGHT);

    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < HEIGHT; ++j) {
            if (gameGrid[i][j] == SnakePart)
                drawSnake(painter, i, j, cellWidth, cellHeight);
            else if(gameGrid[i][j] == Food)
                drawFood(painter);
        }
    }

}

void SnakeWidget::drawSnake(QPainter *painter, int x, int y, int w, int h)
{
    painter->setBrush(Qt::green);
    painter->setRenderHint(QPainter::Antialiasing, true);
    for (int i = 0; i < snake.size(); ++i) {
        if (snake[i].x() == x && snake[i].y() == y)
            painter->drawEllipse(snake[i].x() * w, snake[i].y() * h,
                          w, h);
    }

}

void SnakeWidget::drawFood(QPainter *painter)
{
    double cellWidth = width() / double (WIDTH);
    double cellHeight = height() / double (HEIGHT);

    painter->setBrush(Qt::white);

    painter->drawRect(foodPosition.x() * cellWidth, foodPosition.y() * cellHeight,
                      cellWidth, cellHeight);

}

void SnakeWidget::generateFood()
{


    if (!isFood) {
        while (true) {
            int foodPosX = randGenerator(WIDTH);
            int foodPosY = randGenerator(HEIGHT);

            bool colision = false;
            for (int i = 0; i < snake.size(); ++i)
            {
                if (foodPosX == snake[i].x() && foodPosY == snake[i].y()) {
                    colision = true;
                    break;
                }
            }

            if (!colision) {
                gameGrid[foodPosX][foodPosY] = Food;
                foodPosition = QPoint(foodPosX, foodPosY);
                break;
            }
        }
        isFood = true;
    }


}

void SnakeWidget::updateSnakeMovement()
{
   generateFood();
   directionMap[snake[0].x()][snake[0].y()] = direction;
   foodColission();

   for (int i = 0; i < snake.size(); ++i) {
       QPoint dir = directionMap[snake[i].x()][snake[i].y()];
       gameGrid[snake[i].x()][snake[i].y()] = Empty;
       snake[i].rx() += dir.x();
       snake[i].ry() += dir.y();
       gameGrid[snake[i].x()][snake[i].y()] = SnakePart;
   }  
   colision();
   update();

}

void SnakeWidget::fillGrids()
{
    for (int i = 0; i < WIDTH; ++i)
        for (int j = 0; j < HEIGHT; ++j) {
            gameGrid[i][j] = Empty;
            directionMap[i][j] = QPoint(0, 0);
        }
}

void SnakeWidget::foodColission()
{
    if (snake[0] == foodPosition) {
        QPoint tailPoint = snake[snake.size() - 1];
        QPoint tailDirecion = directionMap[tailPoint.x()][tailPoint.y()];

        QPoint newPart = QPoint(tailPoint.x() - tailDirecion.x(), tailPoint.y() - tailDirecion.y());
        snake.push_back(newPart);
        directionMap[newPart.x()][newPart.y()] = tailDirecion;

        score += 10;
        setWindowTitle(title + QString::number(score));
        timer->setInterval(interval -= 5);
        isFood = false;
    }
}

int SnakeWidget::randGenerator(int number) {
    QTime time;
    int seed = time.currentTime().second();

    QRandomGenerator rand;
    rand.seed(seed);
    int x = rand.bounded(number);
    return x;
}

void SnakeWidget::colision()
{
    for (int i = 0; i < snake.size(); ++i) {
        if (snake[0] == snake[i] && i > 0) {
            clearMap();
            break;
        }
    }

    if (snake[0].x() < 0 || snake[0].y() < 0 ||
            snake[0].x() >= WIDTH || snake[0].y() >= HEIGHT)
        clearMap();

}

void SnakeWidget::clearMap()
{
    score = 0;
    timer->setInterval(250);
    interval = 250;
    snake.clear();
    snake.push_back(QPoint(0, 0));
    direction = QPoint(1, 0);
    fillGrids();
    isFood = false;

}










