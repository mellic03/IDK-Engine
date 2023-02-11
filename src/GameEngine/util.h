#pragma once



template <typename BitType, typename EnumType>
class BitFlag {

  private:
    BitType _bits = 0;

  public:

    BitType *bits()
    {
      return &this->_bits;
    };

    bool get(EnumType flag)
    {
      return static_cast<BitType>(this->_bits) & static_cast<BitType>(flag);
    };

    void set(EnumType flag, bool value)
    {
      BitType mask  = static_cast<BitType>(flag);
      BitType flags = static_cast<BitType>(this->_bits);

      if (value)
        flags |= mask;
      else
        flags &= ~mask;
      
      this->_bits = static_cast<BitType>(flags);
    };

};
