fmill examples and benchmark programs.

running throughtput benchmark:
```sh
# in one terminal tab
./thr_remote tcp://0.0.0.0:7458 1000 10000000
# in another terminal tab
./thr_local tcp://0.0.0.0:7458 1000 10000000

# example output:
# message size: 1000 [B]
# message count: 10000000
# throughput: 911578 [msg/s]
# throughput: 7292.624 [Mb/s]
```

running latency benchmark:
```sh
# in one terminal tab
./lat_remote tcp://0.0.0.0:7458 10 1000000
# in another terminal tab
./lat_local tcp://0.0.0.0:7458 10 1000000

# example output:
# message size: 10 [B]
# roundtrip count: 1000000
# average latency: 11.729 [us]
```