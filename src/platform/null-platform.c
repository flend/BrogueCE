#ifdef BROGUE_NULL

#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <errno.h>
#include "platform.h"

extern playerCharacter rogue;

static FILE *logfile;

static void open_logfile();
static void close_logfile();
static void write_to_log(const char *msg);
static void null_plotChar(enum displayGlyph inputChar,
			  short xLoc, short yLoc,
			  short foreRed, short foreGreen, short foreBlue,
			  short backRed, short backGreen, short backBlue);
static void sendStatusUpdate();
static boolean null_pauseForMilliseconds(short milliseconds);
static void null_nextKeyOrMouseEvent(rogueEvent *returnEvent, boolean textInput, boolean colorsDance);
static void null_remap(const char *input_name, const char *output_name);
static boolean modifier_held(int modifier);
static void notify_event(short eventId, int data1, int data2, const char *str1, const char *str2);
char *strdup(const char *s);

static void gameLoop()
{
  open_logfile();
  write_to_log("Logfile started\n");

  rogueMain();

  close_logfile();
}

char *strdup(const char *s) {
    size_t size = strlen(s) + 1;
    char *p = malloc(size);
    if (p) {
        memcpy(p, s, size);
    }
    return p;
}

static void open_logfile() {

  char log_filename[100];

  char *dirc = strdup(rogue.nextGamePath);
  char *basec = strdup(rogue.nextGamePath);

  const char *filename = basename(basec);
  char filename_no_ext[100];
  const char *path = dirname(dirc);
  const char *last_path_entry;

  const char *last_slash = strrchr(path, '/');

  if(last_slash == NULL)
    last_path_entry = path;
  else
    last_path_entry = last_slash + 1;

  const char *last_dot = strrchr(filename, '.');
  if(last_dot == NULL) {
    strncpy(filename_no_ext, filename, 99);
  }
  else {
    strncpy(filename_no_ext, filename, last_dot - filename);
    filename_no_ext[last_dot - filename] = '\0';
  }

  snprintf(log_filename, 100, "brogue-%s-%s.log", last_path_entry, filename_no_ext);

  logfile = fopen (log_filename, "w");
  if (logfile == NULL) {
    fprintf(stderr, "Logfile not created, errno = %d\n", errno);
  }

  free(dirc);
  free(basec);
}

static void close_logfile() {
  fclose(logfile);
}

static void write_to_log(const char *msg) {
  fprintf(logfile, msg);
  fflush(logfile);
}

static void null_plotChar(enum displayGlyph inputChar,
			  short xLoc, short yLoc,
			  short foreRed, short foreGreen, short foreBlue,
			  short backRed, short backGreen, short backBlue) {

        //Don't draw
}

static void sendStatusUpdate() {
 
    char msg[100];
    snprintf(msg, 100, "seed: %i level: %i gold: %i\n", rogue.seed, rogue.depthLevel, rogue.gold);
    write_to_log(msg);

}

// This function is used both for checking input and pausing
static boolean null_pauseForMilliseconds(short milliseconds)
{
  
  return true;
}

static void null_nextKeyOrMouseEvent(rogueEvent *returnEvent, boolean textInput, boolean colorsDance)
{
    static int status_counter = 0;
    if(!status_counter) {
      sendStatusUpdate();
    }
    status_counter++;
    if(status_counter == 100) {
      status_counter = 0;
    }
    returnEvent->eventType = -1;  
}

static void null_remap(const char *input_name, const char *output_name) {
    // Not needed
}

static boolean modifier_held(int modifier) {
    // Not needed, I am passing in the modifiers directly with the event data
	return 0;
}

static void notify_event(short eventId, int data1, int data2, const char *str1, const char *str2) {

  char msg[100];

  snprintf(msg, 100, "event: %i d1: %i d2: %i s1: %s s2: %s\n", eventId, data1, data2, str1, str2);
  write_to_log(msg);

}

struct brogueConsole nullConsole = {
	gameLoop,
	null_pauseForMilliseconds,
	null_nextKeyOrMouseEvent,
	null_plotChar,
	null_remap,
	modifier_held,
	notify_event
};

#endif
