# SparkWeather
Spark Core Weather Station that sends data to Weather Underground

WiFi Weather Station (Spark Core Version)
Spark Core Wifi Microcontroller Weather station that sends data to wunderground.com
Tacked together by Dan Fein

Notes:
-This is a draft and does not include all the sensors a weather station often would, just temp and humidity for now. More to come.
-This project assumes battery power and has options to save power it will rest for awhile, wake, send data and rest again.
-You will need to sign up for a free account at wunderground.com, to get your pass
When you register a station you will get an ID (If you send data you get ad free membership free)
Sign up at http://www.wunderground.com/personal-weather-station/signup.asp
-Wunderground wants UTC Zulu, not local time, if your RTC is local, offset it in code.
Wunderground Upload guidelines: http://wiki.wunderground.com/index.php/PWS_-_Upload_Protocol
-This code assumes your wifi credentials are already saved in the CC3000's non-volatile memory
