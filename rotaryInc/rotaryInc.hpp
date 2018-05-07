#include<atomic>

class rotaryInc{
  public:
    rotaryInc(int userA, int userB, bool precision);
    int get();
  private:
    std::atomic<int> pulse;
    int pinA, pinB;
    bool nowA, nowB;
    static void rotary(int gpio, int level, uint32_t tick, void *userdata);
    static void rotaryEx(int gpio, int level, uint32_t tick, void *userdata);
};
