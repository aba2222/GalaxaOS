#ifndef __FILESYSTEM__DOSPART_H
#define __FILESYSTEM__DOSPART_H

#include <stdint.h>
#include "drivers/ata.h"
#include "filesystem/fat.h"

namespace myos {
    namespace filesystem {
        struct PartitionTable {
            uint8_t	bootTable; //flag: 0 = no, 0x80 = bootable (or "active")

            uint8_t	startHead;
            uint8_t	startSector : 6; //(Bits 6-7 are the upper two bits for the Starting Cylinder field.)
            uint16_t startCylinder : 10;

            uint8_t	systemID;

            uint8_t	endHead;
            uint8_t	endSector : 6; //Bits 6-7 are the upper two bits for the ending cylinder field
            uint16_t endCylinder : 10;

            uint32_t startLba; //to start of partition -- also equals the partition's starting LBA value
            uint32_t sectorsInPartition;
        } __attribute__((packed));

        struct MasterBootRecord {
            uint8_t MBRBootstrap[440]; //(flat binary executable code)
            uint32_t signature;
            uint16_t unsed; //Optional, reserved 0x00003

            PartitionTable primaryPartition[4];

            uint16_t magicnumber; //(0x55,0xAA)Valid bootsector
        } __attribute__((packed));

        class PartitionManger {
        public:
            PartitionManger();
            void AddPartition(FatPartition* addpart);
            FatPartition* GetPartitionList(uint8_t num);
            
            FatPartition* partitionList[5];
            uint8_t numParts;
        };

        class DOSPartitonTable {
        public:
            static void ReadPartitionTable(myos::drivers::AdvancedTechnologyAttachment* hd, PartitionManger* pm);
        };
    }
}

#endif