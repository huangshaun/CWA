#include "CWA.h"
#include <WiFi.h>

CWA cwa;
WeatherForecastData data;

char WIFI_SSID[] = "ssid";
char WIFI_PWD[]  = "pw";

void setup()
{
  Serial.begin(115200);
  delay(5000);

  WiFi.begin(WIFI_SSID, WIFI_PWD);
  while (WiFi.status() != WL_CONNECTED)
  {
      delay(500);
      Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  cwa.Begin();
/*
  Serial.println(cwa.getApiID("桃園市","", FT_WEEK));
  Serial.println(cwa.getApiID("桃園市","桃園區", FT_WEEK));
  Serial.println(cwa.getApiID("桃園市","桃園區", FT_3DAY));
  Serial.println(cwa.getApiID("桃園市","", FT_36H));
  Serial.println(cwa.getApiID("新北市","", FT_36H));

  // 範例：36 小時
    cwa.requestData.city = "桃園市";
    cwa.requestData.area = ""; // 36H 忽略
    cwa.requestData.type = FT_36H;

    Serial.println("36H URL:");
    Serial.println(cwa.generateUrl(cwa.requestData));

    // 範例：一週，指定區域
    cwa.requestData.city = "桃園市";
    cwa.requestData.area = "桃園區"; 
    cwa.requestData.type = FT_WEEK;

    Serial.println("Week URL (指定區域):");
    Serial.println(cwa.generateUrl(cwa.requestData));

    // 範例：一週，沒指定區域
    cwa.requestData.city = "桃園市";
    cwa.requestData.area = ""; 
    cwa.requestData.type = FT_WEEK;

    Serial.println("Week URL (未指定區域):");
    Serial.println(cwa.generateUrl(cwa.requestData));
*/
  cwa.requestData.city = "桃園市";
      cwa.requestData.area = ""; 
      cwa.requestData.type = FT_36H;

      Serial.println("=== 36H URL ===");
      Serial.println(cwa.generateUrl(cwa.requestData));

      if (cwa.fetchForecast(data))
      {
          Serial.println("36H fetch OK");
          for (int i = 0; i < 14; i++)
          {
              if (data.slots[i].startTime.length() == 0) break;
              Serial.printf("[%02d] %s ~ %s | Wx:%s | PoP:%d%% | CI:%s | MinT:%d | MaxT:%d\n",
                            i,
                            data.slots[i].startTime.c_str(),
                            data.slots[i].endTime.c_str(),
                            data.slots[i].wx.c_str(),
                            data.slots[i].pop,
                            data.slots[i].ci.c_str(),
                            data.slots[i].minT,
                            data.slots[i].maxT);
          }
      }

      // Week 測試
      cwa.requestData.city = "桃園市";
      cwa.requestData.area = "桃園區";
      cwa.requestData.type = FT_WEEK;

      Serial.println("\n=== Week URL ===");
      Serial.println(cwa.generateUrl(cwa.requestData));

      if (cwa.fetchForecast(data))
      {
        Serial.println("Week fetch OK");
        for (int i = 0; i < 14; i++)
        {
            if (data.slots[i].startTime.length() == 0) 
            {
                break;
            }
            
            // 加入 MaxAT 與 MinAT 的顯示
            Serial.printf("[%02d] %s ~ %s | Wx:%-10s | MaxT:%d | MinT:%d | MaxAT:%d | MinAT:%d | UV:%d\n",
                          i,
                          data.slots[i].startTime.c_str(),
                          data.slots[i].endTime.c_str(),
                          data.slots[i].wx.c_str(),
                          data.slots[i].maxT,
                          data.slots[i].minT,
                          data.slots[i].maxApparentT,
                          data.slots[i].minApparentT,
                          data.slots[i].uv);
        }
      }
}

void loop()
{

}
