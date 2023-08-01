#pragma once

#include "QtCore/QThread"
#include "QtWidgets/QApplication"
#include "QtWidgets/QLabel"
#include "QtWidgets/QStylePainter"
#include "QtCore/QTimer"

struct RenderWidgetDescription {
    int width = 1;
    int height = 1;
    void *window_id = nullptr;
};

class RenderWidget : public QWidget
{
    Q_OBJECT;
public:
    RenderWidget(RenderWidgetDescription& rwd) {
        setAttribute(Qt::WA_PaintOnScreen);
		//setAttribute(Qt::WA_MSWindowsUseDirect3D, true);
		setFocusPolicy(Qt::StrongFocus);

        rwd.window_id = (void*)winId();

        draw_next_frame();
    }

    virtual ~RenderWidget() {

    }

    virtual void paintEvent(QPaintEvent * e)
    {
        
    }

public Q_SLOTS:
	void draw_next_frame() {
		repaint();

		QThread::msleep(2);
		QTimer::singleShot(0, this, SLOT(draw_next_frame()));
	}

private:

};