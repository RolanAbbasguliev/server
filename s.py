#!/usr/bin/env python2.7

import sys
import os

files_path = (str)(os.environ['F_PATH'])

body = ""

body += "<html>"
body += "<head>"
body += "<link rel=\"icon\" href=\"" + files_path + "/favicon.ico\">"
body += "<title>Not Welcome</title>"
body += "<style>"
body += "body {background-image: url(" + files_path + "/backgr.png);}" 
body += "</style>"
body += "</head>"
body += "<body>"
body += "<h1>It's second script-generated page</h1>"
body += "<h2>Made by:</h2>"
body += "<p>Naydenovich, Kolesnokov, Zaika</p>"
body += "<a href=\"" + files_path + "/p.py\">"
body += "<img border=\"0\" alt=\"MyServ\" src=\"back_ico.png\" width=\"50\" height=\"50\">"
body += "</a>"
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




