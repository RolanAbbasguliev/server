#!/usr/bin/python

import os
import sys

#print("Its python Lul")
#print("next output is mine: \n\n\n")

variable = (str)(os.environ['HEADERS_LIST'])

variable2 = (int)(os.environ['TEST_M_CON'])

#x = "SIZE_PY: "

#print(x)
#print(variable.__sizeof__() + x.__sizeof__())
#print(variable)

if variable2 == 0:
    print("HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nConnection: keep-alive\r\n\n <html><head><h1>HELLO FROM SERVER, MY FRIEND!</h1></head><body><h2>Made by:</h2><p>Naydenovich, Kolesnokov, Zaika</p></body></html>")
else:
    print("HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nConnection: keep-alive\r\n\n <html><head><h1>HELLO FROM HELL, MY FRIEND!</h1></head><body><h2>Made by:</h2><p>Naydenovich, Kolesnokov, Zaika</p></body></html>")

#print("HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nConnection: keep-alive\r\n\n <html><head><h1>HELLO, FROM SERVER, MY FRIEND!</h1></head><body><h2>Made by:</h2><p>Naydenovich, Kolesnokov, Zaika</p></body></html>")


