#ifndef AI_INFERENCE_H
#define AI_INFERENCE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum {
    IRIS_SETOSA = 0,
    IRIS_VERSICOLOR = 1,
    IRIS_VIRGINICA = 2,
    IRIS_UNKNOWN = 3
} IrisClass;

typedef struct {
    float sepal_length;
    float sepal_width;
    float petal_length;
    float petal_width;
} IrisInput;

typedef struct {
    IrisClass predicted_class;
    float confidence;
    float probabilities[3];
} IrisResult;

int AI_Inference_Init(void);
int AI_Inference_Predict(const IrisInput* input, IrisResult* result);
void AI_Inference_Deinit(void);
const char* AI_Inference_GetClassName(IrisClass cls);
int AI_Inference_GetInputSize(void);
int AI_Inference_GetOutputSize(void);

#ifdef __cplusplus
}
#endif

#endif
