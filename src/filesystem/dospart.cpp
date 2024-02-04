#include "filesystem/dospart.h"

using namespace myos::common;
using namespace myos::drivers;
using namespace myos::filesystem;

void printf(const char*);
void printfHex(uint8_t);

PartitionManger::PartitionManger()
    : numParts(1) {
}

void PartitionManger::AddPartition(FatPartition* addpart) {
    if(numParts > 4) {
        return;
    }
    partitionList[numParts++] = addpart;
}

FatPartition* PartitionManger::GetPartitionList(uint8_t num) {
    printf("\n");
    printfHex((partitionList[1]->partitionOffset >> 24) & 0xff);
    printfHex((partitionList[1]->partitionOffset >> 16) & 0xff);
    printfHex((partitionList[1]->partitionOffset >> 8) & 0xff);
    printfHex((partitionList[1]->partitionOffset >> 0) & 0xff);
    printf("\n");
    printfHex((partitionList[2]->partitionOffset >> 24) & 0xff);
    printfHex((partitionList[2]->partitionOffset >> 16) & 0xff);
    printfHex((partitionList[2]->partitionOffset >> 8) & 0xff);
    printfHex((partitionList[2]->partitionOffset >> 0) & 0xff);
    return partitionList[num];
}

void DOSPartitonTable::ReadPartitionTable(AdvancedTechnologyAttachment* hd, PartitionManger* pm) {
    MasterBootRecord mbr;
    printf((const char*)"MBR:");
    hd->Read28(0, (uint8_t*)&mbr, sizeof(MasterBootRecord));

    //for(int i = 446 + 16; i < 446 + 16 * 2; i++) {
    //    printfHex(((uint8_t*)&mbr)[i]);
    //    printf((const char*)" ");
    //}
    //printf((const char*)" ");
    //return;

    if(mbr.magicnumber != 0xaa55) {
        printf((const char*)"ill MBR");
        return;
    }

    for(int i = 0; i < 4; i++) {
        if(mbr.primaryPartition[i].systemID == 0x00) {
            continue;
        }

        printf((const char*)" Partition");
        printfHex(i & 0xff);

        if (mbr.primaryPartition[i].bootTable == 0x80) {
            printf((const char*)" bootable. Type ");
        } else {
            printf((const char*)" not bootable. Type ");
        }

        printfHex(mbr.primaryPartition[i].systemID);
        printf((const char*)" ");
        FatPartition partitionA(hd, mbr.primaryPartition[i].startLba);
        pm->AddPartition(&partitionA);
        printf((const char*)"\n");
    }
    return;
}