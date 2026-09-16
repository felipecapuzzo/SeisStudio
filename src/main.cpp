#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.resize(1600, 900);
    window.setWindowTitle("SeisStudio");

    window.setStyleSheet("background-color: #151a1f;");



    window.show();
  

    return app.exec();
}