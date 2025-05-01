#ifndef __FILESYSTEM__FAT_H
#define __FILESYSTEM__FAT_H

#include <stdint.h>
#include "drivers/ata.h"

namespace myos {
    namespace filesystem {
        struct BiosParameterBlock32 {
            uint8_t jump[3];
            uint8_t oemIdentifier[8];
            uint16_t bytesPerSector; //remember, all numbers are in the little-endian format
            uint8_t sectorsPerCluster;
            uint16_t reservedSectors; //The boot record sectors are included in this value.
            uint8_t fileAllocationTables; //Number of File Allocation Tables (FAT's) on the storage media. Often this value is 2.
            uint16_t rootDirectoryEntries; //(must be set so that the root directory occupies entire sectors).
            uint16_t totalSectors;
            uint8_t mediaType;
            uint16_t fatSectorsCount; //Only Fat12/16
            uint16_t sectorsPerTrack;
            uint16_t headCount;
            uint32_t hiddenSectors;
            uint32_t totalSectorsCount; //This field is set if there are more than 65535 sectors in the volume, resulting in a value which does not fit in the Number of Sectors entry at 0x13.
        
            uint32_t tableSize;
            uint16_t extFlags;
            uint16_t fatVersionNumber;
            uint32_t rootCluster;
            uint16_t fatInfo; //The sector number of the FSInfo structure.
            uint16_t backupBootSector;
            uint8_t reserved0[12];
            uint8_t driveNumber;
            uint8_t reserved;
            uint8_t bootSignature;  //must 0x28 or 0x29s
            uint32_t volumeId;
            uint8_t volumeLabel[11];
            uint8_t fatTypeLable[8];
            //uint8_t hiddenSectors[420];
            //uint16_t hiddenSectors;
        } __attribute__((packed));

        struct DirectoriesFat32 {
            uint8_t name[8];
            uint8_t ext[3];
            uint8_t	fileAttributes;
            uint8_t	reservedForNT;
            uint8_t	cTimeTenth;
            uint16_t cTime; //	The time that the file was created. Multiply Seconds by 2.
            uint16_t cDate; //The date on which the file was created.
            uint16_t aTime; //	Last accessed date. Same format as the creation date.
            uint16_t firstClusterHi; //The high 16 bits of this entrys first cluster number. For FAT 12 and FAT 16 this is always zero.
            uint16_t wTime; //Last modification time. Same format as the creation time.
            uint16_t wDate; //Last modification date. Same format as the creation date.
            uint16_t firstClusterLow; //The low 16 bits of this entrys first cluster number. Use this number to find the first cluster for this entry.
            uint32_t size;
        } __attribute__((packed));

        class FatPartition {
            public:
                FatPartition(drivers::AdvancedTechnologyAttachment* atah, uint32_t partitionOffset);
                ~FatPartition();

                void GetFatFileList(DirectoriesFat32* returnData);
                char* ReadFileName(DirectoriesFat32 fatDirectories);
                char* ReadTxtFile(DirectoriesFat32 fatDirectories);

                uint32_t partitionOffset;
            private:
                BiosParameterBlock32 bpb;
                drivers::AdvancedTechnologyAttachment* hd;

                uint32_t fat_start;
                uint32_t fat_size;
                uint32_t data_start;
                uint32_t root_start;
        };
    }
}

#endif