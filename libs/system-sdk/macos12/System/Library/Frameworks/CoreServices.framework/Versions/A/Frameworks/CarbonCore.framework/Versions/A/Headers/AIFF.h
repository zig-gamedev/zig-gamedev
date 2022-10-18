/*
     File:       CarbonCore/AIFF.h
 
     Contains:   Definition of AIFF file format components.
                 The contents of this header file are deprecated.
 
     Copyright:  © 1989-2011 by Apple Inc. All rights reserved.
*/
#ifndef __AIFF__
#define __AIFF__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#include <stdint.h> /* uint32_t */

#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#pragma pack(push, 2)

enum {
  AIFFID                        = 'AIFF',
  AIFCID                        = 'AIFC',
  FormatVersionID               = 'FVER',
  CommonID                      = 'COMM',
  FORMID                        = 'FORM',
  SoundDataID                   = 'SSND',
  MarkerID                      = 'MARK',
  InstrumentID                  = 'INST',
  MIDIDataID                    = 'MIDI',
  AudioRecordingID              = 'AESD',
  ApplicationSpecificID         = 'APPL',
  CommentID                     = 'COMT',
  NameID                        = 'NAME',
  AuthorID                      = 'AUTH',
  CopyrightID                   = '(c) ',
  AnnotationID                  = 'ANNO'
};

enum {
  NoLooping                     = 0,
  ForwardLooping                = 1,
  ForwardBackwardLooping        = 2
};


enum {
    /* AIFF-C Versions */
  AIFCVersion1                  = (uint32_t)0xA2805140
};

/* Compression Names */
#define NoneName                        "\pnot compressed"
#define ACE2to1Name                     "\pACE 2-to-1"
#define ACE8to3Name                     "\pACE 8-to-3"
#define MACE3to1Name                    "\pMACE 3-to-1"
#define MACE6to1Name                    "\pMACE 6-to-1"
enum {
                                        /* Compression Types */
  NoneType                      = 'NONE',
  ACE2Type                      = 'ACE2',
  ACE8Type                      = 'ACE8',
  MACE3Type                     = 'MAC3',
  MACE6Type                     = 'MAC6'
};

/*
    AIFF.h use to define a type, ID, which causes conflicts with other headers and application which want to use
    this pretty common name as their own type.  If you were previously relying on this being defined here, you 
    should either define it yourself or change your references to it into a UInt32.
    
    typedef UInt32 ID;
*/
typedef SInt16                          MarkerIdType;
struct ChunkHeader {
  UInt32              ckID;
  SInt32              ckSize;
};
typedef struct ChunkHeader              ChunkHeader;
struct ContainerChunk {
  UInt32              ckID;
  SInt32              ckSize;
  UInt32              formType;
};
typedef struct ContainerChunk           ContainerChunk;
struct FormatVersionChunk {
  UInt32              ckID;
  SInt32              ckSize;
  UInt32              timestamp;
};
typedef struct FormatVersionChunk       FormatVersionChunk;
typedef FormatVersionChunk *            FormatVersionChunkPtr;
struct CommonChunk {
  UInt32              ckID;
  SInt32              ckSize;
  SInt16              numChannels;
  UInt32              numSampleFrames;
  SInt16              sampleSize;
  extended80          sampleRate;
};
typedef struct CommonChunk              CommonChunk;
typedef CommonChunk *                   CommonChunkPtr;
struct ExtCommonChunk {
  UInt32              ckID;
  SInt32              ckSize;
  SInt16              numChannels;
  UInt32              numSampleFrames;
  SInt16              sampleSize;
  extended80          sampleRate;
  UInt32              compressionType;
  char                compressionName[1];     /* variable length array, Pascal string */
};
typedef struct ExtCommonChunk           ExtCommonChunk;
typedef ExtCommonChunk *                ExtCommonChunkPtr;
struct SoundDataChunk {
  UInt32              ckID;
  SInt32              ckSize;
  UInt32              offset;
  UInt32              blockSize;
};
typedef struct SoundDataChunk           SoundDataChunk;
typedef SoundDataChunk *                SoundDataChunkPtr;
struct Marker {
  MarkerIdType        id;
  UInt32              position;
  Str255              markerName;
};
typedef struct Marker                   Marker;
struct MarkerChunk {
  UInt32              ckID;
  SInt32              ckSize;
  UInt16              numMarkers;
  Marker              Markers[1];             /* variable length array */
};
typedef struct MarkerChunk              MarkerChunk;
typedef MarkerChunk *                   MarkerChunkPtr;
struct AIFFLoop {
  SInt16              playMode;
  MarkerIdType        beginLoop;
  MarkerIdType        endLoop;
};
typedef struct AIFFLoop                 AIFFLoop;
struct InstrumentChunk {
  UInt32              ckID;
  SInt32              ckSize;
  UInt8               baseFrequency;
  UInt8               detune;
  UInt8               lowFrequency;
  UInt8               highFrequency;
  UInt8               lowVelocity;
  UInt8               highVelocity;
  SInt16              gain;
  AIFFLoop            sustainLoop;
  AIFFLoop            releaseLoop;
};
typedef struct InstrumentChunk          InstrumentChunk;
typedef InstrumentChunk *               InstrumentChunkPtr;
struct MIDIDataChunk {
  UInt32              ckID;
  SInt32              ckSize;
  UInt8               MIDIdata[1];            /* variable length array */
};
typedef struct MIDIDataChunk            MIDIDataChunk;
typedef MIDIDataChunk *                 MIDIDataChunkPtr;
struct AudioRecordingChunk {
  UInt32              ckID;
  SInt32              ckSize;
  UInt8               AESChannelStatus[24];
};
typedef struct AudioRecordingChunk      AudioRecordingChunk;
typedef AudioRecordingChunk *           AudioRecordingChunkPtr;
struct ApplicationSpecificChunk {
  UInt32              ckID;
  SInt32              ckSize;
  OSType              applicationSignature;
  UInt8               data[1];                /* variable length array */
};
typedef struct ApplicationSpecificChunk ApplicationSpecificChunk;
typedef ApplicationSpecificChunk *      ApplicationSpecificChunkPtr;
struct Comment {
  UInt32              timeStamp;
  MarkerIdType        marker;
  UInt16              count;
  char                text[1];                /* variable length array, Pascal string */
};
typedef struct Comment                  Comment;
struct CommentsChunk {
  UInt32              ckID;
  SInt32              ckSize;
  UInt16              numComments;
  Comment             comments[1];            /* variable length array */
};
typedef struct CommentsChunk            CommentsChunk;
typedef CommentsChunk *                 CommentsChunkPtr;
struct TextChunk {
  UInt32              ckID;
  SInt32              ckSize;
  char                text[1];                /* variable length array, Pascal string */
};
typedef struct TextChunk                TextChunk;
typedef TextChunk *                     TextChunkPtr;

#pragma pack(pop)


#endif /* __AIFF__ */

