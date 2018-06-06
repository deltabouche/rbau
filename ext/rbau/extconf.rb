require 'mkmf-rice'
require 'pry'
extension_name = 'rbau'
dir_config(extension_name)
have_framework('CoreFoundation')
have_framework('CoreServices')
have_framework('AudioToolbox')
have_framework('AudioUnit')
have_framework('CoreMIDI')

$objs = Dir["#{$srcdir}/**/*.cpp"].map { |fn| File.join(File.dirname(fn), File.basename(fn, '.cpp') + '.o')  }
$INCFLAGS << " -I$(srcdir)/core_audio_public_utility"
$VPATH << "$(srcdir)/core_audio_public_utility"

create_makefile(extension_name)

# to clean up object files under util subdirectory.
open('Makefile', 'a') do |f|
  f.write <<EOS

CLEANOBJS := $(CLEANOBJS) core_audio_public_utility/*.#{CONFIG["OBJEXT"]}
EOS
end
