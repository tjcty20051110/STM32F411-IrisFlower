#include "statistics.h"
#include "serial_handler.h"
#include <stdio.h>
#include <string.h>

static Statistics stats;

void Statistics_Init(void)
{
    memset(&stats, 0, sizeof(Statistics));
}

void Statistics_Update(IrisClass predicted, IrisClass actual)
{
    if (predicted >= 0 && predicted <= 2 && actual >= 0 && actual <= 2) {
        stats.total_count++;
        stats.confusion_matrix[actual][predicted]++;
        
        if (predicted == actual) {
            stats.correct_count++;
        }
        
        if (stats.total_count > 0) {
            stats.accuracy = (float)stats.correct_count / (float)stats.total_count * 100.0f;
        }
        
        {
            int i, j;
            uint32_t tp, fp, fn;
            
            for (i = 0; i < 3; i++) {
                tp = stats.confusion_matrix[i][i];
                
                fp = 0;
                for (j = 0; j < 3; j++) {
                    if (j != i) {
                        fp += stats.confusion_matrix[j][i];
                    }
                }
                
                fn = 0;
                for (j = 0; j < 3; j++) {
                    if (j != i) {
                        fn += stats.confusion_matrix[i][j];
                    }
                }
                
                if ((tp + fp) > 0) {
                    stats.precision[i] = (float)tp / (float)(tp + fp) * 100.0f;
                }
                
                if ((tp + fn) > 0) {
                    stats.recall[i] = (float)tp / (float)(tp + fn) * 100.0f;
                }
            }
        }
    }
}

void Statistics_Reset(void)
{
    memset(&stats, 0, sizeof(Statistics));
}

const Statistics* Statistics_Get(void)
{
    return &stats;
}

void Statistics_Print(void)
{
    char buffer[256];
    int len;
    
    len = snprintf(buffer, sizeof(buffer), 
        "\r\n=== Statistics ===\r\n"
        "Total: %lu, Correct: %lu\r\n"
        "Accuracy: %.2f%%\r\n",
        (unsigned long)stats.total_count,
        (unsigned long)stats.correct_count,
        stats.accuracy);
    Serial_Handler_SendString(buffer);
    
    Serial_Handler_SendString("Confusion Matrix:\r\n");
    Serial_Handler_SendString("         Pred:Set  Ver  Vir\r\n");
    
    {
        int i, j;
        const char* names[] = {"Actual:Set", "Actual:Ver", "Actual:Vir"};
        
        for (i = 0; i < 3; i++) {
            len = snprintf(buffer, sizeof(buffer), "%s  ", names[i]);
            for (j = 0; j < 3; j++) {
                len += snprintf(buffer + len, sizeof(buffer) - len, "%4lu ", 
                    (unsigned long)stats.confusion_matrix[i][j]);
            }
            len += snprintf(buffer + len, sizeof(buffer) - len, "\r\n");
            Serial_Handler_SendString(buffer);
        }
    }
    
    Serial_Handler_SendString("Precision/Recall:\r\n");
    {
        int i;
        const char* names[] = {"Setosa", "Versicolor", "Virginica"};
        for (i = 0; i < 3; i++) {
            len = snprintf(buffer, sizeof(buffer), 
                "%s: P=%.1f%% R=%.1f%%\r\n",
                names[i], stats.precision[i], stats.recall[i]);
            Serial_Handler_SendString(buffer);
        }
    }
    Serial_Handler_SendString("==================\r\n");
}
