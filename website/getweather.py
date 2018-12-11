#!/usr/bin/env python
import pyowm

# Free API key
owm = pyowm.OWM('2fc3793643d86bd1743513bbed775d24')

city_id = 5380748   # City ID for Palo Alto


observation = owm.weather_at_id(city_id)                # Get Observation object
forecaster = owm.three_hours_forecast_at_id(city_id)    # Get Forecaster object

weather = observation.get_weather()                     # Get Weather object
forecast = forecaster.get_forecast()                   # Get Forecast object
weathers = forecast.get_weathers()
n_weathers = forecast.count_weathers()


temp_curr_F =  list(weather.get_temperature('fahrenheit').values())[0]
# temp_max_F = list(weather.get_temperature('fahrenheit').values())[1]
# temp_min_F = list(weather.get_temperature('fahrenheit').values())[2]
temp_curr_C =  list(weather.get_temperature('celsius').values())[0]
# temp_max_C = list(weather.get_temperature('celsius').values())[1]
# temp_min_C = list(weather.get_temperature('celsius').values())[2]
wind_speed = list(weather.get_wind('miles_hour').values())[0]
humidity = weather.get_humidity()

print "Content-type: text/html"
# Have to print <html> tags for page to display

print """
<html><body>
"""
print("<br>UTC: " + str( weather.get_reference_time('date') ) )
print("<br>Location: *" + observation.get_location().get_name() + "\n<br>Weather: *" + weather.get_detailed_status())
print("<br>Temperature: *" + str(temp_curr_F) + " F / *" + str(temp_curr_C) + " C")
# print("<br>Max: *" + str(temp_max_F) + " F / *" + str(temp_max_C) + " C")
# print("<br>Min: *" + str(temp_min_F) + " F / *" + str(temp_min_C) + " C")
print("<br>Wind: *" + str(wind_speed) + " MPH")
print("<br>Humidity: *" + str(humidity) + "%")
# Prints the OMW weather code. There are only 6 corresponding weather icon groups
# 2xx Thunder, 3xx&5xx rain, 6xx snow, 7xx 'atmosphere', 800 clear, 8xx clouds
print("<br>Weather Code: *" + str(weather.get_weather_code()))
# # Returns times in UST. Don't want to bother with conversion
# print("\nSunset: *" + str( weather.get_sunset_time('iso') ) )
# print("\nSunrise: *" + str( weather.get_sunrise_time('iso') ) )
# # Rain & Snow are empty when tested... probably don't return any data unless it's actually raining or snowing
# print(list(weather.get_rain()))
# print(list(weather.get_snow()))


# Disable until can represent forecast coherently 
# print("\n5-Day Weather Forecast (in 3-hour increments): \n")

# for i in range(n_weathers):
#     weather = weathers[i]
#     temp_curr_F =  list(weather.get_temperature('fahrenheit').values())[0]
#     temp_max_F = list(weather.get_temperature('fahrenheit').values())[1]
#     temp_min_F = list(weather.get_temperature('fahrenheit').values())[2]
#     temp_curr_C =  list(weather.get_temperature('celsius').values())[0]
#     temp_max_C = list(weather.get_temperature('celsius').values())[1]
#     temp_min_C = list(weather.get_temperature('celsius').values())[2]

#     print("<br>Weather Forecast for UTC: " + str( weather.get_reference_time('date') ) )
#     print("<br>")
#     print(weather.get_detailed_status())
#     print("<br>Temperature: " + str(temp_curr_F) + " F / " + str(temp_curr_C) + " C")
#     print("<br>Max: " + str(temp_max_F) + " F / " + str(temp_max_C) + " C")
#     print("<br>Min: " + str(temp_min_F) + " F / " + str(temp_min_C) + " C")print("<br>")
#     print("")


print """
</body></html>
"""
