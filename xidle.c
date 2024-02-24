/* SPDX-License-Identifier: GPL-2.0-only
 *
 * This program prints the "idle time" of the user to stdout. The "idle time"
 * is the number of milliseconds since input was received on any input device.
 * If unsuccessful, the program prints a message to stderr and exits with a
 * non-zero exit code.
 *
 * Copyright (c) 2005, 2008 Magnus Henoch <henoch@dtek.chalmers.se>
 * Copyright (c) 2006, 2007 by Danny Kukawka <dkukawka@suse.de>,
 *                                           <danny.kukawka@web.de>
 * Copyright (c) 2008 Eivind Magnus Hvidevold <hvidevold@gmail.com>
 * Copyright (c) 2014-2022 Richard Leitner <dev@g0hl1n.net>
 *
 * This file is part of xprintidle.
 *
 * xprintidle is free software; you can redistribute it and/or modify it under
 * the terms of version 2 of the GNU General Public License as published by the
 * Free Software Foundation.
 *
 * xprintidle is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * xprintidle. If not, see <https://www.gnu.org/licenses/>.
 *
 * The function workaroundCreepyXServer was adapted from kpowersave-0.7.3 by
 * Eivind Magnus Hvidevold <hvidevold@gmail.com>. kpowersave is licensed under
 * the GNU GPL, version 2 _only_.
 */

#include <X11/Xlib.h>
#include <X11/extensions/dpms.h>
#include <X11/extensions/scrnsaver.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <builtins.h>
#include <shell.h>
#include <bashgetopt.h>

char *xidle_doc[] = {
  "Query the X server for the user's idle time\n",
  "This code is built using xprintidle.\n",
  (char *)NULL
};

// Predeclare function.
int xidle_builtin ();

// The standard structure describing a builtin command.  bash keeps an array
// of these structures. 
struct builtin xidle_struct = {
  "xidle",             /* builtin name */
  xidle_builtin,       /* function implementing the builtin */
  BUILTIN_ENABLED,     /* initial flags for builtin */
  xidle_doc,           /* array of long documentation strings. */
  "xidle",             /* usage synopsis */
  0                    /* reserved for internal use */
};

int xidle_builtin (list) 
  WORD_LIST *list;
{
  XScreenSaverInfo *ssi;
  Display *dpy;
  int event_basep, error_basep, vendrel;
  uint64_t idle;

  dpy = XOpenDisplay(NULL);
  if (dpy != NULL) {
    if (!XScreenSaverQueryExtension(dpy, &event_basep, &error_basep)) {
      fprintf(stderr, "screen saver extension not supported\n");
      XCloseDisplay(dpy);
      return -1;
    }
    ssi = XScreenSaverAllocInfo();
    if (ssi != NULL) {
      if (!XScreenSaverQueryInfo(dpy, DefaultRootWindow(dpy), ssi)) {
        fprintf(stderr, "couldn't query screen saver info\n");
        XFree(ssi);
        XCloseDisplay(dpy);
        return -1;
      }
      vendrel = VendorRelease(dpy);
      // *idle = ssi->idle;
      idle = ssi->idle;
      XFree(ssi);
      XCloseDisplay(dpy);
      if (&idle < 0) { return (EX_USAGE); }
      else {
        printf("%lu\n", idle);
        return (EXECUTION_SUCCESS);
      }
    }
    fprintf(stderr, "couldn't allocate screen saver info\n");
    XCloseDisplay(dpy);
    return -1;
  }
  fprintf(stderr, "couldn't open display\n");
  return -1;
}
