#ifndef STATISTICS_H
#define STATISTICS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "ai_inference.h"

typedef struct {
    uint32_t total_count;
    uint32_t correct_count;
    uint32_t confusion_matrix[3][3];
    float accuracy;
    float precision[3];
    float recall[3];
} Statistics;

void Statistics_Init(void);
void Statistics_Update(IrisClass predicted, IrisClass actual);
void Statistics_Reset(void);
const Statistics* Statistics_Get(void);
void Statistics_Print(void);

#ifdef __cplusplus
}
#endif

#endif
