#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include "ArduinoJson.h"
#include "CWA.h"
#include "CWA_Struct.h"

void CWA::Begin()
{
    requestData.apiKey = CWA_API_KEY;
}

String CWA::getApiID(const String& city, const String& area, ForecastType type)
{
    for (int i = 0; i < cityMapSize; i++)
    {
        if (city == cityMap[i].cityName)
        {
            if (type == FT_36H)
            {
                return cityMap[i].api36H;
            }
            else if (type == FT_3DAY)
            {
                return cityMap[i].api3Day;
            }
            else if (type == FT_WEEK)
            {
                if (area.length() > 0)
                {
                    return cityMap[i].apiWeek;
                }
                else
                {
                    return "F-D0047-091";
                }
            }
        }
    }
    Serial.println("Error: city not found!");
    return "";
}

String CWA::generateUrl(RequestData &requestData)
{
    if (requestData.city.length() == 0)
    {
        Serial.println("Error: city is required");
        return "";
    }

    String apiID = getApiID(requestData.city, requestData.area, requestData.type);
    if (apiID == "")
    {
        return "";
    }

    String url = "https://" + String(CWA_HOST) + String(CWA_API_PATH) + apiID;
    url += "?Authorization=" + requestData.apiKey;

    if (requestData.type == FT_36H)
    {
        url += "&locationName=" + requestData.city;
        url += "&elementName=" + element36H;
    }
    else if (requestData.type == FT_WEEK)
    {
        if (requestData.area.length() > 0)
        {
            url += "&LocationName=" + requestData.area;
        }
        else
        {
            url += "&LocationName=" + requestData.city;
        }
        url += "&ElementName=" + elementWeek;
    }

    return url;
}

bool CWA::fetchForecast(WeatherForecastData &data)
{
    if (requestData.city.length() == 0)
    {
        return false;
    }

    String url = generateUrl(requestData);
    if (url == "")
    {
        return false;
    }

    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    http.setTimeout(15000); 

    if (!http.begin(client, url))
    {
        return false;
    }
    
    int httpCode = http.GET();
    if (httpCode != HTTP_CODE_OK)
    {
        Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
        http.end();
        return false;
    }

    String payload = http.getString();
    if (payload.length() == 0)
    {
        http.end();
        return false;
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload, DeserializationOption::NestingLimit(20));

    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        http.end();
        return false;
    }

    JsonObject records = doc["records"];
    JsonArray locationArray;

    // 修正警告：使用 .is<JsonArray>() 代替 containsKey
    if (records["Locations"].is<JsonArray>())
    {
        locationArray = records["Locations"][0]["Location"];
    }
    else if (records["locations"].is<JsonArray>())
    {
        locationArray = records["locations"][0]["location"];
    }
    else 
    {
        locationArray = records["location"];
    }

    if (locationArray.isNull() || locationArray.size() == 0)
    {
        http.end();
        return false;
    }

    JsonObject location = locationArray[0];
    JsonArray weatherElements = location["weatherElement"] | location["WeatherElement"].as<JsonArray>();

    for (JsonObject element : weatherElements)
    {
        String name = element["elementName"] | element["ElementName"].as<String>();
        JsonArray timeSlots = element["time"] | element["Time"].as<JsonArray>();

        for (int i = 0; i < 14 && i < timeSlots.size(); i++)
        {
            JsonObject slot = timeSlots[i];
            String st = slot["startTime"] | slot["StartTime"].as<String>();
            String et = slot["endTime"] | slot["EndTime"].as<String>();

            if (st.length() >= 19) 
            {
                st = st.substring(0, 19);
                st.setCharAt(10, ' ');
            }
            if (et.length() >= 19) 
            {
                et = et.substring(0, 19);
                et.setCharAt(10, ' ');
            }

            data.slots[i].startTime = st;
            data.slots[i].endTime = et;

            String val;
            // 修正：將 JsonVariant 改為 JsonVariantConst 以符合 ArduinoJson 7 的類型限制
            if (slot["elementValue"] || slot["ElementValue"])
            {
                JsonVariantConst valObj = (slot["elementValue"] | slot["ElementValue"])[0];
                
                val = valObj["value"].as<String>();
                if (val == "null")
                {
                    val = valObj["MaxTemperature"].as<String>();
                }
                if (val == "null")
                {
                    val = valObj["MinTemperature"].as<String>();
                }
                if (val == "null")
                {
                    val = valObj["Weather"].as<String>();
                }
                if (val == "null")
                {
                    val = valObj["UVIndex"].as<String>();
                }
                if (val == "null")
                {
                    val = valObj["MaxApparentTemperature"].as<String>();
                }
                if (val == "null")
                {
                    val = valObj["MinApparentTemperature"].as<String>();
                }
            }
            else if (slot["parameter"])
            {
                val = slot["parameter"]["parameterName"].as<String>();
            }

            if (name == "Wx" || name == "天氣現象")
            {
                data.slots[i].wx = val;
            }
            else if (name == "MaxT" || name == "最高溫度")
            {
                data.slots[i].maxT = val.toInt();
            }
            else if (name == "MinT" || name == "最低溫度")
            {
                data.slots[i].minT = val.toInt();
            }
            else if (name == "PoP" || name == "降雨機率")
            {
                data.slots[i].pop = val.toInt();
            }
            else if (name == "UVI" || name == "紫外線指數")
            {
                data.slots[i].uv = val.toInt();
            }
            else if (name == "CI")
            {
                data.slots[i].ci = val;
            }
            else if (name == "MaxAT" || name == "最高體感溫度")
            {
                data.slots[i].maxApparentT = val.toInt();
            }
            else if (name == "MinAT" || name == "最低體感溫度")
            {
                data.slots[i].minApparentT = val.toInt();
            }
        }
    }

    http.end();
    return true;
}
