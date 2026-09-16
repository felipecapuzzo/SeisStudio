#include "imageview.hpp"
#include <QPainter>



void ImageView::setData(const float *data, int nx, int nz)
{
    if (!data || nx <= 0 || nz <= 0)
        return;

    // Min/max
    float vmin = data[0];
    float vmax = data[0];

    for (int x = 0; x < nx; ++x)
    {
        for (int z = 0; z < nz; ++z)
        {
            int ind = z + nz * x;
            vmin = std::min(vmin, data[ind]);
            vmax = std::max(vmax, data[ind]);
        }
    }

    // Cria imagem
    image = QImage(nx, nz, QImage::Format_Grayscale8);

    float range = vmax - vmin;

    // Float -> grayscale
    for (int x = 0; x < nx; ++x)
    {
        for (int z = 0; z < nz; ++z)
        {
            int ind = z + nz * x;
            int gray = 128;
            if (range > 0.0f)
            {
                float normalized = (data[ind] - vmin) / range;
                gray = static_cast<int>(255.0f * normalized);
            }

            image.setPixel(x, z, qRgb(gray, gray, gray));
        }
    }

    update();
}

void ImageView::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    if (image.isNull())
        return;

    if (!keepAspectRatio)
    {
        painter.drawImage(rect(), image);
        return;
    }

    QSize targetSize = image.size();
    targetSize.scale(size(), Qt::KeepAspectRatio);

    QRect targetRect(
        (width() - targetSize.width()) / 2,
        (height() - targetSize.height()) / 2,
        targetSize.width(),
        targetSize.height()
    );

    painter.drawImage(targetRect, image);
}

ImageView::ImageView(QWidget* parent) : QWidget(parent){}

void ImageView::setKeepAspectRatio(bool keep)
{
    keepAspectRatio = keep;
    update();
}