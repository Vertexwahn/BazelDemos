#include "window.h"

#include "render_widget.h"

int g_winW;
int g_winH;
RenderWidget * w = nullptr;
QApplication * app = nullptr;

namespace window {

struct HandleImpl {
} DUMMY;

Handle create(unsigned /*winW*/, unsigned /*winH*/, const char* /*name*/) {
    int argc = 1;
    char* argv[] = {const_cast<char*>("MyWindow.exe")};
    app = new QApplication(argc, argv);

    RenderWidgetDescription rwd;
    w = new RenderWidget(rwd);

    //DUMMY = reinterpret_cast<HandleImpl>(rwd.window_id);

	return &DUMMY;
}

void destroy(window::Handle /*wHnd*/) {
    
}

void show(window::Handle /*wHnd*/, bool /*show*/) {

}

void loop(window::Handle wHnd, window::Redraw func) {
    if(w) {
        w->resize(g_winW, g_winH);
        w->show();
    }

    if(app) {
        app->exec();
    }
}

} // end namespace window