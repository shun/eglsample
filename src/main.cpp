
#include <stdio.h>
#include <unistd.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

static Display* xdisp;
static Window window;
 
EGLDisplay eglDisplay;
EGLContext eglContext;
EGLSurface eglSurface;

int gwidth = 720;
int gheight = 720;
GLuint programObject = 0;

int initializeEGL()
{
	eglDisplay = eglGetDisplay( (EGLNativeDisplayType) xdisp );
	if( eglDisplay == EGL_NO_DISPLAY ) {
		fprintf( stderr, "NO EGL DISPLAY\n" );
		return -1;
	}
	if( !eglInitialize( eglDisplay, NULL, NULL ) ) {
		fprintf( stderr, "Unable to initialize EGL\n" );
		return -1;
	}
 
	EGLint attr[] = {
		EGL_BUFFER_SIZE, 16,
		EGL_RENDERABLE_TYPE,
		EGL_OPENGL_ES2_BIT,
		EGL_NONE,
	};
	EGLConfig cfg;
	EGLint numConfigs;
	if( !eglChooseConfig( eglDisplay, attr, &cfg, 1, &numConfigs ) ) {
		fprintf( stderr, "Failed to choose config.\n" );
		return -1;
	}
	if( numConfigs != 1 ) {
		fprintf( stderr, "exactly one config. (%d)\n", numConfigs );
		return -1;
	}
 
	eglSurface = eglCreateWindowSurface( eglDisplay, cfg, window, NULL );
	if( eglSurface == EGL_NO_SURFACE ) {
		fprintf( stderr, "Unable to create surface (eglError=%d)\n", eglGetError() );
		return -1;
	}
 
	EGLint ctxattr[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE,
	};
	eglContext = eglCreateContext( eglDisplay, cfg, EGL_NO_CONTEXT, ctxattr );
	if( eglContext == EGL_NO_CONTEXT ) {
		fprintf( stderr, "Unable to create EGL context. (%d)\n", eglGetError() );
		return -1;
	}
	eglMakeCurrent( eglDisplay, eglSurface, eglSurface, eglContext );

	printf("VENDER     : %s\n", glGetString(GL_VENDOR));
	printf("RENDERER   : %s\n", glGetString(GL_RENDERER));
	printf("VERSION    : %s\n", glGetString(GL_VERSION));
	printf("EXTENTIONS : %s\n", glGetString(GL_EXTENSIONS));
				
	return 0;
}

GLuint loadShader ( GLenum type, const char *shaderSrc ) {

    GLuint shader;

    shader = glCreateShader ( type );
    glShaderSource ( shader, 1, &shaderSrc, NULL ); 
    glCompileShader ( shader );

    GLint compiled;
    glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

    return shader;
}

int initializeShader()
{
	GLbyte vShaderStr[] = 
        "attribute vec4 vPosition; \n"
        "void main() \n"
        "{ \n"
        " gl_Position = vPosition; \n"
		"} \n";

	GLbyte fShaderStr[] = 
		"precision mediump float;\n"
		"void main() \n"
		"{ \n"
		" gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
		"} \n";

    GLuint vertexShader;
    GLuint fragmentShader;

    vertexShader = loadShader( GL_VERTEX_SHADER, (const char*)vShaderStr );
    fragmentShader = loadShader( GL_FRAGMENT_SHADER, (const char*)fShaderStr );

    programObject = glCreateProgram();

    glAttachShader ( programObject, vertexShader );
    glAttachShader ( programObject, fragmentShader );

    glBindAttribLocation ( programObject, 0, "vPosition" );
    glLinkProgram ( programObject );
}

void draw_triangle()
{

    GLfloat vtx[] = {
		 0.0f,  0.5f, 0.0f, 
        -0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f
    };

    glViewport ( 0, 0, gwidth, gheight );

	glClearColor( 0.0f, 1.0f, 0.0f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glUseProgram ( programObject );

    glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vtx );
    glEnableVertexAttribArray ( 0 );
    glDrawArrays ( GL_TRIANGLES, 0, 3 );

	eglSwapBuffers( eglDisplay, eglSurface );
}

int main(int argc, char* argv[])
{
	xdisp = XOpenDisplay(NULL);
	if (NULL == xdisp)
	{
		printf("### [ERROR]: fail xdisp\n");
	}

	Window rootWin = DefaultRootWindow( xdisp );
 
	int screen = DefaultScreen( xdisp );
	unsigned long white = WhitePixel( xdisp, screen );
	unsigned long black = BlackPixel( xdisp, screen );
 
	window = XCreateSimpleWindow( xdisp, rootWin,
								  0, 0,
								  gwidth, gheight,
								  2,
								  black, white );
 
 
	XSelectInput( xdisp, window, KeyPressMask );
 
	XMapWindow( xdisp, window );
 
	if( initializeEGL() < 0 )
	{
		return -1;
	}

	if (initializeShader() < 0)
	{
		return -1;
	}
	draw_triangle();

	sleep(3);
	eglDestroyContext( eglDisplay, eglContext );
	eglDestroySurface( eglDisplay, eglSurface );
	eglTerminate( eglDisplay );
 
	XDestroyWindow( xdisp, window );
	XCloseDisplay( xdisp );
 
	return 0;

}
