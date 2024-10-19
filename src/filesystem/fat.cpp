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
        return "NULL FILE";
    }
    if((fatDirectories.fileAttributes & 0xf) == 0xf) {
        return "LONG NAME";
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
    uint32_t nextFileCluster = firstFileCluster;
    uint8_t* buffer = (uint8_t*)"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    uint32_t fileSectors = data_start + bpb.sectorsPerCluster * (nextFileCluster - 2);
    hd->Read28(fileSectors, buffer, 50);
    buffer[fatDirectories.size] = '\0';
    printf((const char*)buffer);
    return (char*)buffer;
}