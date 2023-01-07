//
// Generated file, do not edit! Created by nedtool 5.1 from Layers/PDCP/pdcpPacket.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __PDCPPACKET_M_H
#define __PDCPPACKET_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0501
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>Layers/PDCP/pdcpPacket.msg:1</tt> by nedtool.
 * <pre>
 * packet pdcpPacket
 * {
 *     string type;
 * }
 * </pre>
 */
class pdcpPacket : public ::omnetpp::cPacket
{
  protected:
    ::omnetpp::opp_string type;

  private:
    void copy(const pdcpPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const pdcpPacket&);

  public:
    pdcpPacket(const char *name=nullptr, short kind=0);
    pdcpPacket(const pdcpPacket& other);
    virtual ~pdcpPacket();
    pdcpPacket& operator=(const pdcpPacket& other);
    virtual pdcpPacket *dup() const override {return new pdcpPacket(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual const char * getType() const;
    virtual void setType(const char * type);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const pdcpPacket& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, pdcpPacket& obj) {obj.parsimUnpack(b);}


#endif // ifndef __PDCPPACKET_M_H

