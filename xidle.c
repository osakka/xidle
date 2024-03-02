/* 
  An Xidle (X Idle Time) Bash Loadable Module.

  $Author: Omar Sakka $
  $Branch: main $
  $Date: Sat Mar  2 10:42:14 PM GMT 2024 $
  $Revision: 3.24c45f3.20240302 $

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

// The standard structure describing a builtin command.
// Bash keeps an array of these structures. 
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
  char str;

  dpy = XOpenDisplay(NULL);
  if (dpy != NULL) {
    if (XScreenSaverQueryExtension(dpy, &event_basep, &error_basep)) {
      ssi = XScreenSaverAllocInfo();
      if (XScreenSaverQueryInfo(dpy, DefaultRootWindow(dpy), ssi)) {
        vendrel = VendorRelease(dpy);
        idle = ssi->idle;
        if (&idle >= 0) {
          printf("%lu\n", idle);
          return (EXECUTION_SUCCESS);
        }
        fprintf(stderr, "unknown error\n");
      }
      else { fprintf(stderr, "couldn't query screen saver info\n"); }
      XFree(ssi);
    }
    else {fprintf(stderr, "screen saver extension not supported\n"); }
    XCloseDisplay(dpy);
  }
  else { fprintf(stderr, "couldn't open display\n"); }
  return (EXECUTION_FAILURE);
}
