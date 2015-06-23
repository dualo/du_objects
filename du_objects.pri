HEADERS += \
    $$PWD/du-touch/parameters/music_parameters_mng.h \
    $$PWD/du-touch/parameters/sound_parameters_mng.h \
    $$PWD/du-touch/parameters/g_parameters_mng.h \
    $$PWD/general/duobject.h \
    $$PWD/general/ducontainer.h \
    $$PWD/general/duarray.h \
    $$PWD/general/duvalue.h \
    $$PWD/general/dubinarydata.h \
    $$PWD/general/dunumeric.h \
    $$PWD/general/dustring.h \
    $$PWD/general/duversion.h \
    $$PWD/general/dudate.h \
    $$PWD/general/duurl.h \
    $$PWD/dumusicfile/dumusic.h \
    $$PWD/dumusicfile/duheader.h \
    $$PWD/dumusicfile/duparameters.h \
    $$PWD/dumusicfile/dusonginfo.h \
    $$PWD/dumusicfile/dutrack.h \
    $$PWD/dumusicfile/duloop.h \
    $$PWD/dumusicfile/duevent.h \
    $$PWD/dumusicfile/instrument/duinstrument.h \
    $$PWD/dumusicfile/instrument/duinstrumentinfo.h \
    $$PWD/dumusicfile/instrument/dupreset.h \
    $$PWD/dumusicfile/instrument/effects/duadsr.h \
    $$PWD/dumusicfile/instrument/effects/duchorus.h \
    $$PWD/dumusicfile/instrument/effects/ducompressor.h \
    $$PWD/dumusicfile/instrument/effects/dudelay.h \
    $$PWD/dumusicfile/instrument/effects/dudistortion.h \
    $$PWD/dumusicfile/instrument/effects/dueffectsettings.h \
    $$PWD/dumusicfile/instrument/effects/duequalizer.h \
    $$PWD/dumusicfile/instrument/effects/dumixer.h \
    $$PWD/dumusicfile/instrument/effects/duvibrato.h \
    $$PWD/dumusicfile/instrument/effects/duwah.h \
    $$PWD/dumusicfile/instrument/effects/dureverb.h \
    $$PWD/midifile/dumidivariablelength.h \
    $$PWD/midifile/dumidistatus.h \
    $$PWD/midifile/dumidibasicevent.h \
    $$PWD/midifile/dumidichannelevent.h \
    $$PWD/midifile/dumidimetaevent.h \
    $$PWD/midifile/dumidisysexevent.h \
    $$PWD/midifile/dumiditrack.h \
    $$PWD/midifile/dumidifile.h \
    $$PWD/miditodumusic/dumidikeymapper.h \
    $$PWD/dudevice.h \
    $$PWD/dutouch.h \
    $$PWD/dulist.h \
    $$PWD/dudevicelist.h \
    $$PWD/duprofile.h \
    $$PWD/dusoundfile/dusound.h

SOURCES += \
    $$PWD/general/duobject.cpp \
    $$PWD/general/ducontainer.cpp \
    $$PWD/general/duarray.cpp \
    $$PWD/general/duvalue.cpp \
    $$PWD/general/dubinarydata.cpp \
    $$PWD/general/dunumeric.cpp \
    $$PWD/general/dustring.cpp \
    $$PWD/general/duversion.cpp \
    $$PWD/general/dudate.cpp \
    $$PWD/general/duurl.cpp \
    $$PWD/dumusicfile/dumusic.cpp \
    $$PWD/dumusicfile/duheader.cpp \
    $$PWD/dumusicfile/duparameters.cpp \
    $$PWD/dumusicfile/dusonginfo.cpp \
    $$PWD/dumusicfile/dutrack.cpp \
    $$PWD/dumusicfile/duloop.cpp \
    $$PWD/dumusicfile/duevent.cpp \
    $$PWD/dumusicfile/instrument/duinstrument.cpp \
    $$PWD/dumusicfile/instrument/duinstrumentinfo.cpp \
    $$PWD/dumusicfile/instrument/dupreset.cpp \
    $$PWD/dumusicfile/instrument/effects/duadsr.cpp \
    $$PWD/dumusicfile/instrument/effects/duchorus.cpp \
    $$PWD/dumusicfile/instrument/effects/ducompressor.cpp \
    $$PWD/dumusicfile/instrument/effects/dudelay.cpp \
    $$PWD/dumusicfile/instrument/effects/dudistortion.cpp \
    $$PWD/dumusicfile/instrument/effects/dueffectsettings.cpp \
    $$PWD/dumusicfile/instrument/effects/duequalizer.cpp \
    $$PWD/dumusicfile/instrument/effects/dumixer.cpp \
    $$PWD/dumusicfile/instrument/effects/duvibrato.cpp \
    $$PWD/dumusicfile/instrument/effects/duwah.cpp \
    $$PWD/dumusicfile/instrument/effects/dureverb.cpp \
    $$PWD/midifile/dumidivariablelength.cpp \
    $$PWD/midifile/dumidistatus.cpp \
    $$PWD/midifile/dumidibasicevent.cpp \
    $$PWD/midifile/dumidichannelevent.cpp \
    $$PWD/midifile/dumidimetaevent.cpp \
    $$PWD/midifile/dumidisysexevent.cpp \
    $$PWD/midifile/dumiditrack.cpp \
    $$PWD/midifile/dumidifile.cpp \
    $$PWD/miditodumusic/dumidikeymapper.cpp \
    $$PWD/dudevice.cpp \
    $$PWD/dutouch.cpp \
    $$PWD/dulist.cpp \
    $$PWD/dudevicelist.cpp \
    $$PWD/duprofile.cpp \
    $$PWD/dusoundfile/dusound.cpp
    
DISTFILES += \
    $$PWD/miditodumusic/defaultmaps.json
