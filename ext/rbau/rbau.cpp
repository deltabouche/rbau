#include <AudioToolbox/AudioToolbox.h>
#include <CoreAudio/CoreAudio.h>
#include "AHHost.h"
#include "AHAudioUnit.h"
#include "AHUtils.h"
#include "FileSystemUtils.h"

#include <iostream>
using namespace std;

#include "rice/Class.hpp"
#include "rice/String.hpp"

using namespace Rice;

static AHTrack *track;
static AHHost *host;

// Object rbau_initialize(Object self)
// {
//   return self;
// }

Object rbau_load_midi_file(Object self, Object fn)
{
  host->LoadMidiFile(from_ruby<string>(fn).c_str());
  return to_ruby(true);
}

Object rbau_load_au_preset(Object self, Object fn)
{
  track->GetSynth()->LoadAUPresetFromFile(from_ruby<string>(fn).c_str());
  return to_ruby(true);
}

Object rbau_bounce_to_file(Object self, Object fn)
{
  host->BounceToFile(from_ruby<string>(fn).c_str());
  return to_ruby(true);
}

Object rbau_play(Object self)
{
  host->Play();
  return to_ruby(true);
}

Object rbau_stop(Object self)
{
  host->Stop();
  return to_ruby(true);
}

Object rbau_add_track(Object self, Object unitName)
{
  track = host->AddTrack(from_ruby<string>(unitName).c_str());
  return to_ruby(true);
}

extern "C" void Init_rbau()
{
  host = new AHHost();
  Class rb_cRbAU =
      define_class("RbAU")
          .define_method("add_track", &rbau_add_track)
          .define_method("load_midi_file", &rbau_load_midi_file)
          .define_method("load_au_preset", &rbau_load_au_preset)
          .define_method("bounce_to_file", &rbau_bounce_to_file)
          .define_method("play", &rbau_play)
          .define_method("stop", &rbau_stop);

  // .define_method("initialize", &rbau_initialize)
}
