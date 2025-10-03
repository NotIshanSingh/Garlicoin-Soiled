#ifndef BITCOIN_PRIMITIVES_BLOCK_H
#define BITCOIN_PRIMITIVES_BLOCK_H

#include <stdint.h>
#include <vector>
#include <string>
#include <sstream>
#include <assert.h>
#include <serialize.h>
#include <chainparams.h>

class CBlockHeader {
public:
    int nVersion;
    uint256 hashPrevBlock;
    uint256 hashMerkleRoot;
    uint32_t nTime;
    uint32_t nBits;
    uint32_t nNonce;
    // New: Quorum Certificate field (QC) to prove agreement for PoA
    std::vector<unsigned char> qc; // The QC signature or other data structure

    ADD_SERIALIZE_METHODS;

    CBlockHeader() : nVersion(1), nTime(0), nBits(0), nNonce(0) { }
    
    uint256 GetHash() const;
    uint256 GetValueHash() const;

    // Serialize/deserialize excluding qc for hashing
    template <typename Stream>
    void SerializeHeaderSansQC(Stream& s) const {
        s << nVersion << hashPrevBlock << hashMerkleRoot << nTime << nBits << nNonce;
    }

    // Serialize/deserialize with qc included (for network transmission)
    template <typename Stream>
    void Serialize(Stream& s) const {
        SerializeHeaderSansQC(s);
        s << qc;
    }

    template <typename Stream>
    void Unserialize(Stream& s) {
        SerializeHeaderSansQC(s);
        s >> qc;
    }
};

// Block represents a full block including the header and transactions.
class CBlock : public CBlockHeader {
public:
    std::vector<std::shared_ptr<CTransaction>> vtx;

    CBlock() : CBlockHeader() {}

    std::string ToString() const;
    uint256 GetHash() const;
};

#endif // BITCOIN_PRIMITIVES_BLOCK_H
