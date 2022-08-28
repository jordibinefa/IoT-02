# Complete project details at https://RandomNerdTutorials.com

from machine import Pin, SoftI2C
import ssd1306
from time import sleep

# ESP32 Pin assignment 
i2c = SoftI2C(scl=Pin(22), sda=Pin(21))

# ESP8266 Pin assignment
#i2c = SoftI2C(scl=Pin(5), sda=Pin(4))

oled_width = 128
oled_height = 64
oled = ssd1306.SSD1306_I2C(oled_width, oled_height, i2c)

oled.text('Reading BME280', 0, 20)
        
oled.show()

import BME280

while True:
  bme = BME280.BME280(i2c=i2c)
  temp = bme.temperature
  hum = bme.humidity
  pres = bme.pressure
  # uncomment for temperature in Fahrenheit
  #temp = (bme.read_temperature()/100) * (9/5) + 32
  #temp = str(round(temp, 2)) + 'F'
  tempOled = 'Temp: '+ temp+'                '
  # print('Temperature: ', temp)
  print(tempOled)
  oled = ssd1306.SSD1306_I2C(oled_width, oled_height, i2c)
  oled.text(tempOled,0,0)
  # print('Humidity: ', hum)
  humOled = 'Hum: '+hum+'                '
  print(humOled)
  oled.text(humOled,0,10)
  # print('Pressure: ', pres)
  pressOled = 'Pres: '+ pres+'                '
  print(pressOled)
  oled.text(pressOled,0,20)
  oled.show()

  sleep(5)

