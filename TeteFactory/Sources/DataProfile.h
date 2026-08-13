#pragma once

#include "tlTeteDefines.h"
class CDataProfile
{
public:
    CDataProfile();
    CDataProfile(const char *sDataFile);
	~CDataProfile(void);

	void SetDataFile(const char *sFileName);
    bool SetDataValue(const char *sSegName, const char *sKeyName, bool value); 
    bool SetDataValue(const char *sSegName, const char *sKeyName, int8_t value);
    bool SetDataValue(const char *sSegName, const char *sKeyName, int32_t value);
    bool SetDataValue(const char *sSegName, const char *sKeyName, uint32_t value);
    bool SetDataValue(const char *sSegName, const char *sKeyName, float value);
    bool SetDataValue(const char *sSegName, const char *sKeyName, double value);
    bool SetDataValue(const char *sSegName, const char *sKeyName, const char *value);

    bool GetBoolValue(const char *sSegName, const char *sKeyName, bool defValue);
    int8_t GetInt8Value(const char *sSegName, const char *sKeyName, int8_t defValue);
    int32_t GetInt32Value(const char *sSegName, const char *sKeyName, int32_t defValue);
    uint32_t GetUnsignedValue(const char *sSegName, const char *sKeyName, uint32_t defValue);
    float GetFloatValue(const char *sSegName, const char *sKeyName, float defValue);
    double GetDoubleValue(const char *sSegName, const char *sKeyName, double defValue);
    char *GetStringValue(const char *sSegName, const char *sKeyName, const char *defValue);

private:
	char m_sFileName[MAX_FILE_NAME];
    char m_sDataValue[MAX_FILE_NAME];
};

