<h1>HTTP-server on C++</h1>

Current stage:
  -multiplex. work with connections
  -welcome page - hardcoded
  -for each request starting new process via fork() end execve(), data trancfer to child process via env var
  
  Coming soon:
    -catching python and php output
    -release GET method
    -make welcome page generateg by %.py file
    
