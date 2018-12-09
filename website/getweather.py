import pyowm

# Free API key
owm = pyowm.OWM('2fc3793643d86bd1743513bbed775d24')

# http://samples.openweathermap.org/data/2.5/weather?lat=37.4422&lon=-122.1635&appid=2fc3793643d86bd1743513bbed775d24

#Palo ALto coords [37.4422, -122.1635]
#City ID: 5380748
place = owm.weather_at_coords(37.4422, -122.1635)
weather = place.get_weather()
temp_curr_F =  list(weather.get_temperature('fahrenheit').values())[0]
temp_max_F = list(weather.get_temperature('fahrenheit').values())[1]
temp_min_F = list(weather.get_temperature('fahrenheit').values())[2]
temp_curr_C =  list(weather.get_temperature('celsius').values())[0]
temp_max_C = list(weather.get_temperature('celsius').values())[1]
temp_min_C = list(weather.get_temperature('celsius').values())[2]
wind_speed = list(weather.get_wind().values())[0]
humidity = weather.get_humidity()
print("The weather in Palo Alto is currently " + weather.get_detailed_status())
print("Temperature: " + str(temp_curr_F) + " F / " + str(temp_curr_C) + " C")
print("Max: " + str(temp_max_F) + " F / " + str(temp_max_C) + " C")
print("Min: " + str(temp_min_F) + " F / " + str(temp_min_C) + " C")
print("Wind: " + str(wind_speed) + " MPH")
print("Humidity: " + str(humidity) + "%")
