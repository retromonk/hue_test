# hue_test
## The binary can be found in ./build/bin.

```
Hue Controller
Usage: ./hue_handler [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -n,--hostname TEXT REQUIRED IP address of the host
  -p,--port INT               Port of the host
```

## Ubuntu Development

1.  ```sudo apt-get install python-pip build-essential cmake```
2.  ```pip install conan```
3.  ```conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan```
4.  ```conan remote add conan-center https://conan.bintray.com```
5.  Clone the repository.
6.  ```cd <target_dir>```
7.  ```mkdir build && cd build```
8.  ```cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release```
9.  ```cmake --build .```

## Alternative Build Instructions
```
pip install conan
mkdir build && cd build
conan install ..
```

(win)
``` 
cmake .. -G "Visual Studio 15 Win64"
cmake --build . --config Release
```

(linux, mac)
```
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```