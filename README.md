# Purpose
unpack QNX EFS image file

# Build
```
RUN cd dumpefs_cpp && rm -rf build && mkdir build && \
    cd dumpefs_cpp/build && \
    CXX=/usr/bin/g++-11 cmake build .. && make -j $(nproc) 
```

