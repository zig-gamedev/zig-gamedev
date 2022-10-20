/*
     File:       Collections.r
 
     Contains:   Collection Manager Interfaces
                 The contents of this header file are deprecated.
 
     Copyright:  © 1989-2011 by Apple Inc. All rights reserved.
*/

#ifndef __COLLECTIONS_R__
#define __COLLECTIONS_R__

#ifndef __CONDITIONALMACROS_R__
#include <CarbonCore/ConditionalMacros.r>
#endif

// 'cltn' - definition of a collection resource (loaded by GetNewCollection)
type 'cltn' {
    longint = $$CountOf(ItemArray);
    array ItemArray
        {
        longint;    // tag
        longint;    // id
            boolean     itemUnlocked            =   false,  // defined attributes bits...
                        itemLocked              =   true;
            boolean     itemNonPersistent       =   false,
                        itemPersistent          =   true;
            unsigned bitstring[14] = 0;                     // reserved attributes bits...
            unsigned bitstring[16];                         // user attributes bits...
        wstring;
        align word;
    };
};

// 'flac' - definition of a flattened collection (created by Flatten[Partial]Collection)
// Note that due to the complexity of this format, it is possible to create 'flac' resources
// using Rez, but it is not possible to DeRez them. DeRez cannot currently handle multiple
// undefined labels as used in this type definition. Instead, DeRez just emits the raw data.
// Some important other limitations:
//     Zero-length items aren't supported.
//     Entries in the item array must be sorted by tag and ID.
//     Entries in the data array must be in the same order as corresponding item array entries.
// Here's an example of using the 'flac' resource type:
//     resource 'flac' (128)
//     {
//         0x40000020,
//         {
//             'TEST', 2, itemUnlocked, itemPersistent, 0x02,
//             'TEST', 3, itemUnlocked, itemPersistent, 0x04,
//             'TEST', 4, itemUnlocked, itemPersistent, 0x08
//         },
//         {
//             "foo",
//             "bird",
//             "The quick brown fox jumped over the lazy dog"
//         }
//     };
type 'flac'
{
    longint = 0x00010000;           // version
    longint noAttributes = 0;           // default collection attributes
    longint = $$CountOf(ItemArray);     // number of items
    array ItemArray         // array of items
    {
        literal longint;    // tag
        longint;            // id
        boolean     itemUnlocked        =   false,  // defined attributes bits
                    itemLocked          =   true;
        boolean     itemNonPersistent   =   false,
                    itemPersistent      =   true;
        unsigned bitstring[14] = 0;                 // reserved attributes bits
        unsigned bitstring[16];                     // user attributes bits
        
        // offset in data block to item data (to the data itself, not to the length)
        longint = (itemData[$$ArrayIndex(ItemArray)] - dataStart) / 8;
    };
dataSize:
    longint = ((dataEnd - dataSize) / 8) - 4;       // size of data
dataStart:
    array ItemDataArray
    {
itemSize:
        // size of this item's data
        longint = ((itemEnd[$$ArrayIndex(ItemDataArray)] - itemSize[$$ArrayIndex(ItemDataArray)]) / 8) - 4;
itemData:
        hex string;         // the item's data
itemEnd:
        ;
        align word;
    };
dataEnd:
    ;
};

#endif /* __COLLECTIONS_R__ */

