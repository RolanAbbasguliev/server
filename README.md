<h1>HTTP-server on C++</h1>

<h2>Current stage:</h2><ul>
  <li>multiplex. work with connections</li>
  <li>welcome page - hardcoded</li>
  <li>for each request starting new process via fork() end execve(), data trancfer to child process via env var</li></ul>

<h2>Coming soon:</h2><ul>
  <li>catching python and php output</li>
  <li>release GET method</li>
  <li>make welcome page generateg by %.py file</li></ul>
