#ifndef AUDIO_H
#define AUDIO_H
#endif
#include <Arduino.h>
#include <SD.h>
#include "boards.hpp"
#include "networks.hpp"
void playAudio(String name);
void sound_for_train(String type, String no, int8_t direction);
void beep();
void beep_warn();
void boot_sound();
extern bool beep_enabled;
extern bool sound_enabled;
extern uint8_t volume;