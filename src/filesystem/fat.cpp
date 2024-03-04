#include "filesystem/fat.h"

using namespace myos::common;
using namespace myos::drivers;
using namespace myos::filesystem;

void printf(const char*);
void printfHex(uint8_t);

FatPartition::FatPartition(drivers::AdvancedTechnologyAttachment* atah, common::uint32_t partitionOffset) 
            : hd(atah), 
              partitionOffset(partitionOffset) {
    //ReadBiosBlock
    hd->Read28(partitionOffset, (uint8_t*)&bpb, sizeof(BiosParameterBlock32));

    fat_start = partitionOffset + bpb.reservedSectors;
    fat_size = bpb.tableSize;
    data_start = fat_start + fat_size*bpb.fileAllocationTables;
    root_start = data_start + bpb.sectorsPerCluster*(bpb.rootCluster - 2);
}

FatPartition::~FatPartition() {}

void FatPartition::GetFatFileList(DirectoriesFat32* returnData) {
    hd->Read28(root_start, (uint8_t*)returnData, 32*sizeof(DirectoriesFat32));
}

char* FatPartition::ReadFileName(DirectoriesFat32 fatDirectories) {
    if(fatDirectories.name[0] == 0x00) {
        printf("NULL FILE");
        return 0;
    }
    if((fatDirectories.fileAttributes & 0xf) == 0xf) {
        printf("LONG NAME");
        return 0;
    }

    char* foo = "        \0";
    for(int j = 0; j < 8; j++) {
        foo[j] = fatDirectories.name[j];
    }
    return foo;
}

char* FatPartition::ReadTxtFile(DirectoriesFat32 fatDirectories) {
    if((fatDirectories.fileAttributes & 0x10) == 0x10) {
        return 0;
    }

    uint32_t firstFileCluster = ((uint32_t)fatDirectories.firstClusterHi) << 16
                        | ((uint32_t)fatDirectories.firstClusterLow);


    int32_t SIZE = fatDirectories.size;
    uint32_t nextFileCluster = firstFileCluster;
    uint8_t buffer[513];
    uint8_t fatBuffer[513];
    while(SIZE > 0) {
        uint32_t fileSectors = data_start + bpb.sectorsPerCluster * (nextFileCluster - 2);
        int sectorOffset = 0;

        for(;SIZE > 0;SIZE -= 512) {
            hd->Read28(fileSectors + sectorOffset, buffer, 512);
            buffer[SIZE > 512 ? 512 : SIZE] = '\0';
            printf((const char*)buffer);

            if(++sectorOffset >= bpb.sectorsPerCluster) {
                return (char*)buffer;
            }
        }

        //TODO:WHY?
        uint32_t fatSectorForCurrentCluster = nextFileCluster / (512/sizeof(uint32_t));
        hd->Read28(fat_start + fatSectorForCurrentCluster, fatBuffer, 512);
        uint32_t fatOffsetInSectorForCurrentCluster = nextFileCluster % (512/sizeof(uint32_t));
        nextFileCluster = ((uint32_t*)&fatBuffer)[fatOffsetInSectorForCurrentCluster] & 0x0FFFFFFF;
    }
}