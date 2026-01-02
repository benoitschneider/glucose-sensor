#include "nvm.h"
#include "config.h"
#include <stdio.h>
#include <string.h>

// --- Mock NVM Implementation --- //
// This is a simple in-memory mock for demonstration purposes.
// In a real system, this would interface with actual non-volatile memory (e.g., Flash, EEPROM, FRAM).

#define NVM_MOCK_STORAGE_SIZE 256 // Example size for mock NVM
static uint8_t nvm_mock_storage[NVM_MOCK_STORAGE_SIZE];
static bool nvm_initialized = false;

// Simple structure to simulate NVM entry (key-value pair)
typedef struct {
    uint16_t key;
    uint8_t  data[sizeof(float)]; // Max size for float
    uint8_t  data_len;            // Length of actual data
    bool     is_valid;
} NVM_Entry_t;

// Using a fixed array of entries for simplicity in mock
#define MAX_NVM_ENTRIES 10
static NVM_Entry_t mock_nvm_entries[MAX_NVM_ENTRIES];

void NVM_Init(void) {
    if (!nvm_initialized) {
        printf("NVM_Init: Initializing mock NVM storage.\n");
        memset(nvm_mock_storage, 0xFF, NVM_MOCK_STORAGE_SIZE); // Simulate erased flash
        memset(mock_nvm_entries, 0, sizeof(mock_nvm_entries));
        nvm_initialized = true;

        // Simulate loading a default value if NVM was 'empty' or corrupted
        // For this mock, we'll just ensure the filter parameter key exists with a default if not set
        float default_param;
        #if (GLUCOSE_FILTER_TYPE == GLUCOSE_FILTER_TYPE_EMA)
        default_param = GLUCOSE_FILTER_DEFAULT_ALPHA;
        #elif (GLUCOSE_FILTER_TYPE == GLUCOSE_FILTER_TYPE_BUTTERWORTH_2ND_ORDER)
        default_param = GLUCOSE_FILTER_DEFAULT_CUTOFF_FREQ;
        #endif

        float read_param;
        if (NVM_ReadFloat(NVM_KEY_FILTER_PARAMETER, &read_param) != NVM_OK) {
            printf("NVM_Init: Filter parameter not found or read failed, writing default (%.4f).\n", default_param);
            NVM_WriteFloat(NVM_KEY_FILTER_PARAMETER, default_param);
        } else {
            printf("NVM_Init: Filter parameter found: %.4f.\n", read_param);
        }
    }
}

static NVM_Entry_t* NVM_FindEntry(uint16_t key) {
    for (int i = 0; i < MAX_NVM_ENTRIES; i++) {
        if (mock_nvm_entries[i].is_valid && mock_nvm_entries[i].key == key) {
            return &mock_nvm_entries[i];
        }
    }
    return NULL;
}

static NVM_Entry_t* NVM_FindEmptyEntry(void) {
    for (int i = 0; i < MAX_NVM_ENTRIES; i++) {
        if (!mock_nvm_entries[i].is_valid) {
            return &mock_nvm_entries[i];
        }
    }
    return NULL;
}

NVM_Status_t NVM_WriteFloat(uint16_t key, float value) {
    if (!nvm_initialized) return NVM_ERROR_NOT_INITIALIZED;

    NVM_Entry_t* entry = NVM_FindEntry(key);
    if (entry == NULL) {
        // Key not found, try to create new entry
        entry = NVM_FindEmptyEntry();
        if (entry == NULL) {
            printf("NVM_WriteFloat: No empty NVM entry available for key 0x%04X.\n", key);
            return NVM_ERROR_WRITE_FAIL; // Out of mock NVM space
        }
        entry->key = key;
        entry->is_valid = true;
    }

    // Copy float bytes to data buffer
    memcpy(entry->data, &value, sizeof(float));
    entry->data_len = sizeof(float);

    printf("NVM_WriteFloat: Wrote key 0x%04X, value %.4f.\n", key, value);
    return NVM_OK;
}

NVM_Status_t NVM_ReadFloat(uint16_t key, float *value) {
    if (!nvm_initialized) return NVM_ERROR_NOT_INITIALIZED;
    if (value == NULL) return NVM_ERROR_READ_FAIL; // Invalid pointer

    NVM_Entry_t* entry = NVM_FindEntry(key);
    if (entry == NULL || entry->data_len != sizeof(float)) {
        printf("NVM_ReadFloat: Key 0x%04X not found or data length mismatch.\n", key);
        return NVM_ERROR_READ_FAIL;
    }

    // Copy data bytes to float
    memcpy(value, entry->data, sizeof(float));

    printf("NVM_ReadFloat: Read key 0x%04X, value %.4f.\n", key, *value);
    return NVM_OK;
}
