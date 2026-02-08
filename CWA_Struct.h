#ifndef CWBStruct_H
#define CWBStruct_H

#include <Arduino.h>

//////////////////////////////
// City to API Map
//////////////////////////////
struct CityApiMap
{
    const char* cityName;
    const char* api36H;   // 縣市 36 小時
    const char* api3Day;  // 縣市 3 天
    const char* apiWeek;  // 縣市週預報
};

const CityApiMap cityMap[] =
{
    {"宜蘭縣", "F-C0032-001","F-D0047-001", "F-D0047-003"},
    {"桃園市", "F-C0032-001", "F-D0047-005", "F-D0047-007"},
    {"新竹縣", "F-C0032-001", "F-D0047-009", "F-D0047-011"},
    {"苗栗縣", "F-C0032-001", "F-D0047-013", "F-D0047-015"},
    {"彰化縣", "F-C0032-001", "F-D0047-017", "F-D0047-019"},
    {"南投縣", "F-C0032-001", "F-D0047-021", "F-D0047-023"},
    {"雲林縣", "F-C0032-001", "F-D0047-025", "F-D0047-027"},
    {"嘉義縣", "F-C0032-001", "F-D0047-029", "F-D0047-031"},
    {"屏東縣", "F-C0032-001", "F-D0047-033", "F-D0047-035"},
    {"臺東縣", "F-C0032-001", "F-D0047-037", "F-D0047-039"},
    {"花蓮縣", "F-C0032-001", "F-D0047-041", "F-D0047-043"},
    {"澎湖縣", "F-C0032-001", "F-D0047-045", "F-D0047-047"},
    {"基隆市", "F-C0032-001", "F-D0047-049", "F-D0047-051"},
    {"新竹市", "F-C0032-001", "F-D0047-053", "F-D0047-055"},
    {"嘉義市", "F-C0032-001", "F-D0047-057", "F-D0047-059"},
    {"臺北市", "F-C0032-001", "F-D0047-061", "F-D0047-063"},
    {"高雄市", "F-C0032-001", "F-D0047-065", "F-D0047-067"},
    {"新北市", "F-C0032-001", "F-D0047-069", "F-D0047-071"},
    {"臺中市", "F-C0032-001", "F-D0047-073", "F-D0047-075"},
    {"臺南市", "F-C0032-001", "F-D0047-077", "F-D0047-079"},
    {"連江縣", "F-C0032-001", "F-D0047-081", "F-D0047-083"},
    {"金門縣", "F-C0032-001", "F-D0047-085", "F-D0047-087"},
    {"全臺", "F-C0032-001", "F-D0047-089", "F-D0047-091"}
};
const uint8_t cityMapSize = sizeof(cityMap)/sizeof(CityApiMap);

//////////////////////////////
// 單筆時間 slot
//////////////////////////////
struct WeatherSlot
{
    String startTime;      // 開始時間
    String endTime;        // 結束時間

    String wx;             // 天氣現象
    int    pop;            // 降雨機率 %
    int    minT;           // 最低溫
    int    maxT;           // 最高溫
    int    minApparentT;   // 最低體感溫度 (新增)
    int    maxApparentT;   // 最高體感溫度 (新增)
    String ci;             // 舒適度
    int    uv;             // 紫外線指數
};

//////////////////////////////
// 預報規格
//////////////////////////////
enum ForecastType 
{
    FT_36H,   // 2-Day 36小時
    FT_3DAY,  // 3-Day 未來三天
    FT_WEEK   // 一週
};

//////////////////////////////
// 預報資料結構
//////////////////////////////
struct WeatherForecastData
{
    bool result;
    String description;
    String locationName;
    String apiID;
    String dataFormat;
    String latitude;
    String longitude;

    WeatherSlot slots[14];  // 最大 14 slot 適合 week 預報
};

//////////////////////////////
// Request 資料結構
//////////////////////////////
struct RequestData
{
    String apiKey;
    String city;
    String area;
    ForecastType type;
};

#endif
