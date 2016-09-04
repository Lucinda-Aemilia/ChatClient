#ifndef TALKWIDGET_H
#define TALKWIDGET_H

#include <QWidget>

namespace Ui {
class TalkWidget;
}

class TalkWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TalkWidget(QWidget *parent = 0);
    ~TalkWidget();

private:
    Ui::TalkWidget *ui;
};

#endif // TALKWIDGET_H
