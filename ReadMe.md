#########################################
这部分是在mfast的基础上加上decimalPlaces特性后的安装说明：
level2代码使用的mfast需要是tag v1.2.1版本，并且需要使用动态库（默认不生成动态库），这份代码是在tag v1.2.1的基础上进行修改的，并且把BUILD_SHARED_LIBS由默认的OFF改为ON，因此只需要按照下面的安装步骤操作即可。

安装步骤：
1.下载：1) github:
           $ git clone -b decimalplace-feature --recursive https://github.com/laiqinghui1992/mFAST-1.git  # [-b decimalplace-feature]: decimalplace-feature分支，[--recursive]: 同时更新submodule内容
           内网gitlab: #用户需要先加到该仓库的gitlab群组中
           $ git clone --recursive git@10.199.101.162:hq/mfast.git

           对于github，如果clone代码时没有加上[-b decimalplace-feature]，可以在clone后再切换到decimalplace-feature分支
           对于github和gitlab，如果clone代码时没有加上[--recursive]，需要在clone后进入本地仓库根目录，执行$ git submodule update --recursive --init,否则tinyxml2这个目录下的submodule代码是空的，影响后续编译

2.编译及安装：#假定当前刚执行完clone操作
           $ cd MFAST-1  #进入本地仓库根目录，对于gitlab该命令为$ cd mfast
           $ mkdir build
           $ cd build
           $ cmake ..
           $ make
           $ make install
#########################################
以下为mfast原来的readme内容

# mFAST [![Build Status](https://travis-ci.org/objectcomputing/mFAST.svg?branch=master)](https://travis-ci.org/objectcomputing/mFAST)

### Introduction


mFAST is a high performance C++ encoding/decoding library for FAST (FIX Adapted for STreaming) protocol.

For more information, please read the [article](http://objectcomputing.github.io/mFAST/).

For build instructions, please read the [wiki page](https://github.com/objectcomputing/mFAST/wiki/Installation).

### Important Notice

mFAST has a preliminary support for FAST protocol version 1.2 now. That includes the new `define` and `type` tags in the FAST XML specification and the new enum/boolean types.
In addition, the generated sequence types support iterators now. However, there is no support for *SET*, *BIT GROUP* and *TIMESTAMP* yet.


If you have used mFAST 1.1 version, please notice there are some backward-incompatible changes. First, to make a existing field absent, please use `msg_mref.omit_fieldName()` instead of `msg_mref.set_fieldName().as_absent()`. Second, if you have developed your own visitor for mFAST cref/mref types, the visitor has to be able to visit the new enum cref/mref types.
