

##  简介
 一个基于th1520，从应用层驱动st7735的例子。（注意IO驱动能力，Licheepi4A需要外置电平转化芯片，自用TXS0108E）
 该例程所有程序均在板子上编译，如有需要交叉编，在编译的时候更换工具链即可。

 ## 开始
 ### 1.下载源码

```shell
git clone https://github.com/fffdee/ST7735_for_Licheepi4A.git
```
### 2. 构建GPIOD库
安装所需要的依赖：
```shell
sudo apt-get install build-essential pkg-config m4 automake autoconf libtool autoconf-archive
sudo apt install gcc g++ gpiod
```
安装GPIOD库(源文件已安装,需要更改库路径参考以下操作)：
```shell
tar zxvf libgpiod-2.0.tar.gz
cd libgpiod-2.0

export CC=gcc
export CXX=g++

#部署库的相关文件到项目文件夹：--prefix后面的路径要换成自己项目所在的路径，方便后续操作
./autogen.sh --enable-tools=yes --prefix=/home/sipeed/TFT_demo/
make
sudo make install
#安装完毕
```

### 3.编译以及运行
```shell
cd TFT_demo
cmake .
make -j4
./tft_demo
```

## 效果图

![效果图1]()
