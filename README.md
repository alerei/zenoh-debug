# zenoh-debug

Minimal reproducible example for pub/sub race condition w/zenoh-0.7.0.

The example program creates to "client" sessions for a Zenoh daemon running as Docker container.
`sub_client` connects and subscribes to a `keyexpr`, before `pub_client` also connects and `put`s a value
for this `keyexpr`. The message never arrives at the subscriber.  

As a workaround, wait a set interval between subscribe and publish, which varies depending on the target
platform.

## Build
    git clone https://github.com/alerei/zenoh-debug.git
    cd zenoh-debug
    mkdir build
    cd build
    cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ..
    cmake --build .

## Run
    docker run -d --rm --name zenoh -p 7447:7447 -p 8000:8000 -e RUST_LOG=debug eclipse/zenoh --adminspace-permissions=rw --rest-http-port=8000
    ./zenoh-debug

## Expected behavior
* prints "z_put returned 0"
* prints "lib_subscribe_callback"
* exits with code 0

## Actual behavior
* prints "z_put returned 0"
* runs indefinitely

## Workaround
Remove comment @main.c:55 (usleep (1000))

## Notes
`z_declare_subscriber` returns with success, so one would expect the subscription to be in place at this point. However,
the value published with `z_put` afterwards never arrives unless a certain amount of time has passed between sub and put.
