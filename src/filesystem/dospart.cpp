#include "filesystem/dospart.h"

using namespace myos::common;
using namespace myos::drivers;
using namespace myos::filesystem;

void printf(const char*, ...);

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
    if(num < 1 && num > 4) {
        return 0;
    }
    printf("\n");
    printf("%d", partitionList[num]->partitionOffset);
    return partitionList[num];
}

void DOSPartitonTable::ReadPartitionTable(AdvancedTechnologyAttachment* hd, PartitionManger* pm) {
    MasterBootRecord mbr;
    printf("MBR:");
    hd->Read28(0, (uint8_t*)&mbr, sizeof(MasterBootRecord));

    //for(int i = 446 + 16; i < 446 + 16 * 2; i++) {
    //    printfHex(((uint8_t*)&mbr)[i]);
    //    printf(" ");
    //}
    //printf(" ");
    //return;

    if(mbr.magicnumber != 0xaa55) {
        printf("ill MBR");
        return;
    }

    for(int i = 0; i < 4; i++) {
        if(mbr.primaryPartition[i].systemID == 0x00) {
            continue;
        }

        printf(" Partition %d", (i & 0xff));

        if (mbr.primaryPartition[i].bootTable == 0x80) {
            printf(" bootable. Type ");
        } else {
            printf(" not bootable. Type ");
        }

        printf("%d", mbr.primaryPartition[i].systemID);
        printf(" ");
        FatPartition* partitionA = new FatPartition(hd, mbr.primaryPartition[i].startLba);
        pm->AddPartition(partitionA);
        printf("\n");
    }
    return;
}