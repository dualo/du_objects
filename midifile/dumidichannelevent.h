#ifndef DUMIDICHANNELEVENT_H
#define DUMIDICHANNELEVENT_H

#include "dumidibasicevent.h"


#define KEY_MIDICHANNELEVENT_KEY        "Key"
#define KEY_MIDICHANNELEVENT_VALUE      "Value"


DU_OBJECT(DuMidiChannelEvent)

class DuMidiChannelEvent : public DuMidiBasicEvent
{
public:
    explicit DuMidiChannelEvent(quint32 time = 0, quint8 status = 0x80);
    virtual ~DuMidiChannelEvent();

    enum Type
    {
        NoteOff = 0x8,
        NoteOn = 0x9,
        KeyAftertouch = 0xA,
        ControlChange = 0xB,
        ProgramChange = 0xC,
        ChannelPressure = 0xD,
        PitchWheelChange = 0xE
    };

    enum Notes
    {
        // Keys are indexed in the following order from 0x00 to 0x7F.

        CMinus1, CsharpMinus1, DMinus1, DsharpMinus1, EMinus1, FMinus1,
        FsharpMinus1, GMinus1, GsharpMinus1, AMinus1, AsharpMinus1, BMinus1,
        C0, Csharp0, D0, Dsharp0, E0, F0, Fsharp0, G0, Gsharp0, A0, Asharp0, B0,
        C1, Csharp1, D1, Dsharp1, E1, F1, Fsharp1, G1, Gsharp1, A1, Asharp1, B1,
        C2, Csharp2, D2, Dsharp2, E2, F2, Fsharp2, G2, Gsharp2, A2, Asharp2, B2,
        C3, Csharp3, D3, Dsharp3, E3, F3, Fsharp3, G3, Gsharp3, A3, Asharp3, B3,
        C4, Csharp4, D4, Dsharp4, E4, F4, Fsharp4, G4, Gsharp4, A4, Asharp4, B4,
        C5, Csharp5, D5, Dsharp5, E5, F5, Fsharp5, G5, Gsharp5, A5, Asharp5, B5,
        C6, Csharp6, D6, Dsharp6, E6, F6, Fsharp6, G6, Gsharp6, A6, Asharp6, B6,
        C7, Csharp7, D7, Dsharp7, E7, F7, Fsharp7, G7, Gsharp7, A7, Asharp7, B7,
        C8, Csharp8, D8, Dsharp8, E8, F8, Fsharp8, G8, Gsharp8, A8, Asharp8, B8,
        C9, Csharp9, D9, Dsharp9, E9, F9, Fsharp9, G9
    };

    enum Controls
    {
        // 14-bit controllers.
        // Arguments should be < 0x80.

        BankSelectMSB = 0x00,                BankSelectLSB = 0x20,
        ModulationWheelMSB = 0x01,           ModulationWheelLSB = 0x21,
        BreathControlMSB = 0x02,             BreathControlLSB = 0x22,
        FootControllerMSB = 0x04,            FootControllerLSB = 0x24,
        PortamentoTimeMSB = 0x05,            PortamentoTimeLSB = 0x25,
        DataEntryMSB = 0x06,                 DataEntryLSB = 0x26,
        ChannelVolumeMSB = 0x07,             ChannelVolumeLSB = 0x27,
        BalanceMSB = 0x08,                   BalanceLSB = 0x28,
        PanMSB = 0x0A,                       PanLSB = 0x2A,
        ExpressionControllerMSB = 0x0B,      ExpressionControllerLSB = 0x2B,
        EffectControl1MSB = 0x0C,            EffectControl1LSB = 0x2C,
        EffectControl2MSB = 0x0D,            EffectControl2LSB = 0x2D,

        GeneralPurposeController1MSB = 0x10, GeneralPurposeController1LSB = 0x30,
        GeneralPurposeController2MSB = 0x11, GeneralPurposeController2LSB = 0x31,
        GeneralPurposeController3MSB = 0x12, GeneralPurposeController3LSB = 0x32,
        GeneralPurposeController4MSB = 0x13, GeneralPurposeController4LSB = 0x33,

        NonRegisteredParameterNumberLSB = 0x62,
        NonRegisteredParameterNumberMSB = 0x63,

        RegisteredParameterNumberLSB = 0x64,
        RegisteredParameterNumberMSB = 0x65,


        // 7-bit controllers.
        // Arguments should be < 0x80.

        // An argument < 0x3F means Off, an argument > 0x40 means On.
        DamperPedalOnOff = 0x40,             SoftPedalOnOff = 0x43,
        PortamentoOnOff = 0x41,              LegatoFootswitch = 0x44,
        SustenutoOnOff = 0x42,               Hold2 = 0x45,

        SoundController1 = 0x46,            // Sound Variation
        SoundController2 = 0x47,            // Timbre
        SoundController3 = 0x48,            // Release Time
        SoundController4 = 0x49,            // Attack Time
        SoundController5 = 0x4A,            // Brightness
        SoundController6 = 0x4B,
        SoundController7 = 0x4C,
        SoundController8 = 0x4D,
        SoundController9 = 0x4E,
        SoundController10 = 0x4F,

        GeneralPurposeController5LSB = 0x50,
        GeneralPurposeController6LSB = 0x51,
        GeneralPurposeController7LSB = 0x52,
        GeneralPurposeController8LSB = 0x53,

        PortamentoControl = 0x54,           // Argument is the source note.

        Effects1DepthLSB = 0x5B,            // External effects
        Effects2DepthLSB = 0x5C,            // Tremolo
        Effects3DepthLSB = 0x5D,            // Chorus
        Effects4DepthLSB = 0x5E,            // Detune
        Effects5DepthLSB = 0x5F,            // Phase

        // Argument N/A.

        DataIncrement = 0x60,               DataDecrement = 0x61,


        // Channel modes.

        AllSoundOff = 0x78,                 // Argument should be 0x00.
        ResetAllControllers = 0x79,         // Argument should be 0x00.
        LocalControlOnOff = 0x7A,           // 0x00 is Off, 0x7F is On.
        AllNotesOff = 0x7B,                 // Argument should be 0x00.

        // These messages include AllNotesOff.
        // MonoModeOn include PolyModeOff.
        // PolyModeOn include MonoModeOff.

        OmniModeOff = 0x7C,                 // Argument should be 0x00.
        OmniModeOn = 0x7D,                  // Argument should be 0x00.
        MonoModeOn = 0x7E,                  // Argument equals the number of channels,
        // or zero if the number of channels equals the number of voices in the receiver.
        PolyModeOn = 0x7F                   // Argument should be 0x00.
    };

    enum Patches
    {
        // Pianos
        AcousticGrandPiano = 0x00,      BrightAcousticPiano = 0x01,
        ElectricGrandPiano = 0x02,      HonkyTonkPiano = 0x03,
        ElectricPiano1 = 0x04,          // Rhodes Piano
        ElectricPiano2 = 0x05,          // Chorused Piano
        Harpsichord = 0x06,             Clavinet = 0x07,

        // Chromatic Percussions
        Celesta = 0x08,                 Glockenspiel = 0x09,
        MusicBox = 0x0A,                Vibraphone = 0x0B,
        Marimba = 0x0C,                 Xylophone = 0x0D,
        TubularBells = 0x0E,            Dulcimer = 0x0F,    // Santur

        // Organs
        DrawbarOrgan = 0x10,            // Hammond
        PercussiveOrgan = 0x11,
        RockOrgan = 0x12,               ChurchOrgan = 0x13,
        ReedOrgan = 0x14,               Accordion = 0x15,       // French
        Harmonica = 0x16,               TangoAccordion = 0x17,  // Band Neon

        // Guitars
        AcousticNylonGuitar = 0x18,     AcousticSteelGuitar = 0x19,
        ElectricJazzGuitar = 0x1A,      ElectricCleanGuitar = 0x1B,
        ElectricMutedGuitar = 0x1C,     OverdrivenGuitar = 0x1D,
        DistortionGuitar = 0x1E,        GuitarHarmonics = 0x1F,

        // Basses
        AcousticBass = 0x20,            ElectricFingeredBass = 0x21,
        ElectricPickedBass = 0x22,      FretlessBass = 0x23,
        SlapBass1 = 0x24,               SlapBass2 = 0x25,
        SynthBass1 = 0x26,              SynthBass2 = 0x27,

        // Strings
        Violin = 0x28,                  Viola = 0x29,
        Cello = 0x2A,                   Contrabass = 0x2B,
        TremoloStrings = 0x2C,          PizzicatoStrings = 0x2D,
        OrchestralHarp = 0x2E,          Timpani = 0x2F,

        // Ensembles
        StringEnsemble1 = 0x30,         StringEnsemble2 = 0x31,
        SynthStrings1 = 0x32,           SynthStrings2 = 0x33,
        ChoirAahs = 0x34,               VoiceOohs = 0x35,
        SynthVoice = 0x36,              OrchestraHit = 0x37,

        // Brass
        Trumpet = 0x38,                 Trombone = 0x39,
        Tuba = 0x3A,                    MutedTrumpet = 0x3B,
        FrenchHorn = 0x3C,              BrassSection = 0x3D,
        SynthBrass1 = 0x3E,             SynthBrass2 = 0x3F,

        // Reeds
        SopranoSax = 0x40,              AltoSax = 0x41,
        TenorSax = 0x42,                BaritoneSax = 0x43,
        Oboe = 0x44,                    EnglishHorn = 0x45,
        Bassoon = 0x46,                 Clarinet = 0x47,

        // Pipes
        Piccolo = 0x48,                 Flute = 0x49,
        Recorder = 0x4A,                PanFlute = 0x4B,
        BlownBottle = 0x4C,             Shakuhachi = 0x4D,
        Whistle = 0x4E,                 Ocarina = 0x4F,

        // Synth Leads
        Lead1 = 0x50,       // Square Wave
        Lead2 = 0x51,       // Sawtooth Wave
        Lead3 = 0x52,       // Calliope
        Lead4 = 0x53,       // Chiffer
        Lead5 = 0x54,       // Charang
        Lead6 = 0x55,       // Voice Solo
        Lead7 = 0x56,       // Fifths
        Lead8 = 0x57,       // Bass + Lead

        // Synth Pads
        Pad1 = 0x58,        // New Age Fantasia
        Pad2 = 0x59,        // Warm
        Pad3 = 0x5A,        // Polysynth
        Pad4 = 0x5B,        // Choir Space Voice
        Pad5 = 0x5C,        // Bowed Glass
        Pad6 = 0x5D,        // Metallic Pro
        Pad7 = 0x5E,        // Halo
        Pad8 = 0x5F,        // Sweep

        // Synth Effects
        FX1 = 0x60,         // Rain
        FX2 = 0x61,         // Soundtrack
        FX3 = 0x62,         // Crystal
        FX4 = 0x63,         // Atmosphere
        FX5 = 0x64,         // Brigthness
        FX6 = 0x65,         // Goblins
        FX7 = 0x66,         // Echoes, Drops
        FX8 = 0x67,         // Sci-fi, Star Theme

        // Ethnic
        Sitar = 0x68,                   Banjo = 0x69,
        Shamisen = 0x6A,                Koto = 0x6B,
        Kalimba = 0x6C,                 BagPipe = 0x6D,
        Fiddle = 0x6E,                  Shanai = 0x6E,

        // Percussive
        TinkleBell = 0x70,              Agogo = 0x71,
        SteelDrums = 0x72,              Woodblock = 0x73,
        TaikoDrum = 0x74,               MelodicTom = 0x75,
        SynthDrum = 0x76,               ReverseCymbal = 0x77,

        // Sound Effects
        GuitarFretNoise = 0x78,         BreathNoise = 0x79,
        Seashore = 0x7A,                BirdTweet = 0x7B,
        TelephoneRing = 0x7C,           Helicopter = 0x7D,
        Applause = 0x7E,                Gunshot = 0x7F
    };

    virtual DuObjectPtr clone() const;

    virtual QByteArray toMidiBinary() const;

    virtual int size() const;

    quint8 getType() const;
    void setType(quint8 value);

    quint8 getChannel() const;
    void setChannel(quint8 value);

    quint8 getKey() const;
    void setKey(quint8 value);

    quint8 getValue() const;
    void setValue(quint8 value);
};

#endif // DUMIDICHANNELEVENT_H
