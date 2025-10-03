#include <primitives/block.h>
#include <crypto/sha256.h>
#include <chainparams.h>

uint256 CBlockHeader::GetHash() const {
    CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
    SerializeHeaderSansQC(ss);
    return ss.GetHash();
}

uint256 CBlockHeader::GetValueHash() const {
    // Return the hash of the header excluding the quorum certificate (used for PoA validation)
    CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
    SerializeHeaderSansQC(ss);
    return ss.GetHash();
}

std::string CBlock::ToString() const {
    std::stringstream ss;
    ss << "CBlock(header=" << GetHash().ToString()
       << " ,time=" << nTime
       << " ,qc=" << (qc.empty() ? "empty" : "present")
       << " ,transactions=" << vtx.size() << ")";
    return ss.str();
}

uint256 CBlock::GetHash() const {
    // We already define GetHash in CBlockHeader. Just call the header's GetHash.
    return CBlockHeader::GetHash();
}
