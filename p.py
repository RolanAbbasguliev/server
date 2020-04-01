#!/usr/bin/python

import os
import sys

#print("Its python Lul")
#print("next output is mine: \n\n\n")

variable = (str)(os.environ['HEADERS_LIST'])

#x = "SIZE_PY: "

#print(x)
#print(variable.__sizeof__() + x.__sizeof__())
#print(variable)

print("HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nConnection: keep-alive\r\n\n <html><head><h1>HELLO, FROM SERVER, MY FRIEND!</h1></head><body><h2>Made by:</h2><p>Naydenovich, Kolesnokov, Zaika</p></body></html>")


