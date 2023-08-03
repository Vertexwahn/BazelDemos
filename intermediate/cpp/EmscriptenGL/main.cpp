// Note: This demo is based on the blog post
// "WebGL with Emscripten: The Bare Minimum"
// which can be found at https://blog.feather.systems/WebGL.html

#include <emscripten/html5.h>
#include <GLES2/gl2.h>

int main(){
	// setting up EmscriptenWebGLContextAttributes
	EmscriptenWebGLContextAttributes attr;
	emscripten_webgl_init_context_attributes(&attr);
	attr.alpha = 0;

	// target the canvas selector
	EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("#canvas", &attr);
	emscripten_webgl_make_context_current(ctx);
    
    // this goes after you have activated the WebGL context
	glClearColor(0.984f, 0.4627f, 0.502f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	return 1;
}
