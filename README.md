<h1>HTTP server based on C++ berkeley sockets</h1>
<h2>Current stage:</h2>
<p>launch it and check it out :)</p>

<h2>Coming soon:</h2>
<ul><li>Full-working POST method</li></ul>
    
<h2>Build & run:</h2>
<ul><li>You need to install boost library. <code>brew install boost</code>(macOS) or <code>sudo apt-get install libboost-all-dev</code>(Ubuntu)</li><li>Build: place src files and "files" folder in one directory, then <code>make build</code>(makefile included), and don't worry about some "warnings" :)</li>
<li>Run: <code>make run</code>(src will be rebuilded). To test server use <code>localhost:1234</code></li>
<li>Clear: to rm all .out files and etc. use <code>make clean</code></li></ul>

<h3>Comments:</h3>
<p>Cmake will be added on later stage, as doxygen docs. We support any page-generating langs couse we use env vars and fetching output via pipe to work with them. </p>
