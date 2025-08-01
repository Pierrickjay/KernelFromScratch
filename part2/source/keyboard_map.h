#ifndef KEYBOARD_MAP_H
#define KEYBOARD_MAP_H

#include "types.h"
#define LEFT_ARROW 0x4B
#define RIGHT_ARROW 0x4D
#define UP_ARROW 0x48
#define DOWN_ARROW 0x50

#define BACKSPACE 0x0E

// États des touches spéciales
#define SHIFT_PRESSED 0x1
#define CAPS_LOCK 0x2

// Scancodes pour les touches spéciales
#define SCANCODE_LSHIFT 0x2A
#define SCANCODE_RSHIFT 0x36
#define SCANCODE_CAPS_LOCK 0x3A

// Table de conversion scancode -> caractère (sans shift)
extern const char scancode_to_ascii[];

// Table de conversion scancode -> caractère (avec shift)
extern const char scancode_to_ascii_shift[];

// Fonction pour convertir un scancode en caractère
char scancode_to_char(u8 scancode, u8 special_keys);

#endif
