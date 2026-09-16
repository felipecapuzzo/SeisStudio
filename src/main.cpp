#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>

#include "geometry.hpp"
#include "model.hpp"
#include "imageview.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget main_window;
    main_window.resize(1600, 900);
    main_window.setWindowTitle("SeisStudio");

    main_window.setStyleSheet("background-color: #151a1f;");

    RegularGrid grid;
    grid.nx = 101;
    grid.nz = 201;
    grid.dx = 10.0f;
    grid.dz = 10.0f;

    AcousticModel model(grid);
    model.fill(2000.0f);

    for (size_t z = 0; z < 101; z++)
    {
        for (size_t x = 0; x < grid.nx; x++)
        {
            size_t index = grid.index(z, x);
            model.vp[index] = 1500.0f + 10.0f * z;
           
        }
    }

    ImageView view;
    view.setData(model.vp,grid.nx,grid.nz);
    view.keepAspectRatio = true;



    QVBoxLayout layout(&main_window);
    layout.addWidget(&view);

    main_window.show();
  

    return app.exec();
}