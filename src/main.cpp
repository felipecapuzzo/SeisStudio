#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <algorithm>

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

    ComputationalGrid computational_grid(grid, 100, true);

    SimulationParameters param;

    param.dt = 0.001;
    param.tmax = 1.0f;

    size_t n_samples_total = grid.nz * grid.nx;

    AcousticWavefield u(computational_grid);

    AcousticModel model(computational_grid);
    model.fill(2000);

    auto wavelet = SourceTimeFunction::Ricker(param.n_time_steps(), param.dt, 30, 0.1);

    // geometry

    // sources positions

    ShotGeometry shot;
    shot.sources.add({100.0f, 1500.0f});

    // receiver line
    for (float x = 500.0f; x <= 2500.0f; x += 10.0f)
    {
        shot.receivers.add({400.0f, x});
    }

    size_t n_rec = shot.receivers.size();

    const size_t n_iterations = param.n_time_steps();
    const size_t rec_ratio = std::max<size_t>(1, get_ratio(0.002f, param.dt));
    const size_t n_seismogram_samples = n_iterations == 0 ? 0 : 1 + (n_iterations - 1) / rec_ratio;

    Seismogram seismogram(n_seismogram_samples, shot.receivers.size(), 0.002);

    AcousticFDSolver forward_modeling(model, param);

    float* snapshot = new float[computational_grid.nx * computational_grid.nz]();


    forward_modeling.RunShot(shot, wavelet, seismogram,snapshot);

    io::ExportFloat32("teste.bin", computational_grid.nx * computational_grid.nz, model.vp);
    io::ExportFloat32("wavelet.bin",param.n_time_steps(),wavelet.signal);

    
    std::cout<< computational_grid.nz<<" "<<computational_grid.nx<<" "<<param.n_time_steps()<<std::endl;

    ImageView view;
    view.setData(snapshot, computational_grid.nx, computational_grid.nz);
    view.keepAspectRatio = true;

    QVBoxLayout layout(&main_window);
    layout.addWidget(&view);

    main_window.show();

    return app.exec();

}
