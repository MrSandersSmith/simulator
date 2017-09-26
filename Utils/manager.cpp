#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/keysymdef.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <Scene.h>
#include <manager.h>

#define X 300
#define Y 50
#define WIDTH 1024
#define HEIGHT 768
#define WIDTH_MIN 640
#define HEIGHT_MIN 480

#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092

#define FPS 30
#define SKIP_TICKS (1000 / FPS)

namespace Simulator {

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

// Helper to check for extension string presence.  Adapted from:
//   http://www.opengl.org/resources/features/OGLextensions/
static bool isExtensionSupported(const char *extList, const char *extension)
{
	const char *start;
	const char *where, *terminator;

	/* Extension names should not have spaces. */
	where = strchr(extension, ' ');
	if (where || *extension == '\0')
		return false;

	/* It takes a bit of care to be fool-proof about parsing the
		 OpenGL extensions string. Don't be fooled by sub-strings,
		 etc. */
	for (start = extList;;) {
		where = strstr(start, extension);

		if (!where)
			break;

		terminator = where + strlen(extension);

		if ( where == start || *(where - 1) == ' ' )
			if ( *terminator == ' ' || *terminator == '\0' )
				return true;

		start = terminator;
	}

	return false;
}

static bool ctxErrorOccurred = false;
static int ctxErrorHandler( Display *dpy, XErrorEvent *ev )
{
	ctxErrorOccurred = true;
	return 0;
}

double get_mseconds() {
	timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return (double) (t.tv_sec * 1000 + t.tv_nsec / 1000000);
	// printf("CLOCK_MONOTONIC res: [%d]sec [%d]nsec\n", ts.tv_sec, ts.tv_nsec);
}


/* main_loop - основной цикл программы */

int main_loop(int argc, char *argv[], Simulator::Scene *scene) {

	/* Устанавливаем связь с X сервером */
	Display *display = XOpenDisplay(NULL); // Указатель на структуру Display

	if (display == NULL ) {
		puts("Can not connect to the X server!\n");
		return 1;
	}

	// Get a matching FB config
	static int visual_attribs[] =
	{
		GLX_X_RENDERABLE    , True,
		GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
		GLX_RENDER_TYPE     , GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
		GLX_RED_SIZE        , 8,
		GLX_GREEN_SIZE      , 8,
		GLX_BLUE_SIZE       , 8,
		GLX_ALPHA_SIZE      , 8,
		GLX_DEPTH_SIZE      , 24,
		GLX_STENCIL_SIZE    , 8,
		GLX_DOUBLEBUFFER    , True,
		//GLX_SAMPLE_BUFFERS  , 1,
		//GLX_SAMPLES         , 4,
		None
	};

	int glx_major, glx_minor;

	// FBConfigs were added in GLX version 1.3.
	if ( !glXQueryVersion( display, &glx_major, &glx_minor ) ||
			( ( glx_major == 1 ) && ( glx_minor < 3 ) ) || ( glx_major < 1 ) )
	{
		printf("Invalid GLX version");
		exit(1);
	}

	printf( "Getting matching framebuffer configs\n" );
	int fbcount;
	GLXFBConfig* fbc = glXChooseFBConfig(display, DefaultScreen(display), visual_attribs, &fbcount);
	if (!fbc)
	{
		printf( "Failed to retrieve a framebuffer config\n" );
		exit(1);
	}
	printf( "Found %d matching FB configs.\n", fbcount );

	// Pick the FB config/visual with the most samples per pixel
	printf( "Getting XVisualInfos\n" );
	int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

	int i;
	for (i = 0; i < fbcount; ++i)
	{
		XVisualInfo *vi = glXGetVisualFromFBConfig( display, fbc[i] );
		if ( vi )
		{
			int samp_buf, samples;
			glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
			glXGetFBConfigAttrib( display, fbc[i], GLX_SAMPLES       , &samples  );

			printf( "  Matching fbconfig %d, visual ID 0x%2lu: SAMPLE_BUFFERS = %d,"
					" SAMPLES = %d\n",
					i, vi -> visualid, samp_buf, samples );

			if ( best_fbc < 0 || samp_buf && samples > best_num_samp )
				best_fbc = i, best_num_samp = samples;
			if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp )
				worst_fbc = i, worst_num_samp = samples;
		}
		XFree( vi );
	}

	GLXFBConfig bestFbc = fbc[ best_fbc ];

	// Be sure to free the FBConfig list allocated by glXChooseFBConfig()
	XFree( fbc );

	// Get a visual
	XVisualInfo *vi = glXGetVisualFromFBConfig( display, bestFbc );
	printf( "Chosen visual ID = 0x%lu\n", vi->visualid );

	printf( "Creating colormap\n" );
	XSetWindowAttributes windowAttribs;
	Colormap cmap;
	windowAttribs.colormap = cmap = XCreateColormap( display,
										   RootWindow( display, vi->screen ),
										   vi->visual, AllocNone );
	windowAttribs.background_pixmap	= None ;
	windowAttribs.background_pixel	= WhitePixel(display, vi->screen);
	windowAttribs.border_pixel		= 0;
	windowAttribs.override_redirect	= true;
	windowAttribs.event_mask		= StructureNotifyMask;

	printf( "Creating window\n" );
	Window window = XCreateWindow( display, RootWindow( display, vi->screen ),
								   X, Y, WIDTH, HEIGHT, 0, vi->depth, InputOutput, vi->visual,
								   CWBackPixel | CWBorderPixel | CWColormap | CWEventMask, &windowAttribs );
	if (!window) {
		printf( "Failed to create window.\n" );
		exit(1);
	}

	// Done with the visual info data
	XFree( vi );
	XStoreName( display, window, "Simulator" );

	// Get the default screen's GLX extension list
	const char *glxExts = glXQueryExtensionsString( display,
						  DefaultScreen( display ) );

	// NOTE: It is not necessary to create or make current to a context before
	// calling glXGetProcAddressARB
	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
								 glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );

	GLXContext ctx = 0;

	// Install an X error handler so the application won't exit if GL 3.0
	// context allocation fails.
	//
	// Note this error handler is global.  All display connections in all threads
	// of a process use the same error handler, so be sure to guard against other
	// threads issuing X commands while this code is running.
	ctxErrorOccurred = false;
	int (*oldHandler)(Display*, XErrorEvent*) =
		XSetErrorHandler(&ctxErrorHandler);

	// Check for the GLX_ARB_create_context extension string and the function.
	// If either is not present, use GLX 1.3 context creation method.
	if ( !isExtensionSupported( glxExts, "GLX_ARB_create_context" ) ||
			!glXCreateContextAttribsARB )
	{
		printf( "glXCreateContextAttribsARB() not found"
				" ... using old-style GLX context\n" );
		ctx = glXCreateNewContext( display, bestFbc, GLX_RGBA_TYPE, 0, True );
	}

	// If it does, try to get a GL 3.0 context!
	else
	{
		int context_attribs[] =
		{
			GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
			GLX_CONTEXT_MINOR_VERSION_ARB, 0,
			//GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			None
		};

		printf( "Creating context\n" );
		ctx = glXCreateContextAttribsARB( display, bestFbc, 0,
										  True, context_attribs );

		// Sync to ensure any errors generated are processed.
		XSync( display, False );
		if ( !ctxErrorOccurred && ctx )
			printf( "Created GL 3.0 context\n" );
		else
		{
			// Couldn't create GL 3.0 context.  Fall back to old-style 2.x context.
			// When a context version below 3.0 is requested, implementations will
			// return the newest context version compatible with OpenGL versions less
			// than version 3.0.
			// GLX_CONTEXT_MAJOR_VERSION_ARB = 1
			context_attribs[1] = 1;
			// GLX_CONTEXT_MINOR_VERSION_ARB = 0
			context_attribs[3] = 0;

			ctxErrorOccurred = false;

			printf( "Failed to create GL 3.0 context"
					" ... using old-style GLX context\n" );
			ctx = glXCreateContextAttribsARB( display, bestFbc, 0,
											  True, context_attribs );
		}
	}

	// Sync to ensure any errors generated are processed.
	XSync( display, False );

	// Restore the original error handler
	XSetErrorHandler( oldHandler );

	if ( ctxErrorOccurred || !ctx )
	{
		printf( "Failed to create an OpenGL context\n" );
		exit(1);
	}

	// Verifying that context is a direct context
	if ( !glXIsDirect ( display, ctx ) )
	{
		printf( "Indirect GLX rendering context obtained\n" );
	}
	else
	{
		printf( "Direct GLX rendering context obtained\n" );
	}

	printf("Making context current\n");
	glXMakeCurrent(display, window, ctx);
	printf("GL_VERSION: %s\n", glGetString( GL_VERSION ));

	Atom atomWmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(display, window, &atomWmDeleteWindow, 1);

	XClearWindow(display, window);
	XMapRaised(display, window);

	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glXSwapBuffers(display, window);

	XSelectInput(display, window, ExposureMask | ResizeRedirectMask | KeyPressMask | KeymapStateMask | StructureNotifyMask);

	// Таймер
	double prevTime, curTime, deltaTime;
	prevTime = get_mseconds();

	/* Создадим цикл получения и обработки событий */
	XEvent event;
	char str[25] = {0};
	KeySym keysym = 0;
	int len = 0;
	bool running = true;

	while (running) {
		XNextEvent(display, &event);

		switch (event.type) {

			case ResizeRequest:
				// printf("event.xresizerequest.width:%d event.xresizerequest.height:%d\n", event.xresizerequest.width, event.xresizerequest.height);
				scene->resize(event.xresizerequest.width, event.xresizerequest.height);
				scene->update();
				glXSwapBuffers(display, window);
				// XClearWindow(display, window);
				break;

			case Expose:
				/* Запрос на перерисовку */

				// XWindowAttributes attribs;
				// XGetWindowAttributes(display, window, &attribs);
				// scene->resize(attribs.width, attribs.height);
				scene->update();
				glXSwapBuffers(display, window);
				break;

			case KeymapNotify:
				XRefreshKeyboardMapping(&event.xmapping);
				break;

			case KeyPress:
				// len = XLookupString(&event.xkey, str, 25, &keysym, NULL);

				// if (len > 0) {
				// 	printf("Key pressed: %s - %s - %s\n", str, len, keysym);
				// }
				// if (keysym == XK_Escape) {
				// 	running = false;
				// }
				break;

			case ClientMessage:
				if (event.xclient.data.l[0] == atomWmDeleteWindow) {
					running = false;
				}
				break;

			case DestroyNotify:
				running = false;
				break;
		}

		curTime = get_mseconds();
		deltaTime = curTime - prevTime;

		if (fabs(deltaTime - SKIP_TICKS) < 0.001) {
			prevTime = curTime;
			// XClearWindow(display, window);
			scene->update();
			glXSwapBuffers(display, window);
		}
	}

	glXMakeCurrent( display, 0, 0 );
	glXDestroyContext( display, ctx );
	XFreeColormap( display, cmap );
	XDestroyWindow( display, window );
	XCloseDisplay( display );

	return 0;
}

}