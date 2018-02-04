class E6CP{
  public:
    E6CP(int pin[8]);
    long get();
  private:
    unsigned char Change[256] = {};
    int absolute[8] = {};
    unsigned char rotaryNow = 0;
    unsigned char rotaryPrev = 0;
    long totalNow = 0;
    void readSpecial();
};
