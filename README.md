**Build and run the server:**

    git clone https://github.com/markusvaikjarv/async_skeleton.git
    cd async_skeleton
    mkdir build && cd build
    cmake .. && make
    ./server
**Make a GET request**

    curl http://localhost:8080/match/test_request
