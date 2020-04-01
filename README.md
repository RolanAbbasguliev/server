<h1>HTTP-server on C++</h1>

<h2>Current stage:</h2><ul>
  <li>multiplex. work with connections</li>
  <li>welcome page - sending by .py script, but hardcoded in it xD</li>
  <li>for each request starting new process via fork() end execve(), data trancfer to child process via env var</li>
  <li>we cathing script's output using pipe() and dup2()</li></ul>

<h2>Coming soon:</h2><ul>
  <li>release GET method</li>
  <li>make welcome page generateg by %.py file</li></ul>
