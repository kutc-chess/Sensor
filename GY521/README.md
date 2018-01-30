# GY521
GY-521をRasPiでI2C通信で使うときのライブラリ

## リファレンスのようなもの

```cpp
RPGY521::GY521()
```
* コンストラクタ
* 静止状態であること
* そのうち可変長引数にする

```cpp
RPGY521::GY521::start()
```
* getYaw()をつかう前に呼び出してください

```cpp
double RPGY521::GY521::getYaw()
```
* start()を実行してからの累積のヨー軸の回転角度が返ってくる

```cpp
void RPGY521::GY521::resetYaw()
```
* ヨー軸の累積値をリセットする
