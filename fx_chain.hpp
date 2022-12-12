#ifndef FX_CHAIN_HPP
#define FX_CHAIN_HPP

#include "fx.hpp"
#include <List.hpp>

class FXChain {
public:
  FXChain(): m_append_cursor(0), m_tick_cursor(0) {}
  void append(FX *fx) {
    if (m_append_cursor >= m_sequence.getSize()) {
      m_sequence.add(fx);
    } else {
      m_sequence[m_append_cursor]->fx_append(fx);
    }
  }

  void append_next_chain() {
    if (m_append_cursor < m_sequence.getSize()) {
      m_append_cursor ++;
    }
  }

  void tick() {
    FX * chain = m_sequence[m_tick_cursor];
    chain->fx_chain_tick();
    if (chain->fx_chain_done()) {
      chain->fx_chain_reset();
      m_tick_cursor ++;
      if (m_tick_cursor >= m_sequence.getSize()) {
        m_tick_cursor = 0;
      }
    }
  }
private:
  int m_append_cursor;
  int m_tick_cursor;
  List<FX*> m_sequence;
};

#endif  // FX_CHAIN_HPP
