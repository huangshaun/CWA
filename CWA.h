#ifndef CWA_H
#define CWA_H

#include <Arduino.h>
#include "ArduinoJson.h"
#include "CWA_Struct.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <JsonStreamingParser.h>

#define CWA_HOST "opendata.cwa.gov.tw"
#define CWA_API_PATH "/api/v1/rest/datastore/"
#define CWA_API_KEY "api key"

class CWA
{
	private:
		const String element36H = "Wx,PoP,CI,MinT,MaxT";
        const String elementWeek = "最高溫度,最低溫度,最高體感溫度,最低體感溫度,紫外線指數,天氣現象";
		
	public:
		RequestData requestData;
		
		void Begin();
		String getApiID(const String& city, const String& area, ForecastType type);
		String generateUrl(RequestData &requestData);
		bool fetchForecast(WeatherForecastData &data); // 新增方法
};
#endif
/* END OF FILE */
