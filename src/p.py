#!/usr/bin/python

import os
import sys

variable = (str)(os.environ['HEADERS_LIST'])
#### variable2 = (int)(os.environ['TEST_M_CON']) ####

response = ""

response += "HTTP/1.1 200 OK\r\n"
response += "Content-Type: text/html; charset=utf-8\r\n"
response += "Content-Length: 487\r\n"
response += "Connection: close\r\n\n"
response += "<html>"
response += "<head><h1>HELLO FROM SERVER, MY FRIEND!</h1></head>"
response += "<link rel=\"icon\" href=\"favicon.ico\">"
response += "<title>Welcome!</title>"
response += "<style>"
response += "body {background-image: url(files/backgr.png);}" 
response += "</style>"
response += "<body>"
response += "<h2>Made by:</h2>"
response += "<p>Naydenovich, Kolesnokov, Zaika</p>"
response += "<h2>Test images</h2>"
response += "<ul>"
response += "<li><a href=\"files/img.jpeg\">Yoda</a></li>"
response += "<li><a href=\"files/img2.jpeg\">Squirrel</a></li>"
response += "<li><a href=\"files/img3.png\">Squirrel</a></li>"
response += "</ul>"
response += "<h3>Music:</h3>"
response += "<ul>"
response += "<li><a href=\"files/music.mp3\">best song ever</a></li>"
response += "</ul>"
response += "</body>"
response += "</html>"


print(response)


