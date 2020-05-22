#!/usr/bin/env python3

import os
import sys

files_path = (str)(os.environ['F_PATH'])

body = ""

body += "<html>"
body += "<head>"
body += "<link rel=\"icon\" href=\"" + files_path + "/favicon.ico\">"
body += "<title>Welcome!</title>"
body += "<style>"
body += "body {background-image: url(" + files_path + "/backgr.png);}" 
body += "</style>"
body += "</head>"
body += "<body>"
body += "<h2>Made by:</h2>"
body += "<p>Naydenovich, Kolesnokov, Zaika</p>"
body += "<h2>Test images</h2>"
body += "<ul>"
body += "<li><a href=\"" + files_path + "/img.jpeg\">Yoda</a></li>"
body += "<li><a href=\"" + files_path + "/img2.jpeg\">Squirrel</a></li>"
body += "<li><a href=\"" + files_path + "/img3.png\">Squirrel</a></li>"
body += "</ul>"
body += "<h3>Music:</h3>"
body += "<ul>"
body += "<li><a href=\"" + files_path + "/music.mp3\">best song ever</a></li>"
body += "</ul>"
body += "<h3>Script:</h3>"  
body += "<ul>"
body += "<li><a href=\"" + files_path + "/s.py\">Script</a></li>"  
body += "<li><a href=\"" + files_path + "/f.py\">I want to say smth</li>"                       
body += "</ul>"        
body += "</body>"
body += "</html>"

headers = ""

headers += "HTTP/1.1 200 OK\r\n"
headers += "Content-Type: text/html; charset=utf-8\r\n"
headers += "Connection: keep-alive\r\n"
headers += "Content-Length: "
headers += (str)(len(body))
headers += "\r\n\r\n"

response = ""

response += headers
response += body

sys.stdout.write( response )
sys.stdout.flush()


