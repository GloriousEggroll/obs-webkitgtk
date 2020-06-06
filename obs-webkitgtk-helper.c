/*
 * obs-webkitgtk. OBS Studio source plugin.
 * Copyright (C) 2020 Florian Zwoch <fzwoch@gmail.com>
 *
 * This file is part of obs-webkitgtk.
 *
 * obs-webkitgtk is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * obs-webkitgtk is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with obs-webkitgtk. If not, see <http://www.gnu.org/licenses/>.
 */

#include <webkit2/webkit2.h>

static gboolean capture(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
	cairo_surface_t *surface = gtk_offscreen_window_get_surface(
		GTK_OFFSCREEN_WINDOW(user_data));

	fwrite(cairo_image_surface_get_data(surface),
	       cairo_image_surface_get_stride(surface) *
		       cairo_image_surface_get_height(surface),
	       1, stdout);

	return TRUE;
}

int main(int argc, char **argv)
{
	if (argc != 4) {
		printf("Usage: %s <width> <height> <url>\n", argv[0]);
		return 0;
	}

	gtk_init(&argc, &argv);

	GtkWidget *window = gtk_offscreen_window_new();
	gtk_window_set_default_size(GTK_WINDOW(window), atoi(argv[1]),
				    atoi(argv[2]));

	WebKitWebView *webview = WEBKIT_WEB_VIEW(webkit_web_view_new());

	gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(webview));
	webkit_web_view_load_uri(webview, argv[3]);
	gtk_widget_show_all(window);

	g_signal_connect(window, "damage-event", G_CALLBACK(capture), window);

	gtk_main();

	return 0;
}