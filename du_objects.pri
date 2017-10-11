win32 {
    INCLUDEPATH += $$PWD/libs/win/libsndfile/include
    DEPENDPATH += $$PWD/libs/win/libsndfile/include

    contains(QMAKE_TARGET.arch, x86_64) {
        LIBS += -L$$PWD/libs/win/libsndfile/lib/x64 -L$$PWD/libs/win/libsndfile/bin/x64 -llibsndfile-1
    } else {
        LIBS += -L$$PWD/libs/win/libsndfile/lib/x86 -L$$PWD/libs/win/libsndfile/bin/x86 -llibsndfile-1
    }
} else:macx {
    INCLUDEPATH += $$PWD/libs/mac/libsndfile/include
    DEPENDPATH += $$PWD/libs/mac/libsndfile/include

    LIBS += -L$$PWD/libs/mac/libsndfile/lib -lsndfile

    libsndfile.path = Contents/Frameworks
    libsndfile.files = $$PWD/libs/mac/libsndfile/lib/libsndfile.1.dylib

    QMAKE_BUNDLE_DATA += libsndfile
} else:unix {
    LIBS += -lsndfile
}

HEADERS += \
    $$PWD/du-touch/dualo_structs/music_structs.h \
    $$PWD/du-touch/dualo_structs/sound_structs.h \
    $$PWD/du-touch/dualo_structs/g_structs.h \
    $$PWD/du-touch/dualo_structs/instr_mapping.h \
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
    $$PWD/dumusicfile/dusonginfo.h \
    $$PWD/dumusicfile/dutrack.h \
    $$PWD/dumusicfile/duloop.h \
    $$PWD/dumusicfile/duevent.h \
    $$PWD/dumusicfile/dumusicinstrument.h \
    $$PWD/instrument/duinstrumentinfo.h \
    $$PWD/instrument/dupreset.h \
    $$PWD/instrument/effects/duchorus.h \
    $$PWD/instrument/effects/ducompressor.h \
    $$PWD/instrument/effects/dudelay.h \
    $$PWD/instrument/effects/dudistortion.h \
    $$PWD/instrument/effects/dueffectsettings.h \
    $$PWD/instrument/effects/duequalizer.h \
    $$PWD/instrument/effects/dumixer.h \
    $$PWD/instrument/effects/dureverb.h \
    $$PWD/dudevice.h \
    $$PWD/dutouch.h \
    $$PWD/dulist.h \
    $$PWD/dudevicelist.h \
    $$PWD/duprofile.h \
    $$PWD/dusoundfile/dusound.h \
    $$PWD/du-touch/dualo_structs/controler_structs.h \
    $$PWD/dusoundfile/dunote.h \
    $$PWD/dusoundfile/dusoundinfo.h \
    $$PWD/dusoundfile/dulayer.h \
    $$PWD/dusoundfile/dusample.h \
    $$PWD/general/duboolean.h \
    $$PWD/du-touch/dualo_structs/old_sound_structs.h \
    $$PWD/du-touch/dualo_structs/music_migration.h \
    $$PWD/jsonApi/JsonApiDocument.h \
    $$PWD/jsonApi/JsonApiErrorObject.h \
    $$PWD/jsonApi/JsonApiResourceObject.h \
    $$PWD/instrument/IntrumentIdentifier.h \
    $$PWD/midifile/dumidivariablelength.h \
    $$PWD/midifile/dumidistatus.h \
    $$PWD/midifile/dumidibasicevent.h \
    $$PWD/midifile/dumidichannelevent.h \
    $$PWD/midifile/dumidimetaevent.h \
    $$PWD/midifile/dumidisysexevent.h \
    $$PWD/midifile/dumiditrack.h \
    $$PWD/midifile/dumidifile.h \
    $$PWD/du-touch/dualo_structs/parameters_structs.h \
    $$PWD/du-touch/dualo_structs/arrangement_structs.h \
    $$PWD/du-touch/dualo_structs/metadata_structs.h \
    $$PWD/DuGameFiles/DuGame.h \
    $$PWD/metadata/DuMetadataChunk.h \
    $$PWD/dumusicfile/DuMusicMetadata.h \
    $$PWD/DuGameFiles/DuGameEvent.h \
    $$PWD/DuGameFiles/DuGameEventMessage.h \
    $$PWD/DuGameFiles/DuArrangementAction.h \
    $$PWD/DuGameFiles/DuGameExitCondition.h \
    $$PWD/general/DuEnum.h \
    $$PWD/du-touch/dualo_structs/dugame_structs.h

SOURCES += \
    $$PWD/general/duobject.cpp \
    $$PWD/general/ducontainer.cpp \
    $$PWD/general/duvalue.cpp \
    $$PWD/general/dubinarydata.cpp \
    $$PWD/general/dunumeric.cpp \
    $$PWD/general/dustring.cpp \
    $$PWD/general/duversion.cpp \
    $$PWD/general/dudate.cpp \
    $$PWD/general/duurl.cpp \
    $$PWD/dumusicfile/dumusic.cpp \
    $$PWD/dumusicfile/dusonginfo.cpp \
    $$PWD/dumusicfile/dutrack.cpp \
    $$PWD/dumusicfile/duloop.cpp \
    $$PWD/dumusicfile/duevent.cpp \
    $$PWD/dumusicfile/dumusicinstrument.cpp \
    $$PWD/instrument/duinstrumentinfo.cpp \
    $$PWD/instrument/dupreset.cpp \
    $$PWD/instrument/effects/duchorus.cpp \
    $$PWD/instrument/effects/ducompressor.cpp \
    $$PWD/instrument/effects/dudelay.cpp \
    $$PWD/instrument/effects/dudistortion.cpp \
    $$PWD/instrument/effects/dueffectsettings.cpp \
    $$PWD/instrument/effects/duequalizer.cpp \
    $$PWD/instrument/effects/dumixer.cpp \
    $$PWD/instrument/effects/dureverb.cpp \
    $$PWD/dudevice.cpp \
    $$PWD/dutouch.cpp \
    $$PWD/dulist.cpp \
    $$PWD/dudevicelist.cpp \
    $$PWD/duprofile.cpp \
    $$PWD/dusoundfile/dusound.cpp \
    $$PWD/dusoundfile/dunote.cpp \
    $$PWD/dusoundfile/dusoundinfo.cpp \
    $$PWD/dusoundfile/dulayer.cpp \
    $$PWD/dusoundfile/dusample.cpp \
    $$PWD/general/duboolean.cpp \
    $$PWD/du-touch/dualo_structs/music_migration.c \
    $$PWD/jsonApi/JsonApiDocument.cpp \
    $$PWD/jsonApi/JsonApiErrorObject.cpp \
    $$PWD/jsonApi/JsonApiResourceObject.cpp \
    $$PWD/general/duarray.cpp \
    $$PWD/midifile/dumidivariablelength.cpp \
    $$PWD/midifile/dumidistatus.cpp \
    $$PWD/midifile/dumidibasicevent.cpp \
    $$PWD/midifile/dumidichannelevent.cpp \
    $$PWD/midifile/dumidimetaevent.cpp \
    $$PWD/midifile/dumidisysexevent.cpp \
    $$PWD/midifile/dumiditrack.cpp \
    $$PWD/midifile/dumidifile.cpp \
    $$PWD/DuGameFiles/DuGame.cpp \
    $$PWD/metadata/DuMetadataChunk.cpp \
    $$PWD/dumusicfile/DuMusicMetadata.cpp \
    $$PWD/DuGameFiles/DuGameEvent.cpp \
    $$PWD/DuGameFiles/DuGameEventMessage.cpp \
    $$PWD/DuGameFiles/DuArrangementAction.cpp \
    $$PWD/DuGameFiles/DuGameExitCondition.cpp \
    $$PWD/general/DuEnum.cpp

DEFINES += NO_MIDI_IMPORT

HEADERS += \
#    $$PWD/miditodumusic/dumidikeymapper.h \
#    $$PWD/miditodumusic/midiconversionhelper.h \
#    $$PWD/miditodumusic/dutimesignaturemodel.h \
#    $$PWD/miditodumusic/dutonalitymodel.h \
#    $$PWD/miditodumusic/duscalemodel.h

SOURCES += \
#    $$PWD/miditodumusic/dumidikeymapper.cpp \
#    $$PWD/miditodumusic/midiconversionhelper.cpp \
#    $$PWD/miditodumusic/dutimesignaturemodel.cpp \
#    $$PWD/miditodumusic/dutonalitymodel.cpp \
#    $$PWD/miditodumusic/duscalemodel.cpp
    
#RESOURCES += \
#    $$PWD/miditodumusic/midiresources.qrc
