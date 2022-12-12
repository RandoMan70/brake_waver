#ifndef FX_CHAIN_HPP
#define FX_CHAIN_HPP

#include "fx.hpp"
#include <List.hpp>

class FXChain {
public:
  void append(FX *fx) {
    if (m_sequence.getSize() == 0) {
      m_sequence.add(fx);
    } else {
//      m_sequence.
    }
  }
  void next_effect();

  void tick();
private:
  List<FX*> m_sequence;
};

#endif  // FX_CHAIN_HPP
