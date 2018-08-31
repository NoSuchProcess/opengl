#include <stdlib.h>
#include <gtk/gtk.h>
#include <gtk/gtkgl.h>
#include <GL/gl.h>
#include <GL/glu.h>

static void realize(GtkWidget * widget, gpointer data)
{
	GdkGLContext *glcontext = gtk_widget_get_gl_context(widget);
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(widget);

	if (!gdk_gl_drawable_gl_begin(gldrawable, glcontext))
		return;

	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClearDepth(1.0);
	glViewport(0, 0, widget->allocation.width, widget->allocation.height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, 1.0, 1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -3.0);

	gdk_gl_drawable_gl_end(gldrawable);
}

static gboolean configure_event(GtkWidget * widget, GdkEventConfigure * event, gpointer data)
{
	GdkGLContext *glcontext = gtk_widget_get_gl_context(widget);
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(widget);

	if (!gdk_gl_drawable_gl_begin(gldrawable, glcontext))
		return FALSE;
	glViewport(0, 0, widget->allocation.width, widget->allocation.height);
	gdk_gl_drawable_gl_end(gldrawable);

	return TRUE;
}

static gboolean expose_event(GtkWidget * widget, GdkEventExpose * event, gpointer data)
{
	GdkGLContext *glcontext = gtk_widget_get_gl_context(widget);
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(widget);

	if (!gdk_gl_drawable_gl_begin(gldrawable, glcontext))
		return FALSE;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (gdk_gl_drawable_is_double_buffered(gldrawable))
		gdk_gl_drawable_swap_buffers(gldrawable);
	else
		glFlush();
	gdk_gl_drawable_gl_end(gldrawable);

	return TRUE;
}

int main(int argc, char *argv[])
{
	GdkGLConfig *glconfig;
	gint major, minor;
	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *drawing_area;
	GtkWidget *button;

	gtk_init(&argc, &argv);
	gtk_gl_init(&argc, &argv);
	gdk_gl_query_version(&major, &minor);
	g_print("\nOpenGL extension version - %d.%d\n", major, minor);

	glconfig = gdk_gl_config_new_by_mode(GDK_GL_MODE_RGB | GDK_GL_MODE_DEPTH | GDK_GL_MODE_DOUBLE);
	if (glconfig == NULL) {
		glconfig = gdk_gl_config_new_by_mode(GDK_GL_MODE_RGB | GDK_GL_MODE_DEPTH);
		if (glconfig == NULL)
			exit(1);
	}

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "GtkGLExt Demo For Blogger");
	gtk_container_set_reallocate_redraws(GTK_CONTAINER(window), TRUE);
	g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(gtk_main_quit), NULL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_widget_show(window);

	vbox = gtk_vbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show(vbox);

	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request(drawing_area, 256, 256);
	gtk_widget_set_gl_capability(drawing_area, glconfig, NULL, TRUE, GDK_GL_RGBA_TYPE);
	g_signal_connect_after(G_OBJECT(drawing_area), "realize", G_CALLBACK(realize), NULL);
	g_signal_connect(G_OBJECT(drawing_area), "configure_event", G_CALLBACK(configure_event), NULL);
	g_signal_connect(G_OBJECT(drawing_area), "expose_event", G_CALLBACK(expose_event), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);
	gtk_widget_show(drawing_area);

	button = gtk_button_new_with_label("Quit");
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_main_quit), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	gtk_widget_show(button);

	gtk_main();

	return 0;
}
