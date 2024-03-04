#ifndef __FILESYSTEM__FAT_H
#define __FILESYSTEM__FAT_H

#include "common/types.h"
#include "drivers/ata.h"

namespace myos {
    namespace filesystem {
        struct BiosParameterBlock32 {
            common::uint8_t jump[3];
            common::uint8_t oemIdentifier[8];
            common::uint16_t bytesPerSector; //remember, all numbers are in the little-endian format
            common::uint8_t sectorsPerCluster;
            common::uint16_t reservedSectors; //The boot record sectors are included in this value.
            common::uint8_t fileAllocationTables; //Number of File Allocation Tables (FAT's) on the storage media. Often this value is 2.
            common::uint16_t rootDirectoryEntries; //(must be set so that the root directory occupies entire sectors).
            common::uint16_t totalSectors;
            common::uint8_t mediaType;
            common::uint16_t fatSectorsCount; //Only Fat12/16
            common::uint16_t sectorsPerTrack;
            common::uint16_t headCount;
            common::uint32_t hiddenSectors;
            common::uint32_t totalSectorsCount; //This field is set if there are more than 65535 sectors in the volume, resulting in a value which does not fit in the Number of Sectors entry at 0x13.
        
            common::uint32_t tableSize;
            common::uint16_t extFlags;
            common::uint16_t fatVersionNumber;
            common::uint32_t rootCluster;
            common::uint16_t fatInfo; //The sector number of the FSInfo structure.
            common::uint16_t backupBootSector;
            common::uint8_t reserved0[12];
            common::uint8_t driveNumber;
            common::uint8_t reserved;
            common::uint8_t bootSignature;  //must 0x28 or 0x29s
            common::uint32_t volumeId;
            common::uint8_t volumeLabel[11];
            common::uint8_t fatTypeLable[8];
            //common::uint8_t hiddenSectors[420];
            //common::uint16_t hiddenSectors;
        } __attribute__((packed));

        struct DirectoriesFat32 {
            common::uint8_t name[8];
            common::uint8_t ext[3];
            common::uint8_t	fileAttributes;
            common::uint8_t	reservedForNT;
            common::uint8_t	cTimeTenth;
            common::uint16_t cTime; //	The time that the file was created. Multiply Seconds by 2.
            common::uint16_t cDate; //The date on which the file was created.
            common::uint16_t aTime; //	Last accessed date. Same format as the creation date.
            common::uint16_t firstClusterHi; //The high 16 bits of this entrys first cluster number. For FAT 12 and FAT 16 this is always zero.
            common::uint16_t wTime; //Last modification time. Same format as the creation time.
            common::uint16_t wDate; //Last modification date. Same format as the creation date.
            common::uint16_t firstClusterLow; //The low 16 bits of this entrys first cluster number. Use this number to find the first cluster for this entry.
            common::uint32_t size;
        } __attribute__((packed));

        class FatPartition {
            public:
                FatPartition(drivers::AdvancedTechnologyAttachment* atah, common::uint32_t partitionOffset);
                ~FatPartition();

                void GetFatFileList(DirectoriesFat32* returnData);
                char* ReadFileName(DirectoriesFat32 fatDirectories);
                char* ReadTxtFile(DirectoriesFat32 fatDirectories);

                common::uint32_t partitionOffset;
            private:
                BiosParameterBlock32 bpb;
                drivers::AdvancedTechnologyAttachment* hd;

                common::uint32_t fat_start;
                common::uint32_t fat_size;
                common::uint32_t data_start;
                common::uint32_t root_start;
        };
    }
}

#endif