#ifndef WINDOWOUTPUT_H
#define WINDOWOUTPUT_H

#include <QDialog>

namespace Ui {
class WindowOutput;
}

class WindowOutput : public QDialog
{
    Q_OBJECT

public:
    explicit WindowOutput(QWidget *parent = nullptr);
    ~WindowOutput();

    void passItemList(QString arg);

private:
    Ui::WindowOutput *ui;
};

#endif // WINDOWOUTPUT_H
