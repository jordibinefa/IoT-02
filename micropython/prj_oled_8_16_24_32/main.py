'''
Based on: https://github.com/maysrp/ssd1306_font

https://github.com/jordibinefa/IoT-02/tree/master/micropython/prj_oled_8_16_24_32

20220830
'''
from machine import SoftI2C, Pin
from ssd1306 import SSD1306_I2C
from font import Font
import time

i2c = SoftI2C(scl=Pin(22), sda=Pin(21))
display= SSD1306_I2C(128, 32, i2c)

f=Font(display)

def clrscr():
    display.fill(0)

n = 0
y = 0
while True:
    clrscr()
    if y == 0:
        f.text("Fonts:",0,0,8)
        f.text("8",0,22,8)
        f.text("16",20,17,16)
        f.text("24",50,7,24)
        f.text("32",90,2,32)
    if y == 1:
        time.sleep(.5)
        f.text("8px:"+str(n),0,12,8)
    if y == 2:
        f.text("16px:"+str(n),0,8,16)
    if y == 3:
        f.text("24px:"+str(n),0,3,24)
    if y == 4:
        f.text("32px:"+str(n),0,0,32)
    n += 1
    y+=1
    if y > 4:
        y = 0
    f.show()
    time.sleep(.5)
