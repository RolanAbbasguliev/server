#!/usr/bin/env python2.7

import sys
import os

files_path = (str)(os.environ['F_PATH'])

body = ""

body += "<html>"
body += "<head>"
body += "<link rel=\"icon\" href=\"" + files_path + "/favicon.ico\">"
body += "<title>Send file</title>"
body += "<style>"
body += "body {background-image: url(" + files_path + "/backgr.png);}" 
body += "</style>"
body += "</head>"
body += "<body>"
body += "<form enctype=\"multipart/form-data\" method=\"post\">"
body += "<p><input type=\"file\" name=\"f\">"
body += "<input type=\"submit\" value=\"Send\"></p>"
body += "</form>"
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
