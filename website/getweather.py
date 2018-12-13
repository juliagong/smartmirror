#!/usr/bin/env python
import pyowm

# Free API key
owm = pyowm.OWM('REDACTED')

# City ID for Palo Alto
city_id = 5380748   
observation = owm.weather_at_id(city_id)               
weather = observation.get_weather()                    

# Current weather status
weather_status = weather.get_detailed_status().replace(" ", "-");

# Current temp in fahrenheit
temp_curr_F =  list(weather.get_temperature('fahrenheit').values())[0]

# Split into integer and decimal parts
temp_F_int = str(temp_curr_F).split('.')[0]
temp_F_dec = str(temp_curr_F).split('.')[1]

# Current temp in celsius
temp_curr_C =  list(weather.get_temperature('celsius').values())[0]

# Split into integer and decimal parts
temp_C_int = str(temp_curr_C).split('.')[0]
temp_C_dec = str(temp_curr_C).split('.')[1]

# Other current weather data
wind_speed = int(list(weather.get_wind('miles_hour').values())[0])
humidity = weather.get_humidity()


######## Begin printed output #######

print ("Content-type: text/plain\n")

print(" *" + weather_status)
print(" *" + str(temp_F_int))
print(" *" + str(temp_F_dec))
print(" *" + str(temp_C_int))
print(" *" + str(temp_C_dec))
print(" *" + str(wind_speed) + " ")
print(" *" + str(humidity) + "% ")
# 2xx Thunder, 3xx&5xx rain, 6xx snow, 7xx 'atmosphere', 800 clear, 8xx clouds
print(" *" + str(weather.get_weather_code()) + " ")

######## End printed output #######

# Disable until can represent forecast coherently 
# Currently gets weather forecast for next 5 days in 3 hour increments
# The reference time for each weather object is also in UTC. 

# # forecaster = owm.three_hours_forecast_at_id(city_id)    # Get Forecaster object
# forecast = forecaster.get_forecast()                   # Get Forecast object
# weathers = forecast.get_weathers()
# n_weathers = forecast.count_weathers()

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
