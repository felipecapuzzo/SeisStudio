#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>

#include "geometry.hpp"
#include "model.hpp"
#include "imageview.hpp"
#include "acoustic_fd.hpp"
#include "parameters.hpp"
#include "sourcetimefunction.hpp"
#include "seismogram.hpp"
#include "io.hpp"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget main_window;
    main_window.resize(1600, 900);
    main_window.setWindowTitle("SeisStudio");

    main_window.setStyleSheet("background-color: #151a1f;");

    
    RegularGrid grid;
    grid.nz = 201;
    grid.nx = 301;
    grid.dx = 10;
    grid.dz = 10;

    SimulationParameters param;

    param.dt = 0.001;
    param.tmax = 1.0f;

    size_t n_samples_total = grid.nz * grid.nx;

    AcousticWavefield u(grid);

    AcousticModel model(grid);
    model.fill(2000);

    auto wavelet = SourceTimeFunction::Ricker(param.n_time_steps(), param.dt, 30, 0.1);

    // geometry

    // sources positions

    ShotGeometry shot;
    shot.sources.add({1000.0f, 1500.0f});

    // receiver line
    for (float x = 500.0f; x <= 2500.0f; x += 10.0f)
    {
        shot.receivers.add({400.0f, x});
    }

    size_t n_rec = shot.receivers.size();

    // float* seismogram = new float[param.n_time_steps() * n_rec()];
    Seismogram seismogram(501, shot.receivers.size(), 0.002);

    AcousticFDSolver forward_modeling(model, param);

    float* snapshot = new float[grid.nx * grid.nz]();


    forward_modeling.RunShot(shot, wavelet, seismogram,snapshot);

    io::ExportFloat32("teste.bin",grid.nx*grid.nz,model.vp);
    io::ExportFloat32("wavelet.bin",param.n_time_steps(),wavelet.signal);

    float sum;
    for (int i = 0; i < param.n_time_steps(); i++)
    {
        sum += wavelet.signal[i];
    }
    std::cout<<sum<<" "<<param.n_time_steps()<<std::endl;

    ImageView view;
    view.setData(snapshot, grid.nx, grid.nz);
    view.keepAspectRatio = true;

    QVBoxLayout layout(&main_window);
    layout.addWidget(&view);

    main_window.show();

    return app.exec();

}