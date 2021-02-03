### 安装vcpkg
```
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat (windows)
./vcpkg/bootstrap-vcpkg.sh  (macosx)
```
### 必装库
```
./vcpkg/vcpkg install CURL
./vcpkg/vcpkg install OpenCV
./vcpkg/vcpkg install OpenSSL
./vcpkg/vcpkg install Boost
./vcpkg/vcpkg install nlohmann-json
./vcpkg/vcpkg install websocketpp
./vcpkg/vcpkg install tinyxml2
./vcpkg/vcpkg install minizip
```