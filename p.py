#!/usr/bin/python

import os
import sys

variable = (str)(os.environ['HEADERS_LIST'])
#variable2 = (int)(os.environ['TEST_M_CON'])

print("HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nConnection: keep-alive\r\n\n <html><head><h1>HELLO FROM SERVER, MY FRIEND!</h1></head><body><h2><a href=\"img.jpeg\">image</a>Made by:</h2><p>Naydenovich, Kolesnokov, Zaika</p></body></html>")


