#ifndef CALCULUSOS_KEYBOARD_H
#define CALCULUSOS_KEYBOARD_H

void keyboard_init(void);

/* Blocking read of one character typed by the user.
 * Returns 0x08 for backspace and '\n' for Enter. */
char keyboard_getchar(void);

#endif
