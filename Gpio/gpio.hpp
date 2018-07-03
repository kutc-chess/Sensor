#include <iostream>

#ifndef GPIO_GPIO_HPP
#define GPIO_GPIO_HPP
/* `cout << a << b << ...` めんどくさいので補助関数 */
template <class... Ts> void println(Ts... xs) {
  (std::cout << ... << xs);
  std::cout << std::endl;
}

/* idとcmdが固定されたやつ */
/* `send(unsigned char, unsigned char, short)`が実装されてる任意のPortに対応する
 */
/* `send(short)`を提供する */
template <class Port> class get_hundle {
  Port &port;
  unsigned char id;
  unsigned char cmd;

public:
  get_hundle(Port &port, const unsigned char id, const unsigned char cmd)
      : port(port), id(id), cmd(cmd) {}

  auto send(const short value) const { return port.send(id, cmd, value); }
};

/* Portの定義を簡単にするやつ */
/* これをpublic継承するとPortからget_hundleを得る`set(id,
 * cmd)`が自動で定義される*/
template <class T> class port_base {
public:
  get_hundle<T> set(const unsigned char id, const unsigned char cmd) {
    return {*static_cast<T *>(this), id, cmd};
  }
};

/* 以下Portの定義例 */
/* port_base<T>を継承してsendとコンストラクタを実装するだけ */
/* サンプルなので実装はめっちゃ適当 */
class async_serial : public port_base<async_serial> {
  const char *path;

public:
  async_serial(const char *path = "/dev/serial1") : path(path) {}

  bool send(const unsigned char id, const unsigned char cmd,
            const short value) {
    println("[ASYNC_SERIAL]", "path: \"", path, "\"; id: ", +id, "; cmd: ",
            +cmd, "; value: ", value);
    return true;
  }
};

class sync_serial : public port_base<sync_serial> {
  const char *path;

public:
  sync_serial(const char *path = "/dev/serial1") : path(path) {}

  int send(const unsigned char id, const unsigned char cmd, const short value) {
    println("[SYNC_SERIAL]", "path: \"", path, "\"; id: ", +id, "; cmd: ", +cmd,
            "; value: ", value);
    return value;
  }
};

class i2c_port : public port_base<i2c_port> {
public:
  i2c_port() {}

  void send(const unsigned char id, const unsigned char cmd,
            const short value) {

    println("[I2C]", "id: ", +id, "; cmd: ", +cmd, "; value: ", value);
  }
};

class wifi_port : public port_base<wifi_port> {
public:
  wifi_port() {}

  void send(const unsigned char id, const unsigned char cmd,
            const short value) {

    println("[WI_FI]", "id: ", +id, "; cmd: ", +cmd, "; value: ", value);
  }
};
#endif
