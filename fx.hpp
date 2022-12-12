#ifndef FX_HPP
#define FX_HPP

class FX {
public:
  FX(): m_fx_next(NULL){};
  virtual void tick();
  virtual void reset();
  virtual bool done();
  virtual ~FX(){};


  void fx_append(FX*fx) {
    if (m_fx_next == NULL) {
      m_fx_next = fx;
    } else {
      m_fx_next->fx_append(fx);
    }
  }

  void fx_chain_tick() {
    tick();
    if (m_fx_next != NULL) {
      m_fx_next -> fx_chain_tick();
    }
  }

  bool fx_chain_reset() {
    this->reset();
    if (m_fx_next != NULL) {
      m_fx_next->fx_chain_reset();
    }
  }

  bool fx_chain_done() {
    return done() || (m_fx_next != NULL ? m_fx_next->fx_chain_done() : false);
  }
protected:
  FX *fx_next() {
    return m_fx_next;
  }
private:
  FX *m_fx_next;
};


#endif  // FX_HPP
