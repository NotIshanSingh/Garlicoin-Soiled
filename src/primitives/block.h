#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <iostream>
#include <string>
#include <stdint.h>
#include <serialize.h>
#include <hash.h>
#include <primitives/transaction.h>

class CBlockHeader {
public:
    // Existing fields for block header
    int nVersion;
    uint256 hashPrevBlock;
    uint256 hashMerkleRoot;
    uint32_t nTime;
    uint32_t nBits;
    uint32_t nNonce;

    // New: QC (quorum certificate) field for PoA
    std::vector<unsigned char> qc;

    // Helper method: Get value hash (signers sign this)
    uint256 GetValueHash() const;

    // Serialization method for block header
    template <typename Stream, typename Operation>
    void Serialize(Stream& s, Operation ser_action) {
        READWRITE(nVersion);
        READWRITE(hashPrevBlock);
        READWRITE(hashMerkleRoot);
        READWRITE(nTime);
        READWRITE(nBits);
        READWRITE(nNonce);
        // Serialize qc (quorum certificate)
        READWRITE(qc);
    }

    // Compute block hash (excluding qc)
    uint256 GetHash() const {
        CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
        ss << nVersion;
        ss << hashPrevBlock;
        ss << hashMerkleRoot;
        ss << nTime;
        ss << nBits;
        ss << nNonce;
        return ss.GetHash();
    }
};

class CBlock : public CBlockHeader {
public:
    std::vector<CTransactionRef> vtx;  // Transactions in the block

    // Serialization for the entire block (including transactions)
    template <typename Stream, typename Operation>
    void Serialize(Stream& s, Operation ser_action)
