#!/usr/bin/python

import os
import sys

variable = (str)(os.environ['HEADERS_LIST'])
#### variable2 = (int)(os.environ['TEST_M_CON']) ####

print("HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nConnection: close\r\n\n <html><head><h1>HELLO FROM SERVER, MY FRIEND!</h1></head><body><h2>Made by:</h2><p>Naydenovich, Kolesnokov, Zaika</p><h2>Test images</h2><ul><li><a href=\"files/img.jpeg\">Yoda</a></li><li><a href=\"files/img2.jpeg\">Squirrel</a></li><li><a href=\"files/img3.png\">Squirrel</a></li></ul></body></html>")


