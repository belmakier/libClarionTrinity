#ifndef CLARION_TRINITY_EVENT_HH
#define CLARION_TRINITY_EVENT_HH

#include "Trinity.hh"
#include "Clarion.hh"

#include <libpixie/event.hh>
#include <libpixie/reader.hh>

namespace ClarionTrinity {
  class Event {
  public:
    Clarion clarion;  //clarion array
    Trinity trinity;  //trinity array
    
    Event(Clarion c, Trinity t) { clarion = c; trinity = t; };
    const PIXIE::Event& Set(const PIXIE::Reader &reader, int eventInd); 
  };
}

#endif
