#pragma once

#include <QWidget>
#include <QImage>

class ImageView : public QWidget
{
public:
    ImageView(QWidget* parent = nullptr);

    void setData(const float* data, int nx, int nz);
    void setKeepAspectRatio(bool keep);
    
    bool keepAspectRatio = false;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QImage image;
};

