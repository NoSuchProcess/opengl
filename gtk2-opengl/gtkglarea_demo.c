#include <GL/gl.h>
#include <GL/glu.h>
#include <gtk/gtk.h>
#include <gtkgl/gtkglarea.h>

gint glarea_button_press(GtkWidget *, GdkEventButton *);
gint glarea_draw(GtkWidget *, GdkEventExpose *);
gint glarea_reshape(GtkWidget *, GdkEventConfigure *);
gint glarea_init(GtkWidget *);
gint glarea_destroy(GtkWidget *);
int main(int, char **);

gint glarea_button_press(GtkWidget * widget, GdkEventButton * event)
{
	int x = event->x;
	int y = event->y;

	if (event->button == 1) {
		g_print("Button 1 press   (%d, %d)\n", x, y);
		return TRUE;
	}
	if (event->button == 2) {
		g_print("Button 1 press   (%d, %d)\n", x, y);
		return TRUE;
	}
	return FALSE;
}

gint glarea_draw(GtkWidget * widget, GdkEventExpose * event)
{
	if (event->count > 0) {
		return (TRUE);
	}
	g_print("Expose Event\n");
	if (gtk_gl_area_make_current(GTK_GL_AREA(widget))) {
		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		/* */
		/* Insert your drawing code here. */
		/* */
		gtk_gl_area_swapbuffers(GTK_GL_AREA(widget));
	}
	return (TRUE);
}

gint glarea_reshape(GtkWidget * widget, GdkEventConfigure * event)
{
	int w = widget->allocation.width;
	int h = widget->allocation.height;

	g_print("Reshape Event\n");
	if (gtk_gl_area_make_current(GTK_GL_AREA(widget))) {
		/* This is an example 2D reshape function. Writing reshape */
		/* functions is beyond the scope of this demo. Check the */
		/* red book or the www.opengl.org for more information on */
		/* how to write reshape code to suit your needs.  */
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(-(w >> 1), (w >> 1), -(h >> 1), h >> 1);
		glMatrixMode(GL_MODELVIEW);
	}
	return (TRUE);
}

gint glarea_init(GtkWidget * widget)
{
	g_print("Realize Event\n");
	if (gtk_gl_area_make_current(GTK_GL_AREA(widget))) {
		/* Insert your OpenGL initialization code here */
	}
	return TRUE;
}

gint glarea_destroy(GtkWidget * widget)
{
	g_print("GTK GL Area Destroy Event\n");
	/* Insert any required cleanup */
	/* code here.  */
	return TRUE;
}

int main(int argc, char **argv)
{
	GtkWidget *window;
	GtkWidget *button_quit;
	GtkWidget *box_main;
	GtkWidget *glarea;

	/* These attributes are passed to glXChooseVisual by the */
	/* gdk (see gdk_gl_choose_visual in gdkgl.c from the GtkGlarea distro).  */
	int attrlist[] = {
		GDK_GL_RGBA,
		GDK_GL_DOUBLEBUFFER,
		GDK_GL_DEPTH_SIZE, 1,
		GDK_GL_NONE
	};
	gtk_init(&argc, &argv);
	if (gdk_gl_query() == FALSE) {
		g_print("OpenGL not supported!\n");
		return (1);
	}

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "GtkGLArea Demo For Blogger");
	gtk_quit_add_destroy(1, GTK_OBJECT(window));
	gtk_signal_connect(GTK_OBJECT(window), "delete_event", GTK_SIGNAL_FUNC(gtk_main_quit), NULL);
	gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(gtk_main_quit), NULL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_widget_show(window);

	box_main = gtk_vbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(window), box_main);
	gtk_widget_show(box_main);

	glarea = gtk_gl_area_new(attrlist);
	gtk_widget_set_events(GTK_WIDGET(glarea), GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK);
	gtk_signal_connect(GTK_OBJECT(glarea), "button_press_event", GTK_SIGNAL_FUNC(glarea_button_press), NULL);
	gtk_signal_connect(GTK_OBJECT(glarea), "expose_event", GTK_SIGNAL_FUNC(glarea_draw), NULL);
	gtk_signal_connect(GTK_OBJECT(glarea), "configure_event", GTK_SIGNAL_FUNC(glarea_reshape), NULL);
	gtk_signal_connect(GTK_OBJECT(glarea), "realize", GTK_SIGNAL_FUNC(glarea_init), NULL);
	gtk_signal_connect(GTK_OBJECT(glarea), "destroy", GTK_SIGNAL_FUNC(glarea_destroy), NULL);
	gtk_widget_set_size_request(glarea, 256, 256);
	gtk_box_pack_start(GTK_BOX(box_main), glarea, TRUE, TRUE, 0);
	gtk_widget_show(glarea);

	button_quit = gtk_button_new_with_label("Quit");
	gtk_signal_connect(GTK_OBJECT(button_quit), "clicked", GTK_SIGNAL_FUNC(gtk_main_quit), NULL);
	gtk_box_pack_start(GTK_BOX(box_main), button_quit, FALSE, TRUE, 0);
	gtk_widget_show(button_quit);

	gtk_main();

	return (0);
}
